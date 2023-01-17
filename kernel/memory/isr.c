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
    uint32_t tmp = regs.int_no;
    uint8_t int_no = tmp;
    tmp = regs.err_code;
    
    print_str("recieved interrupt: ");
    print_hex8(int_no);
    print_char('\n');
    if(tmp!=0){
        print_str("error code: ");
        print_hex32(tmp);
        print_char('\n');
    }
}

void irq_handler(registers_t regs)
{
    uint32_t tmp = regs.int_no;
    uint8_t int_no = tmp;
    if (int_no >= 40)
    {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);

    if (interrupt_handlers[int_no] != 0)
    {
        isr_t handler = interrupt_handlers[int_no];
        handler(regs);
    }
}