#include "apic.h"
#include "debug.h"
#include <limine.h>
extern volatile struct limine_mp_request mp_request;

static inline void write(uint32_t reg_offset, uint64_t val) {
 //MMIO increpented by 16 and we are incrementing by 1 so bit shift
   uint32_t msr = X2APIC_MSR_BASE + (reg_offset >> 4);
    uint32_t low = (uint32_t)val;
    uint32_t high = (uint32_t)(val >> 32);

    __asm__ volatile ("wrmsr" : : "a"(low), "d"(high), "c"(msr) : "memory");
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

void apic_init() 
{   //limine has already done this for my actully, but I still wrote my own, so if I ever wanna change the bootloader this is safe
    if (mp_request.response == NULL || !(mp_request.response->flags & LIMINE_MP_RESPONSE_X86_64_X2APIC)) {
        debug_put("x2APIC not supported :[[[[\n");
        return;
    }
    mask_pic();
    //there are some other stuff that limine has done for me like: "- The local APIC is enabled (`IA32_APIC_BASE` bit 11) and software-enabled (SVR bit 8). - The Spurious Interrupt Vector Register is set to `0x1FF`.- The Task Priority Register is set to 0." - /morpheus-kernel/kernel/limine-protocol/PROTOCOL.md
  
    //also done by limine
    write(SVR, 0x1FF); //going ghost register>:)
    write(TPR, 0x0); //task pro to 0

    write(TDCR, 0x3); //divide by 16 like on os dev
    uint32_t timer_mode_periodic = (1 << 17);
    write(LVTT, 32 | timer_mode_periodic);//LVTT takes care of the timer interrupt (vec32), periodic mode
    write(TICR, 10000000); //no calibrating, just hardcoding the val //small val bc i wanna SEE:pp
}
