#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef enum {
  AST_PROJECT_ROOT,
  AST_BLOCK,
  AST_FIELD,
  AST_OPERATOR,
  AST_VAR_DECL,
  AST_IDENTIFIER,
  AST_MACRO,
  AST_MODULE
} NodeType;

typedef struct node {
  struct node **children;
  size_t num_children;
  char *value;
  NodeType type;
} ASTNode;

ASTNode *init_ast(char *value, NodeType type);
void ast_append(ASTNode *parent, ASTNode *child);

#endif // AST_H