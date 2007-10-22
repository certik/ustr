#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR("");

  if (!ustr_ins(&s1, 0, s1))
    return 1;
  
  return 0;
}
