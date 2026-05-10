#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef enum {
  PROJECT_ROOT,
  BLOCK,
  FIELD,
  OPERATOR,
  VAR_DECL,
  IDENTIFIER,
  MACRO
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