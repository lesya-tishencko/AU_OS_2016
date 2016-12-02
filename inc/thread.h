#ifndef _THREAD_H
#define _THREAD_H

#include <list.h>
#include <desc.h>
#include <spinlock.h>

#define STACK_SIZE  (8 * sizeof(uint64_t))

struct frame {
   uint64_t rdi;
   uint64_t rsi;
   uint64_t rbp;
   uint64_t rdx;
   uint64_t rcx;
   uint64_t rbx;
   uint64_t rax;
   uint64_t rflags;
} __attribute__((packed));

typedef void (*fptr)(void *);

enum state {
    READY,
    RUNNING,
    FINISHED
};

struct thread_t {
    struct frame thread_frame;
    /*void (*handler)(struct thread_t*, fptr, void *args);
    fptr fun;
    void *args;*/
    enum state state;
    struct spinlock state_lock;
};

void new_thread(struct thread_t *thread, fptr fun, void *args);
void join_thread(struct thread_t *thread);

#endif
