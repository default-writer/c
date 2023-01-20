#include "rexo/include/rexo.h"
#include "std/common.h"
#include "playground/brain/brain.h"
#include <linux/limits.h>
#include <string.h>

void process(char* data)
{
    printf("%s\n", data);
}

int main(int argc, char** argv) {
    char cwd[PATH_MAX];
    if (argc > 0) {
        strcpy(cwd, argv[0]);
    }
    char* p = strrchr(cwd, '/');
    if (p != 0) {
        *p = 0;
    }
    strcat(cwd, "/input.txt");
    FILE *f = fopen(cwd, "rb");
    if (f != 0) { 
        fseek(f, 0, SEEK_END);
    } else {
        perror("file not found");
        exit(-1);
    }
    u32 size = (u32)ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = calloc(1, size + 1);
    fread(data,1,size,f);
    fclose(f);
    process(data);
    free(data);
    return 0;
}