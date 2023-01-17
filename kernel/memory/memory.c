#include "memory.h"

void outb(uint16_t port, uint8_t data){
    asm ("outb %1, %w0" : : "dN" (port), "a" (data));
}

uint8_t inb(uint16_t port)
{
   uint8_t ret;
   asm ("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

void cli(){
    asm("cli");
}

void sti(){
    asm("sti");
}