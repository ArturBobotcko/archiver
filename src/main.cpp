#include "../include/archiver.h"

FILE* g_file = NULL;
char* g_filename = NULL;
bool g_is_extracting = false;

void sigint_handler(int signum) {
    if (g_file) {
        fclose(g_file);
        g_file = NULL;
    }
    
    if (g_filename) {
        remove(g_filename);
        if (g_is_extracting) {
            printf("\nOperation canceled. Extracted file deleted: %s\n", g_filename);
        } else {
            printf("\nOperation canceled. Archive file deleted: %s\n", g_filename);
        }
    }
    
    exit(1);
}

int main(int argc, char** argv) {
    int ret;

    signal(SIGINT, sigint_handler);

    if (argc < 4) {
        fputs("archiver usage:\narchiver a <original file> <archive file> - to archive a file\narchiver e <archive file> <original file> - to extract a file\n", stderr);
        return 1;
    }
    else if (argc == 4) {
        if (strcmp(argv[1], "a") == 0) {
            FILE* original_file = fopen(argv[2], "rb");
            g_file = fopen(argv[3], "wb");
            g_filename = argv[3];
            g_is_extracting = false;
            
            if (!original_file) {
                perror("Error opening original file");
                return 1;
            }

            if (!g_file) {
                perror("Error opening archive file");
                return 1;
            }

            ret = def(original_file, g_file, 2);
            if (ret != Z_OK) {
                zerr(ret);
            }

            cout << argv[2] << " was successfully archived to " << argv[3] << endl;

            fclose(original_file);
            fclose(g_file);
            g_file = NULL;
            g_filename = NULL;
        }
        else if (strcmp(argv[1], "e") == 0) {
            FILE* archive_file = fopen(argv[2], "rb");
            g_file = fopen(argv[3], "wb");
            g_filename = argv[3];
            g_is_extracting = true;

            if (!archive_file) {
                perror("Error opening archive file");
                return 1;
            }

            if (!g_file) {
                perror("Error opening original file");
                return 1;
            }

            ret = inf(archive_file, g_file);
            if (ret != Z_OK) {
                zerr(ret);
            }

            cout << argv[2] << " was successfully extracted to " << argv[3] << endl;

            fclose(archive_file);
            fclose(g_file);
            g_file = NULL;
            g_filename = NULL;
        }
    }
}
