#include "ustr.h"

int main(void)
{
  Ustr *s1 = USTR("");

  if (!ustr_io_getfilename(&s1, __FILE__))
    return 1;
  
  return 0;
}
