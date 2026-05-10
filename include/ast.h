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