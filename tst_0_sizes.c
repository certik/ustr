#include "tst.h"

static const char *rf = __FILE__;

/* this doesn't really tes the SS API, just the allocating sizes */
int tst(void)
{
  static const size_t ans[] = {  4,   6,   8,  12,  16,
                                24,  32,  48,  64,  96,
                               128, 192, 256, 384, 512};
  static const size_t ohoff = 2; /* 1 + x + y + z + 1,
                                    x == ref_bytes, y = len bytes, z = len */
  size_t num = 0;

  /* ref_bytes = 0 */
  ASSERT(ustr__ns(ohoff + 0 + 1 + 1) == ans[0]);
  
  ASSERT(ustr__ns(ohoff + 1 +   2) == ans[1]);
  ASSERT(ustr__ns(ohoff + 1 +   3) == ans[1]);
  
  ASSERT(ustr__ns(ohoff + 1 +   4) == ans[2]);
  ASSERT(ustr__ns(ohoff + 1 +   5) == ans[2]);
  
  ASSERT(ustr__ns(ohoff + 1 +   6) == ans[3]);
  ASSERT(ustr__ns(ohoff + 1 +   7) == ans[3]);
  ASSERT(ustr__ns(ohoff + 1 +   8) == ans[3]);
  ASSERT(ustr__ns(ohoff + 1 +   9) == ans[3]);
  
  ASSERT(ustr__ns(ohoff + 1 +  10) == ans[4]);
  ASSERT(ustr__ns(ohoff + 1 +  11) == ans[4]);
  ASSERT(ustr__ns(ohoff + 1 +  12) == ans[4]);
  ASSERT(ustr__ns(ohoff + 1 +  13) == ans[4]);
  
  ASSERT(ustr__ns(ohoff + 1 +  14) == ans[5]);
  ASSERT(ustr__ns(ohoff + 1 +  15) == ans[5]);
  ASSERT(ustr__ns(ohoff + 2 +  16) == ans[5]);
  ASSERT(ustr__ns(ohoff + 2 +  17) == ans[5]);
  ASSERT(ustr__ns(ohoff + 2 +  18) == ans[5]);
  ASSERT(ustr__ns(ohoff + 2 +  19) == ans[5]);
  ASSERT(ustr__ns(ohoff + 2 +  20) == ans[5]);

  for (num =  21; num <  29; ++num)
    ASSERT(ustr__ns(ohoff + 2 + num) == ans[6]);
  
  for (num =  29; num <  45; ++num)
    ASSERT(ustr__ns(ohoff + 2 + num) == ans[7]);
  for (num =  45; num <  61; ++num)
    ASSERT(ustr__ns(ohoff + 2 + num) == ans[8]);
  
  for (num =  61; num <  93; ++num)
    ASSERT(ustr__ns(ohoff + 2 + num) == ans[9]);
  for (num =  93; num < 125; ++num)
    ASSERT(ustr__ns(ohoff + 2 + num) == ans[10]);
  
  return (EXIT_SUCCESS);
}

