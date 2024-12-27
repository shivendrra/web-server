#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>

typedef struct cache_entry {
  char* path;
  char* content_type;
  size_t content_length;
  void* content;
} cache_entry;

typedef struct cache {
  struct hashable *index;
  cache_entry *head, *tail;
  size_t max_size;
  size_t cur_size;
} cache;

extern cache_entry* alloc_entry(char* path, char* content_type, void* content, int content_length);
extern void free_entry(cache_entry* entry);
extern cache* cache_create(size_t max_size, size_t hashsize);
extern void free_cache(cache* cache);
extern void cache_put(cache* cache, char* path, char* content_type, void* content, size_t content_length);
extern cache_entry* cache_get(cache* cache, char* path);

#endif