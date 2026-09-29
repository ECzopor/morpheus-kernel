#include <stdint.h>
#include <stddef.h>

#define COM1 0x3F8
void debug_init(void);
void debug_put(const char *str);
void debug_put_hex(uint64_t val);
