#include "idt.h"
#include "limine.h"
#include <stdbool.h>
#include <stddef.h>
#include "debug.h"

#define IDT_MAX_DESCRIPTORS 256

extern volatile struct limine_framebuffer_request framebuffer_request;

__attribute__((noreturn))
void exception_handler(uint64_t vector, uint64_t error_code){ 
    debug_put("insied exception_handler");
    debug_put_hex(vector);
    debug_put("  ,   ");
    debug_put_hex(error_code);
    if(framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
  {
    for(;;)
    {
      asm("hlt"); //inline assembly
    }
  }
  
  struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
  //gradient
  volatile uint32_t *fb_ptr = framebuffer->address;
  for (size_t y = 0; y < framebuffer->height; y++) {
      for (size_t x = 0; x < framebuffer->width; x++) {
          uint32_t nX = x * 255 / framebuffer->width;
          uint32_t nY = y * 255 / framebuffer->height;
          fb_ptr[y * (framebuffer->pitch / 4) + x] = (nY << 16) | nX;
      }
  }
    __asm__ volatile ("cli");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void going_ghost_handler(void)
{
    return; //bc ig nothing is supposed to happen:p
}

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

static idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    uint16_t current_cs; 
    __asm__ volatile ("mov %%cs, %0" : "=r"(current_cs));

    idt_entry_t* descriptor = &idt[vector]; 
    *descriptor = (idt_entry_t){0};
    
    descriptor->isr_low        = (uint64_t)isr & 0xFFFF; 
    descriptor->kernel_cs      = current_cs; 
    descriptor->ist            = 0; 
    descriptor->attributes     = flags;
    descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF; 
    descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved       = 0;
}

static bool vectors[IDT_MAX_DESCRIPTORS];
extern void* isr_stub_table[]; //this is from the as, file

void idt_init() {
    idtr.base = (uintptr_t)&idt[0]; 
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint16_t vector = 0; vector < IDT_MAX_DESCRIPTORS; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
}
