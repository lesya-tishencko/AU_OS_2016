#include <thread.h>
#include <alloc.h>

void handler(struct thread_t *thread, fptr fun, void *args)
{
     fun(args);
     thread->state = FINISHED;
}


void new_thread(struct thread_t *thread, fptr fun, void *args) {
    thread->thread_frame = *(struct frame *)mem_alloc(STACK_SIZE);
    thread->thread_frame.rdi = 0;
    thread->thread_frame.rsi = 0;
    thread->thread_frame.rbp = 0;
    thread->thread_frame.rdx = 0;
    thread->thread_frame.rcx = 0;
    thread->thread_frame.rax = 0;
    thread->thread_frame.rflags = 0;
    thread->state = READY;
    /*thread->handler = handler;
    thread->fun = fun;
    thread->args = args;*/
    fun(args);
}

/*void exit_thread(struct thread_t *thread) {
    while(thread->state != FINISHED);
    mem_free(thread->frame);
}*/

void join_thread(struct thread_t *thread) {
    lock(&thread->state_lock);
    while(thread->state != FINISHED);
    unlock(&thread->state_lock);
    mem_free(&thread->thread_frame);
}


