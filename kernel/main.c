#include "print.h"
#include "memory/descriptor_tables.h"
#include "timer.h"
#include "memory/paging.h"

kmain(){
    print_clear();
    init_descriptor_tables();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to our 32-bit kernel!\n");
    initialise_paging();
    print_str("And now we are paging!\n");
    //asm volatile ("int $0x3"); // artificially trigger int 0x3
    //init_timer(50);

    uint32_t *ptr = (uint32_t*)0xA0000000;
    uint32_t do_page_fault = *ptr;

    return 0;
}