#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <stdint.h>

void create_buddy_list();
uint64_t allocate_buddy(uint64_t);
void free_buddy(uint64_t, int);

#endif /*__BUDDY_H__*/
