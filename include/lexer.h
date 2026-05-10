#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>
#include <stddef.h>

#define LEXER_BUFFER_SIZE 4096

typedef enum {
  LEFT_PAREN,  // (
  RIGHT_PAREN, // )
  LEFT_BRACE,  // {
  RIGHT_BRACE, // }

  EOL, // \n

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

typedef struct {
  char *val;
  TOKENTYPE type;
} Token;

typedef struct {
  Token **list;
  size_t num_tokens;
} TokenStream;

Lexer *init_lexer(char *src);
Lexer *init_lexer_from_file(char *filename);
char peek(Lexer *lexer);
char eat(Lexer *lexer);
void skip_whitespace(Lexer *lexer);

char *tokenize_identifier(Lexer *lexer);
char *tokenize_int(Lexer *lexer);
char *tokenize_operator(Lexer *lexer);
char *tokenize_keyword(Lexer *lexer);

TokenStream *tokenize(Lexer *lexer);
void dump_tokenstream(TokenStream *ts);

#endif // LEXER_H