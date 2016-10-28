static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <desc.h>
#include "idt.h"
#include "pit.h"
#include "polling.h"
#include "memory_map.h"
#include "buddy.h"

void main(void)
{

	struct desc_table_ptr ptr = {0, 0};

	write_idtr(&ptr);

	config_serial();
	printf("OS has been started:\n");

	setup_memory_map();
	print_memory_map();

	qemu_gdb_hang();

	create_buddy_list();


	uint64_t a = allocate_buddy(1024);
	uint64_t b = allocate_buddy(258);
	uint64_t c = allocate_buddy(32);
	uint64_t d = allocate_buddy(68);
	uint64_t e = allocate_buddy(4096);
	
	free_buddy(a, 10);
	free_buddy(b, 9);
	free_buddy(c, 5);
	free_buddy(d, 7);
	free_buddy(e, 12);

	/*setup_ints();
	enable_ints();


	set_timer(1000);

	__asm__ volatile("int $0" : : );
        __asm__ volatile("int $33" : : );*/


	while (1);
}
