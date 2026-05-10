#include "lexer.h"
#include <stdio.h>
#include "./test.h"

void test_lexer()
{
  char *src = "foo bar \nbaz";
  Lexer *lexer = init_lexer(src);
  while(lexer->head < lexer->len)
  {
    skip_whitespace(lexer);
    printf("Char: %c\n", peek(lexer));
    lexer->head++;
  }
}

void test_tokenizer()
{
  char *src = "foo 1234 == FIELD \n (){}";
  Lexer *lexer = init_lexer(src);

  dump_tokenstream(tokenize(lexer));
}
