#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdlib.h>

typedef struct hashtable {
  size_t size, num_entries;
  float load;
  struct llist **bucket;
  int (*hashf)(void* data, int data_size, size_t bucket_count);
} hashtable;

typedef struct htent {
  void *key, *data;
  size_t key_size;
  int hashed_key;
} htent;

typedef struct foreach_callback_payload {
	void* arg;
	void (*f)(void*, void*);
} foreach_callback_payload;

extern "C" {
  hashtable* hashtable_create(size_t size, int (*hashf)(void*, int, int));
  void hashable_destroy(hashtable* ht);
  void* hashtable_put(hashtable* ht, char* key, void* data);
  void* hashtable_put_bin(hashtable* ht, void* key, size_t key_size, void* data);
  void* hashtable_get(hashtable* ht, char* key);
  void* hashtable_get_bin(hashtable* ht, void* key, size_t key_size);
  void* hashtable_delete(hashtable* ht, char* key);
  void* hashtable_delete_bin(hashtable* ht, char* key, size_t key_size);
  void hashtable_foreach(hashtable* ht, void (*f)(void*, void*), void* arg);
}

#endif