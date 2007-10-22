#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR("");

  if (!ustr_sc_ensure_owner(&s1))
    return 1;
  
  return 0;
}
