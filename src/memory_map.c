/* For writing memory map handler I used https://www.gnu.org/software/grub/manual/multiboot/multiboot.html */

#include <memory_map.h>

extern uint64_t multiboot_info;
extern uint32_t multiboot_header[];
 
void setup_memory_map() {
	memory_map_size = 0;
	multiboot_header_t * mheader = (multiboot_header_t *)multiboot_header;
	multiboot_info_t * minfo = (multiboot_info_t *)multiboot_info;
	if (!((minfo->flags >> 6) & 1)) {
            printf("Multiboot info structure flag 6 is not set not memory map is provided!");
	    return;
	}
	
	multiboot_map_entry_t kernel;
	kernel.addr = mheader->load_addr;
	kernel.len = mheader->bss_end_addr - mheader->load_addr;
	kernel.type = 0;
	memory_map[memory_map_size++] = kernel;

	uint32_t kernel_end = mheader->bss_end_addr;
	 
	uint32_t mmap_length = minfo -> mmap_length;
	uint64_t mmap_addr = minfo -> mmap_addr;

	uint64_t pos = 0;
	while(pos < mmap_length) {
		multiboot_map_entry_t entry = *(multiboot_map_entry_t *)(mmap_addr + pos);
		
		uint64_t entry_end = entry.addr + entry.len;
		if (kernel_end < entry.addr || entry_end < kernel.addr) {
			memory_map[memory_map_size++] = entry;
		}
		else if (kernel.addr <= entry.addr && entry_end <= kernel_end) {
		} 
		else if (entry.addr < kernel.addr && kernel_end < entry_end) {
			entry.len = kernel.addr - entry.addr;
            		memory_map[memory_map_size++] = entry;
            		entry.len = entry_end - kernel_end;
            		entry.addr = kernel_end;
			memory_map[memory_map_size++] = entry;
		}
		else if (kernel.addr < entry.addr) {
			entry.addr = kernel_end;
			memory_map[memory_map_size++] = entry;
		}
		else {
			entry.len = entry_end - entry.addr;
			memory_map[memory_map_size++] = entry;
		}

		pos += entry.size + 4;
	}
}

void print_memory_map() {
	printf("Memory map: \n");
	for (size_t i = 0; i < memory_map_size; i++) {
		printf("size %p range [%p; %p] type %lld\n", memory_map[i].size, memory_map[i].addr, memory_map[i].addr + memory_map[i].len - 1, memory_map[i].type);
	}
}




