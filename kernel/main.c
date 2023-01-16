#include "print.h"
#include "memory/gdt.h"

kmain(){
    GDT_Initialize();
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to our 32-bit kernel!");
}