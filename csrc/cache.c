#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cache.h"
#include "hashtable.h"

cache_entry* alloc_entry(char* path, char* content_type, void* content, int content_length) {
  ///////////////////////////
  ///////////////////////////
  //// to be implemented ////
  ///////////////////////////
  ///////////////////////////
}

cache_entry* free_cache(cache_entry* entry) {
  free(entry->path);
  free(entry->content_type);
  free(entry->content);
  free(entry->content_length);
  free(entry);
}

cache_entry* free_entry(cache_entry* entry) {
  ///////////////////////////
  ///////////////////////////
  //// to be implemented ////
  ///////////////////////////
  ///////////////////////////
}

void dlist_insert_head(cache* cache, cache_entry* ce) {
  if(cache->head == NULL) {
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
  if(ce != cache->head) {
    if(ce == cache->tail) {
      cache->tail = ce->prev;
      cache->tail->next = NULL;
    } else {
      ce->prev->next = ce->next;
      ce->next->prev = ce->prev;
    }

    ce->next = cache->head;
    cache->head->prev = ce->next;
    ce->prev = NULL;
    cache->head = ce;
  }
}

cache_entry* dlist_remove_tail(cache* cache) {
  cache_entry* oldtail = cache->tail;
  cache->tail = oldtail->prev;
  cache->tail->next = NULL;
  cache->cur_size--;
  return oldtail;
}

cache* cache_create(size_t max_size, size_t hashsize) {
  ///////////////////////////
  ///////////////////////////
  //// to be implemented ////
  ///////////////////////////
  ///////////////////////////
}

void free_cache(cache* cache) {
  cache_entry* cur_entry = cache->head;
  hashable_destroy(cache->index);
  while(cur_entry != NULL) {
    cache_entry* next_entry = cur_entry->next;
    free_entry(cur_entry);
    cur_entry = next_entry;
  }
  free(cache);
}

void cache_put(cache* cache, char* path, char* conent_type, void* content, size_t content_length) {
  ///////////////////////////
  ///////////////////////////
  //// to be implemented ////
  ///////////////////////////
  ///////////////////////////
}

cache_entry* cache_get(cache* cache, char* path) {
  ///////////////////////////
  ///////////////////////////
  //// to be implemented ////
  ///////////////////////////
  ///////////////////////////
}