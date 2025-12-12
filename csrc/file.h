#ifndef __FILE__H__
#define __FILE__H__

#include <stdlib.h>

typedef struct file_data {
  size_t size;
  void* data;
} file_data;

extern "C" {
  file_data* file_load(char* filename);
  void file_free(file_data* file);
}

#endif