#include "descriptor_tables.h"
#include <stdint.h>
#include "print.h"

typedef enum {
    GDT_ACCESS_CODE_READABLE        = 0x02,
    GDT_ACCESS_DATA_WRITEABLE       = 0x02,

    GDT_ACCESS_CODE_CONFORMING      = 0x04,
    GDT_ACCESS_DATA_DIRECTION_NORMAL= 0x00,
    GDT_ACCESS_DATA_DIRECTION_DOWN  = 0x04,

    GDT_ACCESS_DATA_SEGMENT         = 0x10,
    GDT_ACCESS_CODE_SEGMENT         = 0x18,

    GDT_ACCESS_DESCRIPTOR_TSS       = 0x00,

    GDT_ACCESS_RING0                = 0x00,
    GDT_ACCESS_RING1                = 0x20,
    GDT_ACCESS_RING2                = 0x40,
    GDT_ACCESS_RING3                = 0x60,

    GDT_ACCESS_PRESENT              = 0x80,

} GDT_ACCESS;

typedef enum {
    GDT_FLAG_64BIT                  = 0x20,
    GDT_FLAG_32BIT                  = 0x40,
    GDT_FLAG_16BIT                  = 0x00,

    GDT_FLAG_GRANULARITY_1B         = 0x00,
    GDT_FLAG_GRANULARITY_4K         = 0x80,
} GDT_FLAG;

GDTGate g_GDT[5];
GDTDescriptor g_GDTDescriptor = { sizeof(g_GDT), g_GDT };

IDTGate g_IDT[256];
IDTDescriptor g_IDTDescriptor = { sizeof(g_IDT), g_IDT };

void __attribute__((cdecl)) GDT_Load(GDTDescriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);
void __attribute__((cdecl)) IDT_Load(IDTDescriptor* descriptor);

void GDT_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    g_GDT[num].BaseLow = (base & 0xFFFF);
    g_GDT[num].BaseMiddle = (base >> 16) & 0xFF;
    g_GDT[num].BaseHigh = (base >> 24) & 0xFF;

    g_GDT[num].LimitLow = (limit & 0xFFFF);
    g_GDT[num].FlagsLimitHi = (limit >> 16) & 0x0F;
    
    g_GDT[num].FlagsLimitHi |= gran & 0xF0;
    g_GDT[num].Access = access;
}

void GDT_Initialize() {
    //NULL descriptor
    GDT_set_gate(0, 0, 0, 0, 0);

    // Kernel 32-bit code segment
    GDT_set_gate(1,
                0,
                0xFFFFF,
                GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
                GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    
    // Kernel 32-bit data segment
    GDT_set_gate(2,
                0,
                0xFFFFF,
                GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
                GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    
    // User 32-bit code segment
    GDT_set_gate(3,
                0,
                0xFFFFF,
                GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
                GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    
    // User 32-bit data segment
    GDT_set_gate(4,
                0,
                0xFFFFF,
                GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
                GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    
    GDT_Load(&g_GDTDescriptor, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}

static void idt_set_gate(uint32_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    g_IDT[num].BaseLow = base & 0xFFFF;
    g_IDT[num].BaseHigh = (base >> 16) & 0xFFFF;

    g_IDT[num].SegSel = sel;
    g_IDT[num].Always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    g_IDT[num].Flags = flags /* | 0x60 */;
}

void IDT_Initialize() {
    // set the whole thing to null
    for (char* i = g_IDT; i < g_IDT + sizeof(g_IDT); i++)
    {
        *i = 0;
    }
    // remap irq table
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
    // set up all the required gates
    idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (uint32_t)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (uint32_t)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (uint32_t)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (uint32_t)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (uint32_t)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
    //--------------------------------------------
    // set up irq gates
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    IDT_Load(&g_IDTDescriptor);
}

void init_descriptor_tables(){
    GDT_Initialize();
    IDT_Initialize();
}