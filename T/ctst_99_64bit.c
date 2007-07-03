#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  if (!USTR_CONF_HAVE_64bit_SIZE_MAX)
    return (EXIT_FAILED_OK);

  ASSERT((s1 = ustr_dupx_empty(0, 8, USTR_FALSE, USTR_FALSE)));
  ASSERT(ustr_sized(s1));
  ASSERT(ustr_size_alloc(s1) >= 14);
  ASSERT(ustr_size_overhead(s1) >= 14);
  ASSERT(ustr_dup(s1));
  ustr_free(s1);
  
  return (EXIT_SUCCESS);
}
