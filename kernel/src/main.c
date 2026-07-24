#include <stdint.h>//for specific type size (uint64_t)
#include <stddef.h>//for size_t, NULLs and so on
#include <stdbool.h>//bool
#include <limine.h>

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
  .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

//this one can copy ANYTHING (therefore the void ptr) and precoesses this byte by byte (that's why there is the uint8_t); restricted - a PROMISE dest and src do not overlap in RAM (restricted - there is only one pointer to this thing)
void *memcpy(void *restrict dest, const void* restrict src, size_t n)
{
  uint8_t *restrict pdest = dest;
  const uint8_t *restrict psrc = src;

  for (size_t i =0; i<n; i++)
  {
    pdest[i] = psrc[i];
  }

  return dest;
}

//takes an existing block in memory (s) and fills with c (used for cleaning:])
void *memset(void* s, int c, size_t n)
{
  uint8_t *p = s;

  for(size_t i=0; i<n; i++)
  {
    p[i] = (uint8_t)c;
  }

  return s;
}

//when dest and src overlap
void memmove (void *dest, const void* src, size_t n)
{
  uint8_t *pdest = dest;
  const uint8_t *psrc = src;

  if((uintptr_t)src > (uintptr_t)dest)
  {
    for(size_t i=0; i<n; i++)
    {
      pdest[i] = psrc[i];
    }
  }else if((uintptr_t)src < (uintptr_t)dest)
  {
    for(size_t i=n; i>0; i--)
    {
      pdest[i-1] = psrc[i-1];
    }
  }

  return dest;
}

//memory compare
int memcmp(const void *s1, const void *s2, size_t n)
{
  const uint8_t *p1 = s1;
  const uint8_t *p2 = s2;

  for (size_t i=0; i<n; i++)
  {
    if(p1[i] != p2[i])
    {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }
  return 0;
}

//when there is fire:
static void hcf(void)
{
  for(;;)
  {
    asm("hlt"); //inline assembly
  }
}

void kmain(void)
{
  //we have a base revision?
  if(LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
  {
    hcf();
  }

  //we have a framebuffer? (karta graficzna - ekran)
  if(framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
  {
    hcf();
  }
  
  struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

  //gradient
  volatile uint32_t *fb_ptr = framebuffer->address;
  for (size_t y = 0; y < framebuffer->height; y++) {
      for (size_t x = 0; x < framebuffer->width; x++) {
           uint32_t nX = x * 205 / framebuffer->width;
          uint32_t nY = y * 255 / framebuffer->height;
          fb_ptr[y * (framebuffer->pitch / 4) + x] = (nY << 8) | nX;
      }
  }

  // potem w pamieci smieci wiec nie chcemy ich czytac   
  hcf();
}
