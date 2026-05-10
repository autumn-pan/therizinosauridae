#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "ast.h"

// Valid keywords to start a statement with
// Additionally, all statements must start on a new line
const NodeType STMT_STARTERS = {
  FIELD,
  MACRO,
  INT,
};

Parser *init_parser(TokenStream *ts)
{
  Parser *parser = malloc(sizeof(Parser));
  if(!parser)
  {
    fprintf(stderr, "Error: Failed to allocate memory for Parser!");
    exit(EXIT_FAILURE);
  }

  parser->tokens = ts->list;
  parser->len = ts->num_tokens;
  parser->head = 0;

  return parser;
}

Token *parser_peek(Parser *parser)
{
  if(parser->head >= parser->len - 1)
    return NULL;

  return parser->tokens[parser->head + 1];
}

Token *parser_eat(Parser *parser)
{
  if(parser->head < parser->len - 1)
    parser->head++;

  return parser->tokens[parser->head - 1];
}

void skip_newline(Parser *parser)
{
  while(parser->tokens[parser->head]->type == EOL)
    parser_eat(parser);
}

ASTNode *parse_statement(Parser *parser) 
{
  // magic
}

ASTNode *parse_block(Parser *parser)
{
  if(parser->tokens[parser->head]->type != LEFT_BRACE)
    return NULL;

  parser_eat(parser);
  ASTNode *block = init_ast("block", BLOCK);
  
  skip_newline(parser);
  while(parser->tokens[parser->head]->type != LEFT_BRACE)
  {
    ASTNode *stmt = parse_statement(parser);
    if(!stmt)
      exit(EXIT_FAILURE);

    ast_append(block, parse_statement(parser));
    skip_newline(parser);
  }

  parser_eat(parser);
  return block;
}