#include "apic.h"

uint64_t lapic_base;

static inline void write(uint32_t reg, uint32_t val)
{
  volatile uint32_t* ptr = (volatile uint32_t*)(lapic_base + reg);
  *ptr = val;
}

static inline uint32_t read(uint32_t reg)
{
  volatile uint32_t* ans = (volatile uint32_t*)(lapic_base + reg);
  return *ans;
}

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1": : "a"(val), "Nd"(port));
}

static void mask_pic(void)
{
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void eoi(void)
{
    write(EOI, 0x0);
}

void apic_init(uint64_t hhdm_offset) //static so other porgrams (files)can use it right?
{   //limine has already done this for my actully, but I still wrote my own, so if I ever wanna change the bootloader this is safe
    mask_pic();
    //there are some other stuff that limine has done for me like: "- The local APIC is enabled (`IA32_APIC_BASE` bit 11) and software-enabled (SVR bit 8). - The Spurious Interrupt Vector Register is set to `0x1FF`.- The Task Priority Register is set to 0." - /morpheus-kernel/kernel/limine-protocol/PROTOCOL.md
    lapic_base = (uint64_t)(LAPIC_PHYS_BASE + hhdm_offset);
    

    //also done by limine
    write(SVR, 0x1FF); //going ghost register>:)
    write(TPR, 0x0); //task pro to 0

    write(TDCR, 0x3); //divide by 16 like on os dev
    write(LVTT, 32 | (1 << 17)); //LVTT takes care of the timer interrupt (vec32), periodic mode
    write(TICR, 10000000); //no calibrating, just hardcoding the val
}
