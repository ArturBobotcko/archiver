#include "../include/archiver.h"

int main(int argc, char** argv)
{
    int ret;

    if (argc < 4) {
        fputs("archiver usage: archiver a <original file> <archive file> - to archive a file\n", stderr);
        fputs("archiver usage: archiver e <archive file> <original file> - to extract a file", stderr);
        return 1;
    }
    else if (argc == 4) {
        if (strcmp(argv[1], "a") == 0) {
            FILE* original_file = fopen(argv[2], "rb");
            FILE* archive_file = fopen(argv[3], "wb");

            if (original_file == NULL) perror("Error opening original file");
            if (archive_file == NULL) perror("Error opening archive file");

            int ret = def(original_file, archive_file, Z_DEFAULT_COMPRESSION);
            if (ret != Z_OK) {
                zerr(ret);
            }

            fclose(original_file);
            fclose(archive_file);
        }
        else if (strcmp(argv[1], "e") == 0) {
            FILE* archive_file = fopen(argv[2], "rb");
            FILE* original_file = fopen(argv[3], "wb");

            if (original_file == NULL) perror("Error opening original file");
            if (archive_file == NULL) perror("Error opening archive file");

            int ret = inf(archive_file, original_file);
            if (ret != Z_OK) {
                zerr(ret);
            }

            fclose(archive_file);
            fclose(original_file);
        }
    }
}
