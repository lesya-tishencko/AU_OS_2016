#include <list.h>

void list_init(struct buddy_node *head, int used, int level) {
	head->next = head->prev = 0x0;
	head->level = level;
	head->used = used;
}

void list_push_front(struct buddy_node *head, struct buddy_node *new_node) {
	head->prev->next = new_node;
	new_node->prev = head->prev;
	head->prev = new_node;
	new_node->next = head;
}

void list_push_back(struct buddy_node *head, struct buddy_node *new_node) {
	head->next->prev = new_node;
	new_node->next = head->next;
	head->next = new_node;
	new_node->prev = head;
}

void list_remove(struct buddy_node *node) {
	node->prev->next = node->next;
	node->next->prev = node->prev;
}

int list_empty(const struct buddy_node *head) { 
	return head->next == 0x0; 
}
