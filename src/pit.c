#include "pit.h"

#define PORT_CMD  0x0043
#define PORT_CH0  0x0040

void set_timer(uint64_t ms)
{
	int count = 1193182ul * ms / 1000ul;
	out8(PORT_CMD, 0x00 | 0x04 | 0x30 | 0x00);
	  
	out8(PORT_CH0, count & 0xFF);
	out8(PORT_CH0, (count >> 8) & 0xFF);
}
