#include <stdlib.h>
#include "mime.h"

MimeEntry mime_table[TABLE_SIZE] = {
  {(char*)"html", (char*)"text/html"},
  {(char*)"htm", (char*)"text/html"},
  {(char*)"jpeg", (char*)"image/jpg"},
  {(char*)"jpg", (char*)"image/jpg"},
  {(char*)"css", (char*)"text/css"},
  {(char*)"js", (char*)"application/javascript"},
  {(char*)"json", (char*)"application/json"},
  {(char*)"txt", (char*)"text/plain"},
  {(char*)"gif", (char*)"image/gif"},
  {(char*)"png", (char*)"image/png"},
  {NULL, NULL}
};