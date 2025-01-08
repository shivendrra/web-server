#include <string.h>
#include <ctype.h>
#include "mime.h"

#define DEFAULT_MIME_TYPE "application/octet-stream"

char *strlower(char *s) {
  for (char *p = s; *p != '\0'; p++) {
    *p = tolower(*p);
  }
  return s;
}

unsigned int hash(char *ext) {
  unsigned int hash = 0;
  while (*ext) {
    hash = (hash * 31 + tolower(*ext)) % TABLE_SIZE;
    ext++;
  }
  return hash;
}

char *mime_type_get(char *filename) {
  char *ext = strrchr(filename, '.');

  if (ext == NULL) {
    return DEFAULT_MIME_TYPE;
  }
  ext++;
  strlower(ext);

  unsigned int index = hash(ext);

  // Linear probing for collision resolution
  for (int i = 0; i < TABLE_SIZE; i++) {
    unsigned int probe_index = (index + i) % TABLE_SIZE;
    if (mime_table[probe_index].ext == NULL) {
      break;  // No match found
    }
    if (strcmp(mime_table[probe_index].ext, ext) == 0) {
      return mime_table[probe_index].mime_type;
    }
  }
  return DEFAULT_MIME_TYPE;
}