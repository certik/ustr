#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR1(\4, "1234");

  if (ustr_srch_fwd(s1, 0, USTR1(\2, "23")) != 2)
    return 1;

  /* didn't include a Versioned symbol in 1.0.1 */
  if (ustr_srch_rep_chr_fwd(s1, 0, '3', 1) != 3)
    return 1;
  if (ustr_srch_rep_chr_fwd(s1, 0, '3', 2) != 0)
    return 1;
  
  return 0;
}
