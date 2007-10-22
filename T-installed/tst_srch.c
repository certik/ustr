#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR1(\4, "1234");

  if (ustr_srch_fwd(s1, 0, USTR1(\2, "23")) != 2)
    return 1;
  
  return 0;
}
