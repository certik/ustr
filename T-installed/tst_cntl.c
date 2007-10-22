#include "ustr.h"

int main(void)
{
  size_t val = 0;

  if (!ustr_cntl_opt(USTR_CNTL_OPT_GET_REF_BYTES, &val) || (val != 1))
    return 1;
  
  return 0;
}
