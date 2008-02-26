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

  ustr_sc_free2(&s1, USTR1(\4, "1234"));
  ASSERT(ustr_ins_fmt_lim(&s1, 2, 150, "%75s%-75sXYZ", "|", "|"));
  ASSERT(ustr_cmp_cstr_eq(s1, "12                                                                          ||                                                                          34"));
  ASSERT(ustr_ins_fmt_lim(&s1, 75, 2, "%s%s", "", ""));
  ASSERT(ustr_ins_fmt_lim(&s1, 75, 1, "%s%s", "", ""));
  ASSERT(ustr_ins_fmt_lim(&s1, 75, 0, "%s%s", "", ""));
  ASSERT(ustr_cmp_cstr_eq(s1, "12                                                                          ||                                                                          34"));
  ASSERT(ustr_len(s1) == 154);
  ASSERT(ustr_ins_undef(&s1, 2, 2));
  ASSERT(ustr_len(s1) == 156);
  ASSERT(ustr_cmp_prefix_cstr_eq(s1, "12"));
  ASSERT(ustr_cmp_suffix_cstr_eq(s1, "                                                                          ||                                                                          34"));

  if (!USTR_DEBUG)
  ASSERT(!ustr_ins_subustr(&s1, 2, USTR1(\4, "1248"), 4, 2));
  if (!USTR_DEBUG)
  ASSERT(!ustr_ins_subustr(&s1, 2, USTR1(\4, "1248"), 5, 1));
  
  ustr_sc_free2(&s1, USTR1(\4, "abcd"));
  ASSERT(ustr_ins_subustr(&s1, 1, s1, 2, 2));
  ASSERT_EQ(s1, USTR1(\6, "abcbcd"));
  ASSERT(ustr_ins_subustr(&s1, 3, s1, 1, 1));
  ASSERT_EQ(s1, USTR1(\7, "abcabcd"));
  ASSERT(ustr_ins_subustr(&s1, 7, s1, 3, 1));
  ASSERT_EQ(s1, USTR1(\x8, "abcabcdc"));
  ASSERT(ustr_ins_subustr(&s1, 7, s1, 4, 1));
  ASSERT_EQ(s1, USTR1(\x9, "abcabcdac"));
  ASSERT(ustr_ins_subustr(&s1, 5, s1, 2, 8));
  ASSERT_EQ(s1, USTR1(\x11, "abcabbcabcdaccdac"));

  ustr_set_cstr(&s1, "123");
  ASSERT(ustr_ins(&s1, 0, s1));
  ASSERT_EQ(s1, USTR1(\6, "123123"));
  ustr_set_cstr(&s1, "123");
  ASSERT(ustr_ins(&s1, 1, s1));
  ASSERT_EQ(s1, USTR1(\6, "112323"));
  ustr_set_cstr(&s1, "123");
  ASSERT(ustr_ins(&s1, 2, s1));
  ASSERT_EQ(s1, USTR1(\6, "121233"));
  ustr_set_cstr(&s1, "123");
  ASSERT(ustr_ins(&s1, 3, s1));
  ASSERT_EQ(s1, USTR1(\6, "123123"));
  
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 0, s1, 1, 2));
  ASSERT_EQ(s1, USTR1(\4, "1212"));
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 1, s1, 1, 2));
  ASSERT_EQ(s1, USTR1(\4, "1122"));
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 2, s1, 1, 2));
  ASSERT_EQ(s1, USTR1(\4, "1212"));
  
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 0, s1, 1, 1));
  ASSERT_EQ(s1, USTR1(\3, "112"));
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 1, s1, 1, 1));
  ASSERT_EQ(s1, USTR1(\3, "112"));
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 2, s1, 1, 1));
  ASSERT_EQ(s1, USTR1(\3, "121"));
  
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 0, s1, 2, 1));
  ASSERT_EQ(s1, USTR1(\3, "212"));
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 1, s1, 2, 1));
  ASSERT_EQ(s1, USTR1(\3, "122"));
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 2, s1, 2, 1));
  ASSERT_EQ(s1, USTR1(\3, "122"));
  
  ustr_set_cstr(&s1, "12");
  ASSERT(ustr_ins_subustr(&s1, 0, s1, 2, 0));
  ASSERT(ustr_ins_subustr(&s1, 1, s1, 2, 0));
  ASSERT(ustr_ins_subustr(&s1, 2, s1, 2, 0));
  ASSERT_EQ(s1, USTR1(\2, "12"));
  
  ustr_set_cstr(&s1, "123456789 ");
  ASSERT(ustr_ins(&s1, 4, s1));
  ASSERT_EQ(s1, USTR1(\x14, "1234123456789 56789 "));

  ustr_set_cstr(&s1, "123456789 ");
  ASSERT(ustr_ins_subustr(&s1, 8, s1, 6, 4));
  ASSERT_EQ(s1, USTR1(\xe, "1234567867899 "));

  return (EXIT_SUCCESS);
}
