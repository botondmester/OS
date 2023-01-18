#include "print.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

// Video Memory begins at 0x000B8000
struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        clear_row(i);
    }
}

void print_newline() {
    col = 0;

    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row(NUM_COLS - 1);
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (col > NUM_COLS) {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char) {
        character: (uint8_t) character,
        color: color,
    };

    col++;
}

void print_str(char* str) {
    for (size_t i = 0;; i++) {
        char character = (uint8_t) str[i];

        if (character == '\0') {
            return;
        }

        print_char(character);
    }
}

void print_hex32(uint32_t hex) {
    print_char('0');
    print_char('x');
    for (size_t i = 8; i > 0; i--) {
        char character = (hex >> (i*4-4)) & 0xF;
        character += 48;
        if(character > 57) character += 7;

        print_char(character);
    }
}
void print_hex18(uint16_t hex) {
    print_char('0');
    print_char('x');
    for (size_t i = 4; i > 0; i--) {
        char character = (hex >> (i*4-4)) & 0xF;
        character += 48;
        if(character > 57) character += 7;

        print_char(character);
    }
}
void print_hex8(uint8_t hex) {
    print_char('0');
    print_char('x');
    for (size_t i = 2; i > 0; i--) {
        char character = (hex >> (i*4-4)) & 0xF;
        character += 48;
        if(character > 57) character += 7;

        print_char(character);
    }
}
void print_dec(uint32_t n){
    if (n == 0)
    {
        print_char('0');
        return;
    }

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    print_str(c2);
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground + (background << 4);
}