#ifndef __MIME_H__
#define __MIME_H__

#define TABLE_SIZE 16

typedef struct {
  char *ext;
  char *mime_type;
} MimeEntry;

extern "C" {
  MimeEntry mime_table[TABLE_SIZE];
  char* mime_type_get(char *filename);
  char* strlower(char *s);
  unsigned int hash(char *ext);
}

#endif