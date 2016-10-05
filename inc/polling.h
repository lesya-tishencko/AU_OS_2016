#ifndef __POLLING_H__
#define __POLLING_H__

#include <ioport.h>

static inline void config_serial() {
   unsigned short s_port = 0x3f8;
   // off interrupt
   out8(s_port + 1, 0);
   
   // set DIV
   out8(s_port + 3, 128); 
   out8(s_port + 0, 3);
   out8(s_port + 1, 0); 

   // set frame
   out8(s_port + 3, 3); 
}

void write_to_serial(const char* data);

#endif
