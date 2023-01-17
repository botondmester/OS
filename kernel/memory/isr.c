#include "isr.h"
#include "print.h"
#include "memory.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else {
        print_str("recieved interrupt: ");
        print_hex8(regs.int_no);
        print_char('\n');
        if(regs.err_code!=0){
            print_str("error code: ");
            print_hex32(regs.err_code);
            print_char('\n');
        }
    }
}

void irq_handler(registers_t regs)
{
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}