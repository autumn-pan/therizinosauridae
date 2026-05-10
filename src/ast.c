#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

ASTNode *init_ast(char *value, NodeType type)
{
  ASTNode *node = malloc(sizeof(ASTNode));
  if(!node) 
  {
    fprintf(stderr, "Error: Failed to allocate memory for ASTNode!");
    exit(EXIT_FAILURE);
  }

  node->value = value;
  node->type = type;

  return node;
}

void ast_append(ASTNode *parent, ASTNode *child) 
{
  ASTNode **tmp = realloc(parent->children, (parent->num_children+1)*sizeof(ASTNode *));
  if(!tmp) 
  {
    fprintf(stderr, "Error: Failed to allocate memory for ASTNode!");
    exit(EXIT_FAILURE);
  }

  parent->children = tmp;
  parent->children[parent->num_children] = child;
  parent->num_children++;
}
