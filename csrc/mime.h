#ifndef _MIME_H_
#define _MIME_H_

typedef struct {
  char *ext, *mime_type;
} MimeEntry;

#define TABLE_SIZE 16

extern MimeEntry mime_table[];

extern "C" {
  char* mime_type_get(char *filename);
  char* strlower(char *s);
  unsigned int hash(char *ext);
  char* ensure_mime_table_initialized();
}

#endif