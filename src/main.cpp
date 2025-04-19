#include "../include/archiver.h"

int main(int argc, char** argv) {
    int ret;

    if (argc < 4) {
        fputs("archiver usage:\narchiver a <original file> <archive file> - to archive a file\narchiver e <archive file> <original file> - to extract a file\n", stderr);
        return 1;
    }
    else if (argc == 4) {
        if (strcmp(argv[1], "a") == 0) {
            FILE* original_file = fopen(argv[2], "rb");
            FILE* archive_file = fopen(argv[3], "wb");
            
            if (!original_file) {
                perror("Error opening original file");
                return 1;
            }

            if (!archive_file) {
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
            FILE* archive_file = fopen(argv[2], "rb");
            FILE* original_file = fopen(argv[3], "wb");

            if (!archive_file) {
                perror("Error opening archive file");
                return 1;
            }

            if (!original_file) {
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
