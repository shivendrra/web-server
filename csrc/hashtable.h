#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>

typedef struct hashtable {
  size_t size;
  size_t num_entries;
  float load;
  struct llist **bucket;
  int (*hashf)(void* data, int data_size, size_t bucket_count);
} hashtable;

typedef struct htent {
  void* key;
  size_t key_size;
  int hashed_key;
  void* data;
} htent;

typedef struct foreach_callback_payload {
	void *arg;
	void (*f)(void *, void *);
} foreach_callback_payload;

extern hashtable *hashtable_create(size_t size, int (*hashf)(void *, int, int));
extern void hashtable_destroy(hashtable *ht);
extern void *hashtable_put(hashtable *ht, char *key, void *data);
extern void *hashtable_put_bin(hashtable *ht, void *key, size_t key_size, void *data);
extern void *hashtable_get(hashtable *ht, char *key);
extern void *hashtable_get_bin(hashtable *ht, void *key, size_t key_size);
extern void *hashtable_delete(hashtable *ht, char *key);
extern void *hashtable_delete_bin(hashtable *ht, void *key, size_t key_size);
extern void hashtable_foreach(hashtable *ht, void (*f)(void *, void *), void *arg);

#endif