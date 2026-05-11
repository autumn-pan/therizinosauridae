#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "./test.h"

void test_parser()
{
  Lexer *lexer = init_lexer_from_file("config1.thz");  
  TokenStream *ts = tokenize(lexer);
}