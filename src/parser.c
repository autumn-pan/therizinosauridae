#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "ast.h"

// Valid keywords to start a statement with
// Additionally, all statements must start on a new line
const NodeType STMT_STARTERS[] = {
  AST_FIELD,
  AST_MACRO
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

  return NULL;
}

ASTNode *parse_block(Parser *parser)
{
  if(parser->tokens[parser->head]->type != LEFT_BRACE)
    return NULL;

  parser_eat(parser);
  ASTNode *block = init_ast("block", AST_BLOCK);

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

ASTNode *parse_module(Parser *parser)
{
  if(parser->tokens[parser->head]->type != AST_MODULE)
    return NULL;

  parser_eat(parser);
  Token *iden = parser_eat(parser);

  if(iden->type != IDENTIFIER)
  {
    fprintf(stderr, "Error: Unexpected token %s, expected identifier", iden->val);
    exit(EXIT_FAILURE);
  }

  ASTNode *module = init_ast(iden->val, AST_MODULE);

  ASTNode *block = parse_block(parser);
  if(!block)
  {
    fprintf(stderr, "Error: Unexpected token %s, expected '{'", iden->val);
    exit(EXIT_FAILURE);  
  }

  ast_append(module, block);
  return module;
}

ASTNode *parse(Parser *parser)
{
  ASTNode *project_root = init_ast("root", AST_PROJECT_ROOT);

  skip_newline(parser);
  while(parser->tokens[parser->head]->type != EOL) 
  {
    ASTNode *module = parse_block(parser);
    if(!module)
    {
      char *token = parser->tokens[parser->head]->val;
      fprintf(stderr, "Error: Unexpected token \"%s\", expected \"MODULE\"", token);
      exit(EXIT_FAILURE);
    }

    ast_append(project_root, module);
    skip_newline(parser);
  }

  return project_root;
}