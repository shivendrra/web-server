#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mime.h"

MimeEntry mime_table[TABLE_SIZE];

static void init_mime_table() {
  static int initialized = 0;
  if (initialized) return;
  
  for (int i = 0; i < TABLE_SIZE; i++) {
    mime_table[i].ext = NULL;
    mime_table[i].mime_type = NULL;
  }
  
  struct {char *ext; char *mime;} entries[] = {
    {(char*)"html", (char*)"text/html"},
    {(char*)"htm", (char*)"text/html"},
    {(char*)"jpeg", (char*)"image/jpeg"},
    {(char*)"jpg", (char*)"image/jpeg"},
    {(char*)"css", (char*)"text/css"},
    {(char*)"js", (char*)"application/javascript"},
    {(char*)"json", (char*)"application/json"},
    {(char*)"txt", (char*)"text/plain"},
    {(char*)"gif", (char*)"image/gif"},
    {(char*)"png", (char*)"image/png"}
  };
  
  for (unsigned long i = 0; i < sizeof(entries)/sizeof(entries[0]); i++) {
    unsigned int h = 0, index;
    char *ext = entries[i].ext;
    while (*ext) {
      h = (h * 31 + tolower(*ext)) % TABLE_SIZE;
      ext++;
    }
    index = h;
    while (mime_table[index].ext != NULL) index = (index + 1) % TABLE_SIZE;
    mime_table[index].ext = entries[i].ext;
    mime_table[index].mime_type = entries[i].mime;
  }
  initialized = 1;
}

char* ensure_mime_table_initialized() {
  init_mime_table();
  return NULL;
}