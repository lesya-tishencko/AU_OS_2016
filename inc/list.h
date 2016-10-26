#ifndef __LIST_H__
#define __LIST_H__


struct buddy_node {
	int level;
	int used;
	struct buddy_node *next;
	struct buddy_node *prev;
};

void list_init(struct buddy_node *head, int used, int level);
void list_push_back(struct buddy_node *head, struct buddy_node *new_node);
void list_pust_front(struct buddy_node *head, struct buddy_node *new_node);
void list_remove(struct buddy_node *node);
int list_empty(const struct buddy_node *head);

#endif /*__LIST_H_*/
