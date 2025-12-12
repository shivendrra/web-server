#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hashtable.h"
#include "llist.h"

#define DEFAULT_SIZE 128
#define DEFAULT_GROW_FACTOR 2

void add_entry_count(hashtable* ht, int d) {
  ht->num_entries += d;
  ht->load = (float)ht->num_entries / ht->size;
}

int default_hashf(void* data, int data_size, int bucket_size) {
  const int R = 31;
  int h = 0;
  unsigned char* p = (unsigned char*)data;
  for (int i = 0; i < data_size; i++) {
    h = (R * h + p[i]) % bucket_size;
  }
  return h;
}

hashtable* hashtable_create(int size, int (*hashf)(void*, int, int)) {
  if (size < 1) size = DEFAULT_SIZE;
  if (hashf == NULL) hashf = default_hashf;
  hashtable* ht = (hashtable*)malloc(sizeof(hashtable));
  if (ht == NULL) return NULL;
  ht->size = size;
  ht->num_entries = 0;
  ht->load = 0;
  ht->bucket = (llist**)malloc(size * sizeof(llist*));
  ht->hashf = (int (*)(void*, int, size_t))hashf;
  for (int i = 0; i < size; i++) ht->bucket[i] = llist_create();
  return ht;
}

void htent_free(void* v, void* args) {
  (void)args;
  htent* e = (htent*)v;
  free(e->key);
  free(e);
}

void hashtable_destroy(hashtable *ht) {
  for (int i = 0; i < ht->size; i++) {
    llist *list = ht->bucket[i];
    llist_foreach(list, htent_free, NULL);
    llist_destroy(list);
  }
  free(ht->bucket);
  free(ht);
}

void* hashtable_put(hashtable* ht, char* key, void* data) {
  return hashtable_put_bin(ht, key, strlen(key), data);
}

void* hashtable_put_bin(hashtable *ht, void *key, size_t key_size, void *data) {
  int index = ht->hashf(key, key_size, ht->size);
  llist* list = ht->bucket[index];
  htent* ent = (htent*)malloc(sizeof(htent));
  ent->key = malloc(key_size);
  memcpy(ent->key, key, key_size);
  ent->key_size = key_size;
  ent->hashed_key = index;
  ent->data = data;
  if (llist_append(list, ent) == NULL) {
    free(ent->key);
    free(ent);
    return NULL;
  }
  add_entry_count(ht, +1);
  return data;
}

int htcmp(void *a, void *b) {
  htent *entA = (htent*)a, *entB = (htent*)b;
  int size_diff = entB->key_size - entA->key_size;
  if (size_diff) return size_diff;
  return memcmp(entA->key, entB->key, entA->key_size);
}

void* hashtable_get(hashtable *ht, char *key) {
  return hashtable_get_bin(ht, key, strlen(key));
}

void* hashtable_get_bin(hashtable *ht, void *key, int key_size) {
  int index = ht->hashf(key, key_size, ht->size);
  llist *list = ht->bucket[index];
  htent cmpent;
  cmpent.key = key;
  cmpent.key_size = key_size;
  htent *n = (htent*)llist_find(list, &cmpent, htcmp);
  if (n == NULL) return NULL;
  return n->data;
}

void* hashtable_delete(hashtable *ht, char *key) {
  return hashtable_delete_bin(ht, key, strlen(key));
}

void* hashtable_delete_bin(struct hashtable *ht, void *key, int key_size) {
  int index = ht->hashf(key, key_size, ht->size);
  llist *list = ht->bucket[index];
  htent cmpent;
  cmpent.key = key;
  cmpent.key_size = key_size;
  htent *ent = (htent*)llist_delete(list, &cmpent, htcmp);
  if (ent == NULL) return NULL;
  void *data = ent->data;
  free(ent->key);
  free(ent);
  add_entry_count(ht, -1);
  return data;
}

void foreach_callback(void *vent, void *vpayload) {
  htent *ent = (htent*)vent;
  foreach_callback_payload *payload = (foreach_callback_payload*)vpayload;
  payload->f(ent->data, payload->arg);
}

void hashtable_foreach(hashtable *ht, void (*f)(void*, void*), void *arg) {
  foreach_callback_payload payload;
  payload.f = f;
  payload.arg = arg;
  for (int i = 0; i < ht->size; i++) {
    llist *list = ht->bucket[i];
    llist_foreach(list, foreach_callback, &payload);
  }
}