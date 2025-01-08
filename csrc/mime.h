#ifndef _MIME_H_
#define _MIME_H_

typedef struct {
  char *ext;
  char *mime_type;
} MimeEntry;

#define TABLE_SIZE 16  // Fixed-size hash table
MimeEntry mime_table[TABLE_SIZE] = {
  {"html", "text/html"},
  {"htm", "text/html"},
  {"jpeg", "image/jpg"},
  {"jpg", "image/jpg"},
  {"css", "text/css"},
  {"js", "application/javascript"},
  {"json", "application/json"},
  {"txt", "text/plain"},
  {"gif", "image/gif"},
  {"png", "image/png"},
  {NULL, NULL}  // Sentinel value
};

extern char *mime_type_get(char *filename); // Function to get the MIME type based on a file name
char *strlower(char *s); // Utility function to convert a string to lowercase
unsigned int hash(char *ext); // Hash function for file extensions

#endif