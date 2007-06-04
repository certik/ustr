#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make();
  Ustrp *sp1 = USTRP1(\x3, "\xE2\x80\xA0");
  Ustrp *sp2 = USTRP1(\x3, "†");

  ASSERT_PEQ(sp1, sp2);
  ASSERT(ustrp_utf8_valid(sp1));
  ASSERT(ustrp_len(sp2)      == 3);
  ASSERT(ustrp_utf8_len(sp2) == 1);
  ASSERT(ustrp_utf8_width(sp2) == 1);

  s1 = USTR1(\x1, "\xF1");
  ASSERT(!ustr_utf8_valid(s1));
  ASSERT(!ustr_utf8_len(s1));
  ASSERT(!ustr_utf8_width(s1));

  ustr_pool_free(pool);

  return (EXIT_SUCCESS);
}
