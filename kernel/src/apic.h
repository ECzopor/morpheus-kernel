#pragma once
#include <stdint.h>

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0/* IO base address for slave PIC */
#define PIC1_DATA (PIC1+1)
#define PIC2_DATA (PIC2+1)
#define LAPIC_PHYS_BASE 0xFEE00000

// LAPIC Register Offsets
#define TPR   0x0080  // Task Priority Register
#define EOI   0x00B0  // End of Interrupt Register
#define SVR   0x00F0  // Spurious Interrupt Vector Register
#define LVTT  0x0320  // LVT Timer Register
#define TDCR  0x0380  // Timer Divide Configuration Register
#define TICR  0x0390  // Timer Initial Count Register

void apic_init(uint64_t hhdm_offset);
void eoi(void);
