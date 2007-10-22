#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR("");

  if (!ustr_utf8_valid(s1))
    return 1;
  
  return 0;
}
