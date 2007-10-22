#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR("");

  if (!ustr_add_fmt(&s1, "%s", "abcd"))
    return 1;
  
  return 0;
}
