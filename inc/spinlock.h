#ifndef _SPINLOCK_H
#define _SPINLOCK_H

#include <stdatomic.h>

#define LOCKED 1
#define UNLOCKED 0

struct spinlock {
    atomic_uint locked;
};

void lock(struct spinlock *lock);
void unlock(struct spinlock *lock);

#endif
