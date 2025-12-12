#ifndef __CACHE_H__
#define __CACHE_H__

#include <stdlib.h>

typedef struct cache_entry {
  char *path, *content_type;
  size_t content_length;
  void* content;
  struct cache_entry *prev, *next;
} cache_entry;

typedef struct cache {
  struct hashtable *index;
  cache_entry *head, *tail;
  size_t max_size;
  size_t cur_size;
} cache;

extern "C" {
  cache* cache_create(size_t max_size, size_t hashsize);
  
  cache_entry* alloc_entry(char* path, char* content_type, void* content, int content_length);
  void free_entry(cache_entry* entry);
  void free_cache(cache* cache);
  void cache_put(cache* cache, char* path, char* content_type, void* content, size_t content_length);
  cache_entry* cache_get(cache* cache, char* path);
}

#endif