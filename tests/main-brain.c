#include "rexo/include/rexo.h"
#include "std/common.h"
#include "playground/brain/brain.h"

void process(char* data) {
    printf("%s\n", data);
}

void get_full_path(int argc, char** argv, char* path, const char* file_name) {
    if (argc > 0) {
        strcpy(path, argv[0]);
    }
    char* p = strrchr(path, '/');
    if (p != 0) {
        *p = 0;
    }
    strcat(path, file_name);
}

long get_file_size(FILE* f) {
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

int main(int argc, char** argv) {
    char cwd[PATH_MAX];
    FILE *f;
    char* data;
    get_full_path(argc, argv, cwd, "/input.txt");
    if ((f = fopen(cwd, "rb")) != 0) {
        u32 size = (u32)get_file_size(f);
        data = calloc(1, size + 1);
        fread(data,1,size,f);
        fclose(f);
        process(data);
        free(data);
    }
    return 0;
}