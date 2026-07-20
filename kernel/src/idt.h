#pragma once
#include <stdint.h>
////IDT ENTRY:
typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR --so for my null/kernel code and data it will alywas be 08
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now - i dont have the TSS yet right -> thats why its 0
	uint8_t     attributes;   // Type and attributes; see the IDT page 
  uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address - why not just keep them together -> bc we want backwards compability
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed)) idt_entry_t;

//IDTR - interrupt descriptor table register

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtr_t;
