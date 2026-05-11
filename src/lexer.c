#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define NUM_OPERATORS 10
#define NUM_KEYWORDS 7

const char WHITESPACE_LIST[] = {' ', '\t'};
const char *OPERATOR_LIST[] = {"=", "==", "-", "+", "*", "/", ">", "<", ">=", "<="};
const char OPERATOR_CHARS[] = {'=', '>', '<', '-', '+', '*', '/'};

const char *KEYWORDS[] = {
  "MODULE",
  "FIELD",
  "DEFAULT",
  "MACRO",
  "IF",
  "ELIF",
  "ELSE"
};

Lexer *init_lexer(char *src) 
{
  Lexer *lexer = malloc(sizeof(Lexer *));
  if(!lexer)
  {
    fprintf(stderr, "Error: Failed to allocate memory for lexer");
    return NULL;
  }
  if(!src)
  {
    fprintf(stderr, "Error: Null parameter(s) passed to init_lexer");
    return NULL;
  }

  lexer->src = src;
  lexer->len = strlen(src);
  lexer->head = 0;

  return lexer;
}

Lexer *init_lexer_from_file(char *filename)
{
  FILE *file = fopen(filename, "r");

  if(!file) 
  {
    fprintf(stderr, "Error: Invalid files passed to init_lexer_from_file");
    return NULL;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  uint64_t fsize = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate an additional byte to account for the terminator
  char *src = malloc(fsize + 1);
  fread(src, fsize, 1, file);
  fclose(file);

  return init_lexer(src);
}

char peek(Lexer *lexer) 
{
  // Provides next character in the source without making edits
  if(!lexer)
  {
    fprintf(stderr, "Error: Null parameter(s) passed to peek");
    return '\0';
  }

  // The last character in the source will be at index n-1
  // And this method peeks ahead by one index
  // So we must validate that the head is less than n-1
  if(lexer->head+1 >= lexer->len)
  {
    return '\0';
  }

  return lexer->src[lexer->head + 1];
}

char eat(Lexer *lexer)
{
  // returns the current head position and moves the head forward.
  if(!lexer)
  {
    fprintf(stderr, "Error: Null parameter(s) passed to eat");
    return '\0';
  }

  char c = lexer->src[lexer->head];
  if(lexer->head < lexer->len)
    lexer->head++;
  
  return c;
}

bool is_whitespace(char c)
{
  // Skips consecutive whitespace
  for(int i = 0; i < sizeof(WHITESPACE_LIST)/sizeof(char); i++)
  {
    if(WHITESPACE_LIST[i] == c)
      return true;
  }

  return false;
}

void skip_whitespace(Lexer *lexer) 
{
  while(is_whitespace(lexer->src[lexer->head]))
  {
    eat(lexer);
  }
}

/////////////////////////////////
// Tokenizer
/////////////////////////////////

void append_char(char *str, char c)
{
  int len = strlen(str);
  str[len] = c;
  str[len+1] = '\0';
}

bool is_keyword(char *word)
{
  for(int i = 0; i < NUM_KEYWORDS; i++)
  {
    if(strcmp(KEYWORDS[i], word) == 0)
      return true;
  }
  return false;
}

char *tokenize_identifier(Lexer *lexer)
{ 
  char *iden = malloc(128);

  if(!isalpha(lexer->src[lexer->head]))
    return NULL;

  while(isalnum(lexer->src[lexer->head]))
    append_char(iden, eat(lexer));
  
  return iden;
}

char *tokenize_int(Lexer *lexer)
{
  char *iden = malloc(128);

  while(isdigit(lexer->src[lexer->head]))
    append_char(iden, eat(lexer));

  if(iden[0] == '\0')
    return NULL;
  
  return iden;
}

bool is_operator_char(char c)
{
  for(int i = 0; i < sizeof(OPERATOR_CHARS)/sizeof(char); i++)
  {
    if(c == OPERATOR_CHARS[i])
      return true;
  }

  return false;
}

char *tokenize_operator(Lexer *lexer)
{
  char *iden = malloc(128);

  while(is_operator_char(lexer->src[lexer->head]))
    append_char(iden, eat(lexer));

  for(int i = 0; i < NUM_OPERATORS; i++)
    if(strcmp(OPERATOR_LIST[i], iden) == 0)
      return iden;


  return NULL;
}

Token *init_token(char *val, TOKENTYPE type)
{
  Token *token = malloc(sizeof(Token));
  if(!token)
  {
    fprintf(stderr, "Error: Failed to allocate memory for Token!");
    return NULL;
  }

  token->val = val;
  token->type = type;

  return token;
}

TokenStream *init_ts()
{
  TokenStream *ts = malloc(sizeof(TokenStream));
  if(!ts)
  {
    fprintf(stderr, "Error: Failed to allocate memory for TokenStream!");
    return NULL;
  }

  ts->num_tokens = 0;
  ts->list = NULL;
  
  return ts;
}

void append_token(TokenStream* ts, Token *token)
{
  Token **tmp;

  if(ts->list)
    tmp = realloc(ts->list, (ts->num_tokens + 1)*sizeof(Token*));
  else
    tmp = malloc((ts->num_tokens + 1)*sizeof(Token*));

  if(!tmp)
  {
    fprintf(stderr, "Error: Failed to allocate memory to append token!");
  }

  ts->list = tmp;
  ts->list[ts->num_tokens] = token;
  ts->num_tokens++;
}

TokenStream *tokenize(Lexer *lexer)
{
  TokenStream *ts = init_ts();
  while(lexer->head < lexer->len)
  {
    // Parse primary tokens
    skip_whitespace(lexer);
    char *token = tokenize_identifier(lexer);
    if(token) {
      if(is_keyword(token))
        append_token(ts, init_token(token, KEYWORD));
      else
        append_token(ts, init_token(token, IDENTIFIER));
      continue;
    }

    token = tokenize_int(lexer);
    if(token) {append_token(ts, init_token(token, INT)); continue;}

    token = tokenize_operator(lexer);
    if(token) {append_token(ts, init_token(token, BINOP)); continue;}

    // Parse misc tokens
    switch(lexer->src[lexer->head]) 
    {
      case '\n':  append_token(ts, init_token("\n", EOL)); 
                  eat(lexer); 
                  break;
      case '{': append_token(ts, init_token("{", LEFT_BRACE));
                eat(lexer); 
                break;
      case '}': append_token(ts, init_token("}", RIGHT_BRACE));
                eat(lexer); 
                break;
      case '(': append_token(ts, init_token("(", LEFT_PAREN));
                eat(lexer); 
                break;
      case ')': append_token(ts, init_token(")", RIGHT_PAREN));                 
                eat(lexer); 
                break;
      default: fprintf(stderr, "Error: Unrecognized token: %c\n", 
                       lexer->src[lexer->head]);
                exit(EXIT_FAILURE);

    }
  }

  return ts;
}

void dump_tokenstream(TokenStream *ts)
{
  for(int i = 0; i < ts->num_tokens; i++)
  {
    printf("Token %i: %s\n", i, ts->list[i]->val);
  }
}