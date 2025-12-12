#ifndef __LLIST_H__
#define __LLIST_H__

typedef struct llist {
	struct llist_node *head;
	int count;
} llist;

typedef struct llist_node {
  void* data;
  llist_node *next;
} llist_node;

extern "C" {
  llist *llist_create(void);
  void llist_destroy(llist *llist);
  void *llist_insert(llist *llist, void *data);
  void *llist_append(llist *llist, void *data);
  void *llist_head(llist *llist);
  void *llist_tail(llist *llist);
  void *llist_find(llist *llist, void *data, int (*cmpfn)(void *, void *));
  void *llist_delete(llist *llist, void *data, int (*cmpfn)(void *, void *));
  int llist_count(llist *llist);
  void llist_foreach(llist *llist, void (*f)(void *, void *), void *arg);
  void **llist_array_get(llist *llist);
  void llist_array_free(void **a);
}

#endif