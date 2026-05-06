#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const char WHITESPACE_LIST[] = {' ', '\t', '\n'};

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