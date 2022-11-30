#include <stdlib.h>

// allocated memory, 0 - free, 1 - used
unsigned char memory[256] = {};
// map of used memory
unsigned char map = 0;
// index of first bit on bit field with 0 - free
unsigned char bit = 1;
// bit field for all free & used elements
unsigned char bitfield = 0;
// bit mask
unsigned char bitmask[8] = { 
    0b00000001,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000
};

void add_element()
{
    // bit = 2
    unsigned char old = map;  // 00000000
    map = map | bitmask[bit]; // 11111111
    bit = (map + 1) & map;
}