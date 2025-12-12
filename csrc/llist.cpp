#include <stdlib.h>
#include "llist.h"

llist* llist_create(void) {
  llist* list = (llist*)calloc(1, sizeof(llist));
  return list;
}

void llist_destroy(llist* list) {
  llist_node* current = list->head;
  while (current != NULL) {
    llist_node* next = current->next;
    free(current);
    current = next;
  }
  free(list);
}

void* llist_insert(llist* list, void* data) {
  llist_node* node = (llist_node*)malloc(sizeof(llist_node));
  if (node == NULL) return NULL;
  node->data = data;
  node->next = list->head;
  list->head = node;
  list->count++;
  return data;
}

void* llist_append(llist* list, void* data) {
  llist_node* node = (llist_node*)malloc(sizeof(llist_node));
  if (node == NULL) return NULL;
  node->data = data;
  node->next = NULL;
  if (list->head == NULL) {
    list->head = node;
  } else {
    llist_node* current = list->head;
    while (current->next != NULL) current = current->next;
    current->next = node;
  }
  list->count++;
  return data;
}

void* llist_head(llist* list) {
  if (list->head == NULL) return NULL;
  return list->head->data;
}

void* llist_tail(llist* list) {
  if (list->head == NULL) return NULL;
  llist_node* current = list->head;
  while (current->next != NULL) current = current->next;
  return current->data;
}

void* llist_find(llist* list, void* data, int (*cmpfn)(void*, void*)) {
  llist_node* current = list->head;
  while (current != NULL) {
    if (cmpfn(current->data, data) == 0) return current->data;
    current = current->next;
  }
  return NULL;
}

void* llist_delete(llist* list, void* data, int (*cmpfn)(void*, void*)) {
  llist_node *current = list->head, *prev = NULL;
  while (current != NULL) {
    if (cmpfn(current->data, data) == 0) {
      if (prev == NULL) list->head = current->next;
      else prev->next = current->next;
      void* result = current->data;
      free(current);
      list->count--;
      return result;
    }
    prev = current;
    current = current->next;
  }
  return NULL;
}

int llist_count(llist* list) {
  return list->count;
}

void llist_foreach(llist* list, void (*f)(void*, void*), void* arg) {
  llist_node* current = list->head;
  while (current != NULL) {
    f(current->data, arg);
    current = current->next;
  }
}

void** llist_array_get(llist* list) {
  if (list->count == 0) return NULL;
  void** array = (void**)malloc(list->count * sizeof(void*));
  if (array == NULL) return NULL;
  llist_node* current = list->head;
  int i = 0;
  while (current != NULL) {
    array[i++] = current->data;
    current = current->next;
  }
  return array;
}

void llist_array_free(void** array) {
  free(array);
}