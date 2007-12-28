#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_ll_make();
  Ustrp *sp1 = USTRP("");

  s1 = USTR1_CHK(\6, " \ns1\f ");
  ASSERT(ustr_sc_trim(&s1, USTR1_CHK(\2, "\t ")));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_rtrim_cstr(&s1, "\n"));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_ltrim_cstr(&s1, "\f"));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_trim_chrs(&s1, "\n\f", 0));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_ltrim_chrs(&s1, "\n\f", 0));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_ltrim_cstr(&s1, ""));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_ltrim(&s1, USTR1(\2, "\f ")));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_ltrim(&s1, USTR("")));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_rtrim_chrs(&s1, "\n\f", 0));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_rtrim_cstr(&s1, ""));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_rtrim(&s1, USTR1(\2, "\n ")));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_rtrim(&s1, USTR("")));
  ASSERT_EQ(s1, USTR1(\4, "\ns1\f"));
  ASSERT(ustr_sc_trim_chrs(&s1, "\n\f", 1));
  ASSERT_EQ(s1, USTR1(\3, "s1\f"));
  ASSERT(ustr_sc_trim_chrs(&s1, "\n\f", 2));
  ASSERT_EQ(s1, USTR1(\2, "s1"));
  ASSERT(ustr_sc_trim_cstr(&s1, "  s 1  "));
  ASSERT_EQ(s1, USTR(""));
  
  sp1 = USTRP1_CHK(\5, " s1\t ");
  ASSERT(ustrp_sc_trim(pool, &sp1, USTRP1_CHK(\2, "\t ")));
  ASSERT_PEQ(sp1, USTRP1(\2, "s1"));
  
  sp1 = USTRP1_CHK(\7, " \nsp1\f ");
  ASSERT(ustrp_sc_trim(pool, &sp1, USTRP1_CHK(\2, "\t ")));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_rtrim_cstr(pool, &sp1, "\n"));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_ltrim_cstr(pool, &sp1, "\f"));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_trim_chrs(pool, &sp1, "\n\f", 0));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_ltrim_chrs(pool, &sp1, "\n\f", 0));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_ltrim_cstr(pool, &sp1, ""));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_ltrim(pool, &sp1, USTRP1(\2, "\f ")));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_ltrim(pool, &sp1, USTRP("")));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_rtrim_chrs(pool, &sp1, "\n\f", 0));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_rtrim_cstr(pool, &sp1, ""));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_rtrim(pool, &sp1, USTRP1(\2, "\n ")));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_rtrim(pool, &sp1, USTRP("")));
  ASSERT_PEQ(sp1, USTRP1(\5, "\nsp1\f"));
  ASSERT(ustrp_sc_trim_chrs(pool, &sp1, "\n\f", 1));
  ASSERT_PEQ(sp1, USTRP1(\4, "sp1\f"));
  ASSERT(ustrp_sc_trim_chrs(pool, &sp1, "\n\f", 2));
  ASSERT_PEQ(sp1, USTRP1(\3, "sp1"));
  ASSERT(ustrp_sc_trim_cstr(pool, &sp1, "  s 1 p "));
  ASSERT_PEQ(sp1, USTRP(""));
  sp1 = USTRP1_CHK(\7, " \nsp1\f ");
  ASSERT(ustrp_sc_trim_cstr(pool, &sp1, "  s 1 p \f \n "));
  ASSERT_PEQ(sp1, USTRP(""));
  
  ustr_pool_free(pool);
  
  return (EXIT_SUCCESS);
}
