#pragma once
#include<stdint.h>

void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);
void cli();
void sti();
static inline void io_wait(void)
{
    outb(0x80, 0);
};