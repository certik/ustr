#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  ASSERT(ustr_ins_fmt(&s1, 0, "abcd %.4d xyz", 4));
  ASSERT_EQ(s1, USTR1(\xd, "abcd 0004 xyz"));
  ASSERT(ustr_ins_fmt(&s2, 1, "abcd %.4d xyz", 4));
  ASSERT_EQ(s2, USTR1(\xf, "sabcd 0004 xyz2"));

  ASSERT(ustr_ins_rep_chr(&s1, 4, '-', 2));
  ASSERT_EQ(s1, USTR1(\xf, "abcd-- 0004 xyz"));
  ASSERT(ustr_ins_cstr(&s1, 5, "=="));
  ASSERT_EQ(s1, USTR1(\x11, "abcd-==- 0004 xyz"));
  
  ASSERT(ustr_ins(&s2, 5, USTR1(\2, "--")));
  ASSERT_EQ(s2, USTR1(\x11, "sabcd-- 0004 xyz2"));
  ASSERT(ustr_ins_subustr(&s2, 6, USTR1(\5, "x-==-"), 3, 2));
  ASSERT_EQ(s2, USTR1(\x13, "sabcd-==- 0004 xyz2"));
  
  return (EXIT_SUCCESS);
}
