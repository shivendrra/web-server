#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "file.h"

file_data* file_load(char* filename) {
  char *buffer, *p;
  struct stat buf;
  int bytes_read, bytes_remaining, total_bytes = 0;
  if (stat(filename, &buf) == -1) return NULL;
  if (!(buf.st_mode & S_IFREG)) return NULL;
  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) return NULL;
  bytes_remaining = buf.st_size;
  p = buffer = (char*)malloc(bytes_remaining);
  if (buffer == NULL) {
    fclose(fp);
    return NULL;
  }
  while (bytes_read = fread(p, 1, bytes_remaining, fp), bytes_read != 0 && bytes_remaining > 0) {
    if (bytes_read == -1) {
      free(buffer);
      fclose(fp);
      return NULL;
    }
    bytes_remaining -= bytes_read;
    p += bytes_read;
    total_bytes += bytes_read;
  }
  fclose(fp);
  file_data* filedata = (file_data*)malloc(sizeof(file_data));
  if (filedata == NULL) {
    free(buffer);
    return NULL;
  }
  filedata->size = total_bytes;
  filedata->data = buffer;
  return filedata;
}

void file_free(file_data* filedata) {
  free(filedata->data);
  free(filedata);
}