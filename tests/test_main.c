#define TESTING

#ifdef TESTING
#include "./test.h"
#include <stdio.h>

int main()
{
  test_lexer();
  return 0;
}

#endif
