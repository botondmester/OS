#pragma once
#include <stdint.h>

#define GDT_CODE_SEGMENT 0x08
#define GDT_DATA_SEGMENT 0x10

typedef struct {
    uint16_t LimitLow;      // limit    (bits 0-15)
    uint16_t BaseLow;       // base     (bits 0-15)
    uint8_t  BaseMiddle;    // base     (bits 16-23)
    uint8_t  Access;        // access
    uint8_t  FlagsLimitHi;  // limit    (bits 16-19) | flags
    uint8_t  BaseHigh;      // base     (bits 24-31)
}__attribute__((packed)) GDTGate;

typedef struct {
    uint16_t Limit;         // sizeof(gdt) - 1
    GDTGate* ptr;          // address of GDT
}__attribute__((packed)) GDTDescriptor;

typedef struct {
    uint16_t BaseLow;
    uint16_t SegSel;
    uint8_t Always0;
    uint8_t Flags;
    uint16_t BaseHigh;
}__attribute__((packed)) IDTGate;

typedef struct {
    uint16_t Limit;
    IDTGate* ptr;
}__attribute__((packed)) IDTDescriptor;

void init_descriptor_tables();

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();