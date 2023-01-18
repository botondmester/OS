#include "assert.h"
#include "print.h"

extern void panic(const char *message, const char *file, uint32_t line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    print_str("PANIC(");
    print_str(message);
    print_str(") at ");
    print_str(file);
    print_str(":");
    print_dec(line);
    print_str("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

extern void panic_assert(const char *file, uint32_t line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    print_str("ASSERTION-FAILED(");
    print_str(desc);
    print_str(") at ");
    print_str(file);
    print_str(":");
    print_dec(line);
    print_str("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
