#include "../include/archiver.h"
#include <indicators/cursor_control.hpp>
#include <iomanip> // For percentage formatting

// Function to get file size that works with large files
int64_t get_file_size(FILE* file) {
#if defined(_WIN32) || defined(_WIN64)
    // On Windows, use _ftelli64 for large file support
    return _ftelli64(file);
#else
    // On POSIX systems, ftello should work with _FILE_OFFSET_BITS=64
    return ftello(file);
#endif
}

// Function to set file position that works with large files
int set_file_position(FILE* file, int64_t position, int origin) {
#if defined(_WIN32) || defined(_WIN64)
    // On Windows, use _fseeki64 for large file support
    return _fseeki64(file, position, origin);
#else
    // On POSIX systems, fseeko should work with _FILE_OFFSET_BITS=64
    return fseeko(file, position, origin);
#endif
}

// Convert bytes to human-readable format
std::string format_bytes(int64_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024 && unit < 4) {
        size /= 1024;
        unit++;
    }
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << size << " " << units[unit];
    return ss.str();
}

int def(FILE* source, FILE* dest, int level) {
    int ret, flush;
    unsigned have;
    z_stream strm;
    vector<unsigned char> in(CHUNK);
    vector<unsigned char> out(CHUNK);

    show_console_cursor(false);

    // Use 64-bit file functions for large files
    set_file_position(source, 0, SEEK_END);
    int64_t file_size = get_file_size(source);
    set_file_position(source, 0, SEEK_SET);
    
    // Check for valid file size
    if (file_size < 0) {
        std::cerr << "Error: Could not determine file size or file is too large." << std::endl;
        show_console_cursor(true);
        return Z_ERRNO;
    }
    
    std::cout << "File size: " << format_bytes(file_size) << std::endl;
    
    ProgressBar bar{
        option::BarWidth{80},
        option::ForegroundColor{Color::white},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
        option::MaxProgress{static_cast<size_t>(file_size)},
        option::Fill{"="},
        option::Lead{">"},
        option::Remainder{" "},
        option::ShowPercentage{true},
        option::ShowElapsedTime{true},
        option::ShowRemainingTime{true},
    };

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    ret = deflateInit(&strm, level);

    int64_t total_read = 0;

    /* compress until end of file */
    do {
        strm.avail_in = static_cast<uInt>(fread(in.data(), 1, CHUNK, source));
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            show_console_cursor(true);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in.data();

        total_read += strm.avail_in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out.data();
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out.data(), 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                show_console_cursor(true);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */
        
        bar.set_progress(static_cast<size_t>(total_read));
        
        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */
    
    std::cout << std::endl;

    /* clean up and return */
    (void)deflateEnd(&strm);

    show_console_cursor(true);

    return Z_OK;
}

int inf(FILE* source, FILE* dest) {
    int ret;
    unsigned have;
    z_stream strm;
    vector<unsigned char> in(CHUNK);
    vector<unsigned char> out(CHUNK);

    show_console_cursor(false);

    // Use 64-bit file functions for large files
    set_file_position(source, 0, SEEK_END);
    int64_t file_size = get_file_size(source);
    set_file_position(source, 0, SEEK_SET);
    
    // Check for valid file size
    if (file_size < 0) {
        std::cerr << "Error: Could not determine file size or file is too large." << std::endl;
        show_console_cursor(true);
        return Z_ERRNO;
    }
    
    std::cout << "File size: " << file_size << " bytes" << std::endl;

    ProgressBar bar{
        option::BarWidth{80},
        option::ForegroundColor{Color::white},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
        option::MaxProgress{static_cast<size_t>(file_size)},
        option::Fill{"="},
        option::Lead{">"},
        option::Remainder{" "},
        option::ShowPercentage{true},
        option::ShowElapsedTime{true},
        option::ShowRemainingTime{true},
    };

    int64_t total_read = 0;

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        show_console_cursor(true);
        return ret;
    }

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = static_cast<uInt>(fread(in.data(), 1, CHUNK, source));
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            show_console_cursor(true);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in.data();

        total_read += strm.avail_in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out.data();
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
                [[fallthrough]];
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                show_console_cursor(true);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            if (fwrite(out.data(), 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                show_console_cursor(true);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        bar.set_progress(static_cast<size_t>(total_read));

        //if (total_read < file_size) {
        //    bar.set_progress(static_cast<size_t>(total_read));
        //    // No need to show byte counts for extraction
        //}
        //else {
        //    bar.set_progress(static_cast<size_t>(file_size));
        //    // No need to show byte counts for extraction
        //}

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    std::cout << std::endl;

    /* clean up and return */
    (void)inflateEnd(&strm);

    show_console_cursor(true);

    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void zerr(int ret) {
    fputs("archiver: ", stderr);
    switch (ret) {
    case Z_ERRNO:
        if (ferror(stdin)) {
            fputs("error reading stdin\n", stderr);
        }
        if (ferror(stdout)) {
            fputs("error writing stdout\n", stderr);
        }
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}