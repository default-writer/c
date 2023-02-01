#include "std/common.h"

void int_to_char(int input, int pointer, char* output) {
    switch (input) {
    case 0:
        output[pointer] = '0';
        break;
    case 1:
        output[pointer] = '1';
        break;
    case 2:
        output[pointer] = '2';
        break;
    case 3:
        output[pointer] = '3';
        break;
    case 4:
        output[pointer] = '4';
        break;
    case 5:
        output[pointer] = '5';
        break;
    case 6:
        output[pointer] = '6';
        break;
    case 7:
        output[pointer] = '7';
        break;
    case 8:
        output[pointer] = '8';
        break;
    case 9:
        output[pointer] = '9';
        break;
    case 10:
        output[pointer] = 'A';
        break;
    case 11:
        output[pointer] = 'B';
        break;
    case 12:
        output[pointer] = 'C';
        break;
    case 13:
        output[pointer] = 'D';
        break;
    case 14:
        output[pointer] = 'E';
        break;
    case 15:
        output[pointer] = 'F';
        break;
    default:
        output[0] = 'x';
        output[1] = 'x';
        output[2] = 'x';
    }
}

int aprx(int input) {
    if (input < 0) {
        return 0;
    } else if (input > 255) {
        return 255;
    } else {
        return input;
    }
}

// указатель в плане смещения относительно конца массива
int pointer = 0; 

// char* output - выходной массив
void dec_to_hex(int dec_input, char* output) {
    int_to_char((dec_input % 16), pointer, &output[0]);
    dec_input = dec_input / 16;
    pointer = pointer - 1;
    int_to_char((dec_input % 16), pointer, &output[0]);
    pointer = pointer - 1;
}

int rgb(int r, int g, int b, char* output) {
    r = aprx(r);
    g = aprx(g);
    b = aprx(b);

    dec_to_hex(b, &output[0]);
    dec_to_hex(g, &output[0]);
    dec_to_hex(r, &output[0]);

    return 0;
}

int main() {

    char buffer[8] = { '0',  'x', ' ', ' ', ' ', ' ', ' ', ' ' };

    pointer = strlen(buffer) - 1;

    rgb(12, 33, 18, buffer); // точка, где буфер корраптится

    printf("%s\n", buffer);

    // cout<< buffer << "\n";
    return 0;
}
