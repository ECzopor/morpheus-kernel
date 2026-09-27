#pragma once
#include "apic.h"
#include <stdint.h>

void timer_handler();
void sleep(uint64_t time);
