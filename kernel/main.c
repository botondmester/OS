#include "print.h"
#include "memory/descriptor_tables.h"
#include "timer.h"
#include "memory/memory.h"

kmain(){
    print_clear();
    init_descriptor_tables();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to our 32-bit kernel!\n");
    initialise_paging();
    print_str("And now we are paging!\n");
    //asm volatile ("int $0x3"); // artificially trigger int 0x3
    //init_timer(50); // init PIT at 100hz

    uint32_t a = kmalloc(8);
    uint32_t b = kmalloc(8);
    print_hex32(a);
    print_char('\n');
    print_hex32(b);
    print_char('\n');
    kfree(a);
    kfree(b);
    uint32_t c = kmalloc(12);
    print_hex32(b);
    print_char('\n');

    return 0;
}