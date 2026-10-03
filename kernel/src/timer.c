#include "timer.h"
#include "limine.h"
#include <stddef.h>
#include "debug.h"

extern volatile struct limine_framebuffer_request framebuffer_request;

uint64_t volatile ticks=0; //why do why need volatile here?

void timer_handler()
{
    ticks++;
    eoi();
    debug_put("TICK! ");
    //LATER Inside the timer interrupt we can do scheduling, check limits and sleep queues?
    //gradients for testing:
    
}

void sleep(uint64_t time)
{
    if(framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
      for(;;)
      {
        __asm__ volatile("hlt"); //inline assembly
      }
    }

        struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    
   //gradient for testing
   volatile uint32_t *fb_ptr = framebuffer->address;
   for (size_t y = 0; y < framebuffer->height; y++) {
       for (size_t x = 0; x < framebuffer->width; x++) {
           uint32_t nX = x * 255 / framebuffer->width;
           uint32_t nY = y * 255 / framebuffer->height;
           fb_ptr[y * (framebuffer->pitch / 4) + x] = (nX << 8) | (nY << 16) | (255-nX);
       }
   }

  uint64_t target = ticks+time;
    while(ticks < target)
    {
        __asm__ volatile ("hlt");
    }

}
