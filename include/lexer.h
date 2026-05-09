#include <stdint.h>

#define LEXER_BUFFER_SIZE 4096

typedef enum {
  LEFT_PAREN,  // (
  RIGHT_PAREN, // )
  LEFT_BRACE,  // {
  RIGHT_BRACE, // }

  BINOP, // Binary Operator
  UNOP,  // Unary Operator

  INT,

  IDENTIFIER,
  KEYWORD

} TOKENTYPE;

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

char *tokenize_identifier(Lexer *lexer);
char *tokenize_int(Lexer *lexer);
char *tokenize_operator(Lexer *lexer);
char *tokenize_keyword(Lexer *lexer);