#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR1(\3, "a|b");
  size_t off = 0;
  
  if (!ustr_split(s1, &off, USTR1(\1, "|"), NULL, 0))
    return 1;
  
  return 0;
}
