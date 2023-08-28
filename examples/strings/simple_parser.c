#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// static u64 load_data() {
//     u64 list_ptr = list->alloc();
//     u64 reversed_list_ptr = list->alloc();
//     u64 file_path_ptr = string->load("c-simple-parser.txt");
//     u64 mode_ptr = string->load("rb");
//     u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
//     if (f_ptr != 0) {
//         u64 file_data_ptr = file->data(f_ptr);
//         u64 size = data->size(file_data_ptr);
//         if (size > 100) {
//             size = 100;
//         }
//         file->free(f_ptr);
//         u8* file_data = data->unsafe(file_data_ptr);
//         u8* tmp = file_data;
//         while (*tmp != 0 && size > 0) {
//             while (*tmp != 0 && *tmp != '\n') {
//                 tmp++;
//                 size--;
//             }
//             *tmp++ = '\0';
//             size--;
//             u64 string_ptr = string->load((char*)file_data);
//             list->push(list_ptr, string_ptr);
//             file_data = tmp;
//         }
//         u64 data_ptr = 0;
//         while ((data_ptr = list->pop(list_ptr)) != 0) {
//             list->push(reversed_list_ptr, data_ptr);
//         }
//         list->free(list_ptr);
//     }
//     string->free(mode_ptr);
//     return reversed_list_ptr;
// }

// static u64 read_input(const char* prompt) {
//     char buffer[100];
//     global_memset(&buffer, 0, 100);
//     printf(">%s:\n", prompt);
//     char ch = 0;
//     for (int i = 0; i < 100; i++) {
//         ch = (char)getchar();
//         if (ch == EOF || ch == '\n') {
//             break;
//         }
//         buffer[i] = ch;
//     }
//     u64 data_ptr = string->load((const char*)&buffer);
//     return data_ptr;
// }

// static u64 read_data(u64 list_ptr, const char* prompt) {
//     u64 data_ptr = 0;
//     u64 ui_mode_ptr = string->load("UI_MODE");
//     u64 mode_ptr = string->load("--ui");
//     u64 value_ptr = os->getenv(ui_mode_ptr);
//     if (ui_mode_ptr != 0 && string->strcmp(value_ptr, mode_ptr) != 0) {
//         data_ptr = read_input(prompt);
//     } else {
//         printf(">%s:\n", prompt);
//         data_ptr = list->pop(list_ptr);
//         if (data_ptr == 0) {
//             printf("\n");
//         }
//     }
//     return data_ptr;
// }

typedef void (*f)(char** c);

f _;

void _a(char** c);
void _b(char** c);
void _c(char** c);
void _q(char** c);

int result = 0;

/*

    a -> (b
    b -> )c
    c -> \0_
      -> a

*/

void _a(char** c) {
    if (**c == '(') {
        _ = _b;
        (*c)++;
    } else {
        _ = _q;
    }
}

void _b(char** c) {
    if (**c == ')') {
        _ = _c;
        (*c)++;
    } else {
        _ = _q;
    }
}

void _c(char** c) {
    if (**c == '\0') {
        result = 1;
        _ = _q;
    } else {
        _ = _a;
    }
}

void _q(char **c) {
    _ = 0;
}

char* input = 0;

static void read_input() {
    memset(input, 0, 101);
    printf(">");
    char ch = 0;
    for (int i = 0; i < 100; i++) {
        ch = (char)getchar();
        if (ch == EOF || ch == '\n') {
            break;
        }
        input[i] = ch;
    }
}

int main(void) {
    input = calloc(1, 101);
    while (1) {
        read_input();
        if (*input == 'q') {
            break;
        }
        _ = _a;
        while(_ != 0) {
            _(&input);
        }
        if (*input != '\0') {
            result = 0;
        }
        printf("%d\n",result);
        result = 0;
    }
    return 0;
}