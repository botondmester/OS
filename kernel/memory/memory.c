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

// Copy len bytes from src to dest.
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Write len copies of val into dest.
void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
    do
    {
      *dest++ = *src++;
    }
    while (*src != 0);
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src)
{
    while (*dest != 0)
    {
        *dest = *dest++;
    }

    do
    {
        *dest++ = *src++;
    }
    while (*src != 0);
    return dest;
}

extern uint32_t kernel_end;
uint32_t placement_address = (uint32_t)&kernel_end;

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys)
{
  if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (phys)
  {
    *phys = placement_address;
  }
  uint32_t tmp = placement_address;
  placement_address += sz;
  return tmp;
}

uint32_t kmalloc_a(uint32_t sz){
    return kmalloc_int(sz, 1, 0);
}
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys){
    return kmalloc_int(sz, 0, phys);
}
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys){
    return kmalloc_int(sz, 1, phys);
}
uint32_t kmalloc(uint32_t sz){
    return kmalloc_int(sz, 0, 0);
}