#ifndef _IDT_H
#define _IDT_H

#include "stdio.h"
#include "ioport.h"
#include <stdint.h>
#include "memory.h"
#include "polling.h"

struct frame {
   uint64_t rdi;
   uint64_t rsi;
   uint64_t rbp;
   uint64_t rdx;
   uint64_t rcx;
   uint64_t rbx;
   uint64_t rax;
   uint64_t int_no;
   uint64_t error;
} __attribute__((packed));

void setup_ints(void);

#endif
