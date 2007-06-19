#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make_pool();
  Ustrp *sp1 = USTRP("");
  Ustr  *s3  = ustr_dupx_empty(0, 1, 0, 0);
  Ustr  *s4  = USTR("");
  Ustrp *sp3 = ustrp_dupx_empty(pool, 0, 1, 0, 0);
  Ustrp *sp4 = USTRP("");
  
  ASSERT(pool);
  ASSERT(s3);

  ASSERT(ustr_set_rep_chr(&s4, '0', 1024));
  ASSERT(ustrp_set_rep_chr(pool, &sp4, '0', 1024));

  ustr_sc_free2(&s1, ustr_dup(s3));
  ustr_sc_free2(&s1, ustr_dup(s3));
  ustr_sc_free2(&s1, ustr_dup(s3));
  ustr_sc_free2(&s1, ustr_dup(s3));
  
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  
  ASSERT(ustr_add_fmt_lim(&s1, 1, "%s", "12"));
  ASSERT_EQ(s1, USTR1(\x1, "1"));
  
  ASSERT(ustrp_add_fmt_lim(pool, &sp1, 1024, "%.2000d", 0));
  ASSERT_PEQ(sp1, sp4);
  
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  ASSERT(ustrp_add_fmt(pool, &sp1, "%.1024d", 0));
  ASSERT_PEQ(sp1, sp4);
  
  ustr_sc_free(&s1);
  ASSERT((s1 = ustr_dupx_fmt_lim(1, 1, 1, 0, 4, "%s", "12345678")));
  ASSERT_EQ(s1, USTR1(\x4, "1234"));
  
  ustrp_sc_free(pool, &sp1);
  ASSERT((sp1 = ustrp_dupx_fmt_lim(pool, 1, 1, 1, 0, 4, "%s", "12345678")));
  ASSERT_PEQ(sp1, USTRP1(\x4, "1234"));
  
  ustr_sc_free(&s1);
  ASSERT((s1 = ustr_dupx_fmt(1, 1, 1, 0, "%s", "12345678")));
  ASSERT_EQ(s1, USTR1(\x8, "12345678"));
  
  ustrp_sc_free(pool, &sp1);
  ASSERT((sp1 = ustrp_dupx_fmt(pool, 1, 1, 1, 0, "%s", "12345678")));
  ASSERT_PEQ(sp1, USTRP1(\x8, "12345678"));
  
  ustr_sc_free(&s1);
  ASSERT((s1 = ustr_dup_fmt_lim(4, "%s", "12345678")));
  ASSERT_EQ(s1, USTR1(\x4, "1234"));
  
  ustrp_sc_free(pool, &sp1);
  ASSERT((sp1 = ustrp_dup_fmt_lim(pool, 4, "%s", "12345678")));
  ASSERT_PEQ(sp1, USTRP1(\x4, "1234"));
  
  ustr_sc_free(&s1);
  ASSERT((s1 = ustr_dup_fmt("%s", "12345678")));
  ASSERT_EQ(s1, USTR1(\x8, "12345678"));
  
  ustrp_sc_free(pool, &sp1);
  ASSERT((sp1 = ustrp_dup_fmt(pool, "%s", "12345678")));
  ASSERT_PEQ(sp1, USTRP1(\x8, "12345678"));

  ASSERT(ustr_set_undef(&s1, 400));
  ASSERT(ustrp_set_undef(pool, &sp1, 400));
  
  ASSERT(ustr_set_fmt(&s1, "%s", "12345678"));
  ASSERT_EQ(s1, USTR1(\x8, "12345678"));

  ASSERT(ustrp_set_fmt(pool, &sp1, "%s", "12345678"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "12345678"));
  
  ASSERT(ustr_set_fmt_lim(&s1, 2, "%s", "12345678"));
  ASSERT_EQ(s1, USTR1(\x2, "12"));

  ASSERT(ustrp_set_fmt_lim(pool, &sp1, 2, "%s", "12345678"));
  ASSERT_PEQ(sp1, USTRP1(\x2, "12"));
  
  ASSERT(ustr_set_fmt_lim(&s1, 4, "%s", "12345678"));
  ASSERT_EQ(s1, USTR1(\x4, "1234"));

  ASSERT(ustrp_set_fmt_lim(pool, &sp1, 4, "%s", "12345678"));
  ASSERT_PEQ(sp1, USTRP1(\x4, "1234"));
  
  ASSERT(ustr_set_empty(&s1));
  ASSERT(ustrp_set_empty(pool, &sp1));

  ASSERT(ustrp_set_cstr(pool, &sp1, "abcd"));
  ASSERT_PEQ(sp1, USTRP1(\x4, "abcd"));
  ASSERT(ustrp_set(pool, &sp1, USTRP1(\x3, "xyz")));
  ASSERT_PEQ(sp1, USTRP1(\x3, "xyz"));

  ustr_sc_free2(&s1, USTR1(\x3, "xyz"));
  ASSERT(ustr_set_subustr(&s1, s1, 2, 1));
  ASSERT_EQ(s1, USTR1(\x1, "y"));

  ASSERT(ustrp_set_subustrp(pool, &sp1, sp1, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x1, "y"));
  
  ustr_pool_free(pool);

  ustr_sc_free(&s3);
  ustr_sc_free(&s4);

  return (EXIT_SUCCESS);
}
