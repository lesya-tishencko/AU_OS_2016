#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

#include "stdio.h"
#include <stdint.h>
#include "memory.h"

#define MEMORY_MAP_MAX_SIZE 128

struct multiboot_map_entry {
       uint32_t size;
       uint64_t addr;
       uint64_t len;
       uint32_t type;
} __attribute__((packed));

typedef struct multiboot_map_entry multiboot_map_entry_t;

extern size_t memory_map_size;
extern multiboot_map_entry_t memory_map[MEMORY_MAP_MAX_SIZE];

struct multiboot_header {
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;
	uint32_t header_addr;
	uint32_t load_addr;
	uint32_t load_end_addr;
	uint32_t bss_end_addr;
	uint32_t entry_addr;
} __attribute__((packed));

typedef struct multiboot_header multiboot_header_t;

struct multiboot_info {
	uint32_t flags;
	uint32_t mem_lower;
        uint32_t mem_upper;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
        uint32_t mods_addr;
	uint64_t sym1;
	uint64_t sym2;
	uint32_t mmap_length;
	uint32_t mmap_addr;
	uint64_t drivers;
	uint32_t config_table;
    	uint32_t boot_loader_name;
	uint32_t apm_table;
	uint64_t vbe1;
	uint64_t vbe2;
} __attribute__((packed));

typedef struct multiboot_info multiboot_info_t;

void setup_memory_map();
void print_memory_map();

#endif /*__MEMORY_MAP_H__*/
