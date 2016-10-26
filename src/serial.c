#include "polling.h"

void write_to_serial(const char* data) {
   unsigned short s_port = 0x3f8;
   // write string
   while (*data != '\0') {
	while ((in8(s_port + 5) & (1 << 5)) == 0)
	   continue;
        out8(s_port + 0, (uint8_t)(*data));
        data++;
   }
}
