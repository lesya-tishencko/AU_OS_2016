#include <buddy.h>
#include <list.h>
#include <memory_map.h>

#define NODE_UNUSED 0
#define NODE_USED 1
#define MAX_ORDER 52
	
typedef struct buddy_node buddy_node_t;

struct buddy_list {
	buddy_node_t *head;
	buddy_node_t *last;
};

typedef struct buddy_list buddy_list_t;

buddy_list_t allocator[MAX_ORDER];
int created[MAX_ORDER];
int buddy_count = 0;

int get_order(uint64_t size) {
	int ord = 0;
	uint64_t pow = 1;
	while (pow < size) {
		ord++;
		pow <<= 1;
	}

	return ord;
}

void create_buddy_list() {
	for (uint64_t i = 0; i < memory_map_size; ++i) {
		multiboot_map_entry_t * entry = &memory_map[i];
		if (entry->type != 1)
			continue;
		buddy_node_t * current = (buddy_node_t *)(entry->addr + VIRTUAL_BASE);
		int ord = get_order(entry->len);
		if (!created[ord - 1]) {
			allocator[ord - 1].head = current;
			list_init(allocator[ord - 1].head, NODE_UNUSED, ord);
			allocator[ord - 1].last = allocator[ord - 1].head;
			created[ord - 1] = 1;
			buddy_count++;
			continue;
		}
		list_init(current, NODE_UNUSED, ord);
		list_push_back(allocator[ord - 1].last, current);
		allocator[ord - 1].last = current;
		buddy_count++;
	}
	printf("Allocator is created. Firstly, we have %d blocks\n", buddy_count);
}

uint64_t allocate_buddy(uint64_t size) {
	int need_order = get_order(size);
	int i = 0;
	while (!created[i] || (i + 1 < need_order)) {
		++i;
	}
	buddy_node_t * current = allocator[i].head;
	if (list_empty(allocator[i].head))
		created[i] = 0;
	else
		allocator[i].head = allocator[i].head->next;
	list_remove(current);
	while (current->level > need_order) {
		buddy_node_t * new_node = (buddy_node_t *)(current + (1 << (current->level - 1)));
		list_init(new_node, NODE_UNUSED, current->level - 1);
		if (!created[current->level - 2]) {
			allocator[current->level - 2].head = new_node;
			allocator[current->level - 2].last = allocator[current->level - 2].head;
			created[current->level - 2] = 1;
			current->level--;
			continue;
		}
		list_push_back(allocator[current->level - 2].last, new_node);
		allocator[current->level - 2].last = new_node;
		current->level--;
	}
	current->used = NODE_USED;
	printf("Block with %p address %lld size was allocated\n", current, (1 << need_order));  
	return (uint64_t)current;
}

void free_buddy(uint64_t addr, int level) {
	buddy_node_t * current = (buddy_node_t *)addr;
	list_init(current, NODE_USED, level);
	int curr_level = level;
	if (created[curr_level - 1]) {
		while (1) {
			buddy_node_t * pair = allocator[curr_level - 1].head->prev;
			int flag = 0;
			do {
				pair = pair->next;
				if ((buddy_node_t *)(current + (1 << (curr_level))) == pair) {
					flag = 1;
					break;
				}
			} while (!list_empty(pair));
			if (!flag) break;
			if (!pair->used && pair->level == curr_level) {
				if (pair == allocator[curr_level - 1].head)
					created[curr_level - 1] = 0;
				list_remove(pair);
				current->level = curr_level++;
			}
			else
				break;
		}
	}
	current->used = NODE_UNUSED;
	if (!created[curr_level - 1]) {
		allocator[curr_level - 1].head = current;
		allocator[curr_level - 1].last = allocator[curr_level - 1].head;
		created[curr_level - 1] = 1;
	}
	else {
		list_push_back(allocator[curr_level - 1].last, current);
		allocator[curr_level - 1].last = current;
	}
	printf("Block with %p address with size %lld was free\n", current, (1 << level)); 
}


