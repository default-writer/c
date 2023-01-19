#include "playground/brain/brain.h"
#include <stdio.h>

#ifdef USE_MOCKS
#define TEXT "123\0";
#define fopen(...) (_fopen())
#define fseek(...) (_fseek())
#define ftell(...) (_ftell())
#define fread(...) (_fread(__VA_ARGS__))
#define fclose(...) (_fclose(__VA_ARGS__))
#define FILE char
FILE* _fopen() { return TEXT; }
int _fseek() { return 0; }
long _ftell() { return strlen(_fopen()); }
unsigned long _fread(u8* ptr, size_t size, size_t n, FILE* stream) { sscanf(stream, "%s", ptr); return _ftell(); }
void _fclose(FILE* file) { printf("file closed: %s\n", file); }
#endif

void process(u8 *data) {
    printf("%s\n", data);
}

int main() {
    FILE *f = fopen("input.txt", "rb");
    fseek(f, 0, SEEK_END); 
    u32 size = (u32)ftell(f);
    fseek(f, 0, SEEK_SET);
    u8 *data = calloc(1, size + 1);
    fread(data,1,size,f);
    fclose(f);
    process(data);
    free(data);
    return 0;
}

/**

void process(u8 *data) {
    printf("%s", data);
}

void call(u8* (*_alloc)(), void (*_free)(void*),void(*_function)(u8* data)) {
    u8 *data = _alloc();
    _function(data);
    _free(data);
}


void call_calloc() {
    return calloc(1, size + 1);
}

typedef void (*function)();

*/