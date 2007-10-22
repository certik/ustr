#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR1(\2, "14");
  const char *ptr = NULL;
  
  if (ustr_replace(&s1, USTR1(\1, "4"), USTR1(\2, "21"), 0) != 1)
    return 1;
  ptr = ustr_cstr(s1);
  if (ptr[0] != '1' || ptr[1] != '2' || ptr[2] != '1')
    return 1;
  
  return 0;
}
