#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR("");

  if (!ustr_add_b_uint32(&s1, 4))
    return 1;
  
  return 0;
}
