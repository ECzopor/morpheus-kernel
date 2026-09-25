#include "apic.h" //should i put any other includes?


static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1": : "a"(val), "Nd"(port));
}

static void mask_pic(void)
{
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void apic_init(void) //static so other porgrams (files)can use it right?
{
    mask_pic();
}
