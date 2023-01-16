#include "isr.h"
#include "print.h"

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