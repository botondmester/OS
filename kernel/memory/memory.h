#pragma once
#include<stdint.h>
#include "kheap.h"
#include "paging.h"
#include "assert.h"

void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);
void cli();
void sti();
static inline void io_wait(void)
{
    outb(0x80, 0);
};

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
void memset(uint8_t *dest, uint8_t val, uint32_t len);
int strcmp(char *str1, char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);