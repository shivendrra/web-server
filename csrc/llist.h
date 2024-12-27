#ifndef LLIST_H
#define LLIST_H

typedef struct llist {
	struct llist_node *head;
	int count;
} llist;

typedef struct llist_node {
  void* data;
  llist_node *next;
} llist_node;

extern llist *llist_create(void);
extern void llist_destroy(llist *llist);
extern void *llist_insert(llist *llist, void *data);
extern void *llist_append(llist *llist, void *data);
extern void *llist_head(llist *llist);
extern void *llist_tail(llist *llist);
extern void *llist_find(llist *llist, void *data, int (*cmpfn)(void *, void *));
extern void *llist_delete(llist *llist, void *data, int (*cmpfn)(void *, void *));
extern int llist_count(llist *llist);
extern void llist_foreach(llist *llist, void (*f)(void *, void *), void *arg);
extern void **llist_array_get(llist *llist);
extern void llist_array_free(void **a);

#endif
