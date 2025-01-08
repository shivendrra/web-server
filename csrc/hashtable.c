#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hashtable.h"
#include "llist.h"

#define  DEFAULT_SIZE  128
#define  DEFAULT_GROW_FACTOR  2

void add_entry_count(hashtable* ht, int d) {
  ht->num_entries += d;
  ht->load = (float)ht->num_entries / ht->size;
}

int default_hashf(void* data, size_t data_size, size_t bucket_size) {
  const int R = 31; // small prime
  int h = 0;
  unsigned char* p = data;
  for (int i = 0; i < data_size; i++) {
    h = (R * h + p[i]) % bucket_size;
  }
  return h;
}

hastable* hastable_create(int size, int (*hashf)(void *, int, int)) {
  if (size < 1) {
    size = DEFAULT_SIZE;
  }
  if (hashf == NULL) {
    hashf = default_hashf;
  }
  hashtable* ht = malloc(sizeof* ht);
  if (ht == NULL) return NULL;
  ht->size = size;
  ht->num_entries = 0;
  ht->load = 0;
  ht->bucket = malloc(size * sizeof(struct* llist));
  ht->hashf = hashf;
  for (int i = 0; i < size; i++) {
    ht->bucket[i] = llist_create();
  }
  return ht;
}

void htnet_free(void* htnet, void* args) {
  (void)args;
  free(htnet);
}

void hashtable_destroy(hashtable *ht) {
  for (int i = 0; i < ht->size; i++) {
    llist *llist = ht->bucket[i];
	  llist_foreach(llist, htent_free, NULL);
    llist_destroy(llist);
  }
  free(ht);
}

void *hashtable_put(struct hashtable *ht, char *key, void *data) {
  return hashtable_put_bin(ht, key, strlen(key), data);
}

void *hashtable_put_bin(hashtable *ht, void *key, size_t key_size, void *data) {
  int index = ht->hashf(key, key_size, ht->size);
  llist* llist = ht->bucket[index];
  htnet* ent = malloc(sizeof* ent);
  ent->key = malloc(key_size);
  memcpy(ent->key, key, key_size);
  ent->key_size = key_size;
  ent->hashed_key = index;
  ent->data = data;
  if (llist_append(llist, ent) == NULL) {
    free(ent->key);
    free(ent);
    return NULL;
  }

  add_entry_point(ht, +1);
  return data;
}

int htcmp(void *a, void *b) {
  htent *entA = a, *entB = b;
  int size_diff = entB->key_size - entA->key_size;
  if (size_diff) {
    return size_diff;
  }

  return memcmp(entA->key, entB->key, entA->key_size);
}

void *hashtable_get(hashtable *ht, char *key) {
  return hashtable_get_bin(ht, key, strlen(key));
}

void *hashtable_get_bin(hashtable *ht, void *key, int key_size)
{
  int index = ht->hashf(key, key_size, ht->size);
  llist *llist = ht->bucket[index];

  htent cmpent;
  cmpent.key = key;
  cmpent.key_size = key_size;
  htent *n = llist_find(llist, &cmpent, htcmp);

  if (n == NULL) { return NULL; }
  return n->data;
}

void *hashtable_delete(hashtable *ht, char *key) {
  return hashtable_delete_bin(ht, key, strlen(key));
}

void *hashtable_delete_bin(struct hashtable *ht, void *key, int key_size)
{
  int index = ht->hashf(key, key_size, ht->size);
  llist *llist = ht->bucket[index];

  htent cmpent;
  cmpent.key = key;
  cmpent.key_size = key_size;

  htent *ent = llist_delete(llist, &cmpent, htcmp);
	if (ent == NULL) {
		return NULL;
	}
	void *data = ent->data;
  free(ent);
  add_entry_count(ht, -1);
	return data;
}

void foreach_callback(void *vent, void *vpayload) {
  htent *ent = vent;
  foreach_callback_payload *payload = vpayload;

	payload->f(ent->data, payload->arg);
}

void hashtable_foreach(hashtable *ht, void (*f)(void *, void *), void *arg) {
	foreach_callback_payload payload;

	payload.f = f;
	payload.arg = arg;

	for (int i = 0; i < ht->size; i++) {
		llist *llist = ht->bucket[i];
		llist_foreach(llist, foreach_callback, &payload);
	}
}