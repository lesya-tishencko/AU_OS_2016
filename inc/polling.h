#ifndef __POLLING_H__
#define __POLLING_H__

#include <ioport.h>

static inline void write_to_serial(const char* data) {
   unsigned short s_port = 0x3f8;
   // off interrupt
   out8(s_port+1, 0);
   
   // set DIV
   out8(s_port+3, 128); 
   out8(s_port+0, 3);
   out8(s_port+1, 0); 

   // set frame
   out8(s_port+3, 3); 

   // write string
   while (*data != '\0') {
	while ((in8(s_port+5) & (1<<5)) == 0)
	   continue;
        out8(s_port+0, (uint8_t)(*data));
        data++;
   }
}

#endif
