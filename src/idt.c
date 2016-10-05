#include "idt.h"

/* Controller section */

static void config(unsigned offset)
{
	out8(0x20, 1 | (1 << 4));
	out8(0xA0, 1 | (1 << 4));

	out8(0x21, 32);
	out8(0xA1, 40);

	out8(0x21, (1 << 2));
	out8(0xA1, 2);

	out8(0x21, 1);
	out8(0xA1, 1);
}

static void hide(unsigned interrupt)
{
	unsigned value = (0xFFFF | 1) << interrupt;
	if (interrupt < 8)
		out8(0x21, value & 0xFF);
	else
		out8(0xA1, (value >> 8) & 0xFF);
}

static void set(unsigned interrupt)
{
	unsigned value = 0xFFFF & ~(1 << interrupt);
	if (interrupt < 8)
		out8(0x21, value & 0xFF);
	else
		out8(0xA1, (value >> 8) & 0xFF);
}

static void pic_eoi(unsigned interrupt)
{
	if (interrupt >= 8) {
		out8(0xA0, 1 << 5);
		out8(0x20, 1 << 5);
	} else {
		out8(0x20, 1 << 5);
	}
}


/* End of controller section */

struct idt_ptr {
	uint16_t size;
	uint64_t base;
} __attribute__((packed));

struct idt_entry {
	uint16_t offset1;
	uint16_t ss;
	uint16_t flags;
        uint32_t offset2;
} __attribute__((packed));

// from entry.S
extern uint64_t entry_table[];

static struct idt_entry descryptor_table[35];
static struct idt_ptr idt_ptr;

static void set_idt_entry(int int_no, unsigned short cs, unsigned long offset,  unsigned long flags) {
	(descryptor_table + int_no)->offset1 = offset & 0xFFFFul;
	(descryptor_table + int_no)->ss = cs;
	(descryptor_table + int_no)->flags = flags;
	(descryptor_table + int_no)->offset2 = offset & 0xFFFF0000ul;
}

static void set_handler_addr(int no)
{
	set_idt_entry(no, KERNEL_CS, *(entry_table + no), (0xEul << 8) | (1ul << 15));
}

static void set_idt(const struct idt_ptr* ptr) 
{ __asm__ volatile ("lidt (%0)" : : "a"(ptr)); }

// common handler, is calling from entry.S
void c_handler(struct frame* frame) {
	int int_no = frame->int_no;

	if (int_no < 32) {
		write_to_serial("Error code is: ");
		write_to_serial("\n"); 
	}
	else if (int_no == 32) {
		write_to_serial( "i8254 PIT interrupt handler called\n" );
		pic_eoi(0);
	}
	else {
		write_to_serial("Has been got intterupt from device\n");
		pic_eoi(int_no - 32);
	}
}

void setup_ints(void)
{
	for (int i = 0; i < 35; ++i)
		set_handler_addr(i);

	idt_ptr.size = sizeof(descryptor_table) - 1;
	idt_ptr.base = (uint64_t)descryptor_table;

	set_idt(&idt_ptr);

	config(32);

	for (int i = 32; i < 35; ++i)
		set(i - 32);
}
