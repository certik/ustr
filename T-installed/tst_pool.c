#include "ustr.h"

int main(void)
{
  Ustr_pool *p = ustr_pool_ll_make();

  if (!p)
    return 1;
  
  return 0;
}
