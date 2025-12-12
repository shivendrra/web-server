#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cache.h"
#include "hashtable.h"

cache_entry* alloc_entry(char* path, char* content_type, void* content, int content_length) {
  cache_entry* entry = (cache_entry*)malloc(sizeof(cache_entry));
  if (entry == NULL) return NULL;
  entry->path = strdup(path);
  entry->content_type = strdup(content_type);
  entry->content_length = content_length;
  entry->content = malloc(content_length);
  if (entry->content == NULL) {
    free(entry->path);
    free(entry->content_type);
    free(entry);
    return NULL;
  }
  memcpy(entry->content, content, content_length);
  entry->prev = entry->next = NULL;
  return entry;
}

void free_entry(cache_entry* entry) {
  if (entry == NULL) return;
  free(entry->path);
  free(entry->content_type);
  free(entry->content);
  free(entry);
}

void dlist_insert_head(cache* cache, cache_entry* ce) {
  if (cache->head == NULL) {
    cache->head = cache->tail = ce;
    ce->prev = ce->next = NULL;
  } else {
    cache->head->prev = ce;
    ce->next = cache->head;
    ce->prev = NULL;
    cache->head = ce;
  }
}

void dlist_move_to_head(cache* cache, cache_entry* ce) {
  if (ce != cache->head) {
    if (ce == cache->tail) {
      cache->tail = ce->prev;
      cache->tail->next = NULL;
    } else {
      ce->prev->next = ce->next;
      ce->next->prev = ce->prev;
    }
    ce->next = cache->head;
    cache->head->prev = ce;
    ce->prev = NULL;
    cache->head = ce;
  }
}

cache_entry* dlist_remove_tail(cache* cache) {
  cache_entry* oldtail = cache->tail;
  cache->tail = oldtail->prev;
  if (cache->tail != NULL) cache->tail->next = NULL;
  else cache->head = NULL;
  cache->cur_size--;
  return oldtail;
}

cache* cache_create(size_t max_size, size_t hashsize) {
  cache* c = (cache*)malloc(sizeof(cache));
  if (c == NULL) return NULL;
  c->index = hashtable_create(hashsize, NULL);
  if (c->index == NULL) {
    free(c);
    return NULL;
  }
  c->head = c->tail = NULL;
  c->max_size = max_size;
  c->cur_size = 0;
  return c;
}

void free_cache(cache* cache) {
  cache_entry* cur_entry = cache->head;
  hashtable_destroy(cache->index);
  while (cur_entry != NULL) {
    cache_entry* next_entry = cur_entry->next;
    free_entry(cur_entry);
    cur_entry = next_entry;
  }
  free(cache);
}

void cache_put(cache* cache, char* path, char* content_type, void* content, size_t content_length) {
  cache_entry* existing = (cache_entry*)hashtable_get(cache->index, path);
  if (existing != NULL) {
    free(existing->content_type);
    free(existing->content);
    existing->content_type = strdup(content_type);
    existing->content_length = content_length;
    existing->content = malloc(content_length);
    memcpy(existing->content, content, content_length);
    dlist_move_to_head(cache, existing);
    return;
  }
  cache_entry* new_entry = alloc_entry(path, content_type, content, content_length);
  if (new_entry == NULL) return;
  if (cache->cur_size >= cache->max_size) {
    cache_entry* removed = dlist_remove_tail(cache);
    hashtable_delete(cache->index, removed->path);
    free_entry(removed);
  }
  dlist_insert_head(cache, new_entry);
  hashtable_put(cache->index, path, new_entry);
  cache->cur_size++;
}

cache_entry* cache_get(cache* cache, char* path) {
  cache_entry* entry = (cache_entry*)hashtable_get(cache->index, path);
  if (entry != NULL) dlist_move_to_head(cache, entry);
  return entry;
}