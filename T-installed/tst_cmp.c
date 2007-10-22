#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR("");

  if (!ustr_cmp_eq(s1, s1))
    return 1;
  
  return 0;
}
