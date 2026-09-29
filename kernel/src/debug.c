#include "debug.h"

static inline void outb (uint16_t port, uint8_t val)
{
  __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void debug_init(void) {
    outb(COM1 + 1, 0x00);    // Disable interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB
    outb(COM1 + 0, 0x03);    // Divisor 3 (38400 baud)
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, 1 stop bit
}

static void putc(char c) {
    while ((inb(COM1 + 5) & 0x20) == 0);
    outb(COM1, c);
}

void debug_put(const char *str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') putc('\r');
        putc(str[i]);
    }
}

void debug_put_hex(uint64_t val) {
    debug_put("0x");
    const char *hex_chars = "0123456789ABCDEF";
    for (int i = 60; i >= 0; i -= 4) {
        putc(hex_chars[(val >> i) & 0xF]);
    }
}
