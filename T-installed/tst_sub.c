#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR("");

  if (!ustr_sub(&s1, 1, s1))
    return 1;
  
  return 0;
}
