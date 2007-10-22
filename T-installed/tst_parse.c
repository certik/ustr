#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR1(\1, "4");

  if (ustr_parse_uint(s1, 0, 0, NULL, NULL) != 4)
    return 1;
  
  return 0;
}
