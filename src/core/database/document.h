#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <stdint.h>

typedef struct {
  uint32_t id;
  char *content;
} Document;

typedef struct {
  Document *items;
  unsigned short with_content;
  unsigned short size;
} DocumentCollection;

#endif