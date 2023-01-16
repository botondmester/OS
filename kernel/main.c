#include "print.h"
#include "memory/descriptor_tables.h"

kmain(){
    init_descriptor_tables();
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to our 32-bit kernel!\n");
    asm volatile ("int $0x3"); // artificially trigger int 0x3
}