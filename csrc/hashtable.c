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

}