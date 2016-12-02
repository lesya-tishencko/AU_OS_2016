#ifndef _THREAD_H
#define _THREAD_H

#include <list.h>
#include <desc.h>
#include <list.h>
#include <spinlock.h>

#define STACK_SIZE  (6 * sizeof(uint64_t))
#define GLOBAL_SIZE (6 * sizeof(uint64_t))

struct st_frame {
   uint64_t rdi;
   uint64_t rsi;
   uint64_t rbp;
   uint64_t rcx;
   uint64_t rbx;
} __attribute__((packed));

typedef void (*fptr)(void *);

enum state {
    READY,
    RUNNING,
    STOPPED,
    FINISHED
};

struct thread_t {
    struct st_frame *thread_frame;
    struct list_head node;
    void (*handler)(struct thread_t*, fptr, void *args);
    fptr fun;
    void *args;
    enum state state;
};

void new_thread(struct thread_t *thread, fptr fun, void *args);
void exit_thread();
void join_thread(struct thread_t *thread);


void scheduler(void);
void scheduler_setup(void);

#endif
