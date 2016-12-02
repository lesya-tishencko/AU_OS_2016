#include <thread.h>
#include <alloc.h>
#include <ints.h>

static struct thread_t *current;
extern void thread_frame_switch(struct st_frame **prev, struct st_frame *next);
static struct list_head threads_list;
struct spinlock mem_lock;

void handler(struct thread_t *thread, fptr fun, void *args)
{
     current = thread;
     thread->state = RUNNING;
     fun(args);
}

void new_thread(struct thread_t *thread, fptr fun, void *args) {
    thread = (struct thread_t *)mem_alloc(GLOBAL_SIZE);
    thread->thread_frame = (struct st_frame *)mem_alloc(STACK_SIZE);
    thread->thread_frame->rdi = 0;
    thread->thread_frame->rsi = 0;
    thread->thread_frame->rbp = 0;
    thread->thread_frame->rcx = 0;
    thread->thread_frame->rbx = 0;
    thread->state = READY;
    thread->handler = handler;
    thread->fun = fun;
    thread->args = args;

    disable_ints();
    list_add_tail(&threads_list, &thread->node);
    enable_ints();
}

void exit_thread() {
    current->state = FINISHED;
    while (1) scheduler();
}

void join_thread(struct thread_t *thread) {
    while(thread->state != FINISHED);
    mem_free(&thread->thread_frame);
    mem_free(thread);
}

void scheduler(void) {
    disable_ints();
    struct thread_t *next;

    if (!list_empty(&threads_list)) {
        
	if (current->state == RUNNING) {
	    list_add_tail(&threads_list, &current->node);
            current->state = STOPPED;
        }

        next = LIST_ENTRY(threads_list.next, struct thread_t, node);
        list_del(threads_list.next);

        thread_frame_switch(&current->thread_frame, next->thread_frame);

        if (next->state == READY)
	    next->handler(next, next->fun, next->args);
        else {
            next->state = RUNNING;
            current = next;
        }
    }
    enable_ints();
}

void scheduler_setup(void) { 
    list_init(&threads_list); 
}
