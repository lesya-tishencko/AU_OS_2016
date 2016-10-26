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

void main(void)
{
	qemu_gdb_hang();


	struct desc_table_ptr ptr = {0, 0};

	write_idtr(&ptr);

	config_serial();
	write_to_serial("OS has been started:\n");

	setup_ints();
	enable_ints();


	set_timer(1000);

	__asm__ volatile("int $0" : : );
        __asm__ volatile("int $33" : : );


	while (1);
}
