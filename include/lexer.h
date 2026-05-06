#include <stdint.h>

#define LEXER_BUFFER_SIZE 4096

typedef struct {
  char *src;
  uint64_t len;
  uint64_t head;
} Lexer;

Lexer *init_lexer(char *src);
Lexer *init_lexer_from_file(char *filename);
char peek(Lexer *lexer);
char eat(Lexer *lexer);
void skip_whitespace(Lexer *lexer);
