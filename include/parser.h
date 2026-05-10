#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#include <stddef.h>

typedef struct {
  Token **tokens;
  size_t len;
  size_t head;
} Parser;

#endif // PARSER_H


