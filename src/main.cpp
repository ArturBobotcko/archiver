#include "../include/archiver.h"

int main(int argc, char** argv) {
    int ret;

    if (argc < 4) {
        fputs("archiver usage:\narchiver a <original file> <archive file> - to archive a file\narchiver e <archive file> <original file> - to extract a file", stderr);
        return 1;
    }
    else if (argc == 4) {
        if (strcmp(argv[1], "a") == 0) {
            FILE* original_file;
            FILE* archive_file;
            
            if (fopen_s(&original_file, argv[2], "rb") != 0) {
                perror("Error opening original file");
                return 1;
            }

            if (fopen_s(&archive_file, argv[3], "wb") != 0) {
                perror("Error opening archive file");
                return 1;
            }

            ret = def(original_file, archive_file, 2);
            if (ret != Z_OK) {
                zerr(ret);
            }

            cout << argv[2] << " was successfully archived to " << argv[3] << endl;

            fclose(original_file);
            fclose(archive_file);
        }
        else if (strcmp(argv[1], "e") == 0) {
            FILE* archive_file;
            FILE* original_file; 

            if (fopen_s(&archive_file, argv[2], "rb") != 0) {
                perror("Error opening archive file");
                return 1;
            }

            if (fopen_s(&original_file, argv[3], "wb") != 0) {
                perror("Error opening original file");
                return 1;
            }

            ret = inf(archive_file, original_file);
            if (ret != Z_OK) {
                zerr(ret);
            }

            cout << argv[2] << " was successfully extracted to " << argv[3] << endl;

            fclose(archive_file);
            fclose(original_file);
        }
    }
}
