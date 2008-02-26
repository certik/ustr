#include "tst.h"
#include <errno.h>

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = NULL;
  Ustrp *sp1 = USTRP1(\x4, "abcd");
  Ustrp *sp2 = USTRP1(\x4, ".xyz");
  Ustrp *sp3 = USTRP1(\x8, "abcd.xyz");
  Ustr *s3 = NULL;
  unsigned int scan = 0;
  unsigned int lim  = 0;
  size_t off = 0;

  if (USTR_CONF_USE_DYNAMIC_CONF)
  {
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 1);
    TST_MC_SET_NUM(1);
    assert(USTR_CNTL_MALLOC_CHECK_ADD(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 2);
    assert(USTR_CNTL_MALLOC_CHECK_ADD(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 3);
    assert(!USTR_CNTL_MALLOC_CHECK_ADD(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 3);
    assert(USTR_CNTL_MALLOC_CHECK_ADD(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 4);
    assert(USTR_CNTL_MALLOC_CHECK_DEL(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 3);
    assert(USTR_CNTL_MALLOC_CHECK_DEL(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 2);
    assert(USTR_CNTL_MALLOC_CHECK_DEL(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 1);
    assert(!USTR_CNTL_MALLOC_CHECK_DEL(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 1);
    assert(!USTR_CNTL_MALLOC_CHECK_DEL(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 1);
    ustr_free(s2); s2 = USTR("");
    assert(USTR_CNTL_MALLOC_CHECK_END());
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 0);
    assert(!USTR_CNTL_MALLOC_CHECK_END());
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 0);
    assert(!USTR_CNTL_MALLOC_CHECK_END());
    
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 0);
    assert(USTR_CNTL_MALLOC_CHECK_BEG(USTR_TRUE));
    assert(USTR_CNTL_MALLOC_CHECK_LVL() == 1);
  }
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!(pool = ustr_pool_ll_make()));
  }
  ASSERT((pool = ustr_pool_ll_make()));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustr_pool_make_subpool(pool));
  }
  ASSERT(ustr_pool_make_subpool(pool));
  ASSERT(ustr_pool_make_subpool(pool));
  
  lim  = 2;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!pool->pool_sys_malloc(pool, 1));
  }
  ASSERT(pool->pool_sys_malloc(pool, 1));

  ASSERT(!ustr_dup_undef(-2));
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_dup_buf(pool, "xy", 1));
  }
  ASSERT((sp1 = ustrp_dup_buf(pool, "xy", 1)));
  ASSERT_PEQ(sp1, USTRP1(\1, "x"));

  if (ustrp_setf_share(sp1))
  {
    TST_MC_SET_NUM(1);
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_set_buf(pool, &sp1, "ab", 1));
    ASSERT(!ustrp_enomem(sp1));
    ASSERT_PEQ(sp1, USTRP1(\1, "x"));
    ASSERT(ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT_PEQ(sp1, USTRP1(\1, "x"));

    /*
    TST_MC_SET_NUM(1);
    ASSERT(!ustrp_set_buf(pool, &sp1, "ab", 1));
    ASSERT( ustrp_enomem(sp1));
    ASSERT( ustrp_setf_enomem_clr(sp1));
    ASSERT_PEQ(sp1, USTRP1(\1, "x"));
    */
    ASSERT( ustrp_set_buf(pool, &sp1, "ab", 1));
    ASSERT(!ustrp_enomem(sp1));
    ASSERT_PEQ(sp1, USTRP1(\1, "a"));

    sp1 = USTRP1(\4, "abcd");
    ASSERT(ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT(ustrp_setf_share(sp1));
    TST_MC_SET_NUM(1);
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_set_subustrp(pool, &sp1, sp1, 3, 1));
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_setf_enomem_clr(sp1));
    ASSERT( ustrp_set_subustrp(pool, &sp1, sp1, 3, 1));
    ASSERT(!ustrp_enomem(sp1));
    ASSERT( ustrp_len(sp1) == 1);
    ASSERT( ustrp_alloc(sp1));
    ASSERT_PEQ(sp1, USTRP1(\1, "c"));

    ASSERT(ustrp_setf_share(sp1));
    TST_MC_SET_NUM(1);
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_set_empty(pool, &sp1));
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_setf_enomem_clr(sp1));
    ASSERT( ustrp_set_empty(pool, &sp1));
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_len(sp1));
    ASSERT( ustrp_alloc(sp1));
  }
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_dup_empty(pool));
  }
  ASSERT((sp1 = ustrp_dupx_undef(pool, 1, 0, 0, 0, 1256)));
  ASSERT(ustrp_del(pool, &sp1, 1000));
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT( ustrp_setf_enomem_clr(sp1));
    ASSERT(!ustrp_realloc(pool, &sp1, 0));
    ASSERT(!ustrp_exact(sp1));
    ASSERT( ustrp_size(sp1) > ustrp_len(sp1));
    ASSERT( ustrp_enomem(sp1));
  }
  ASSERT(!ustrp_exact(sp1));
  ASSERT( ustrp_size(sp1) > ustrp_len(sp1));
  ASSERT( ustrp_realloc(pool, &sp1, 0));
  ASSERT(!ustrp_exact(sp1));
  ASSERT( ustrp_size(sp1) == ustrp_len(sp1));
  
  TST_MC_SET_NUM(1);
  ASSERT(ustrp_del(pool, &sp1, 56));
  ASSERT(ustrp_size_overhead(sp1) >= 6);
  ASSERT(!ustrp_realloc(pool, &sp1, 0));
  ASSERT(ustrp_size_overhead(sp1) >= 6);
  ASSERT( ustrp_realloc(pool, &sp1, 0));
  ASSERT(ustrp_size_overhead(sp1) >= 6);
  ASSERT(!ustrp_realloc(pool, &sp1, (65 * 1000) + 529)); /* too big */
  ASSERT(!ustrp_realloc(pool, &sp1, (65 * 1000) + 529)); /* too big */
  ASSERT(!ustrp_realloc(pool, &sp1, (65 * 1000) + 529)); /* too big */
  ASSERT((sp1 = ustrp_dupx_undef(pool, 1, 0, 1, 0, 65520)));
  ASSERT((sp1 = ustrp_dupx_undef(pool, 1, 0, 1, 0, 65500)));
  ASSERT(!ustrp_realloc(pool, &sp1, 1)); /* too small */
  ASSERT(!ustrp_realloc(pool, &sp1, 1)); /* too small */
  ASSERT(!ustrp_realloc(pool, &sp1, 1)); /* too small */
  
  ustr_pool_clear(pool);

  ASSERT((sp1 = ustrp_dupx_undef(pool, 1, 0, 0, 0, 65536)));
  ASSERT(!ustrp_add_undef(pool, &sp1, -65534));
  
  ustr_pool_clear(pool);

  ASSERT((sp1 = ustrp_dup_undef(pool, 16)));
  TST_MC_SET_NUM(1);
  ASSERT(ustrp_len(sp1) == 16);
  ASSERT(!ustrp_enomem(sp1));
  ASSERT( ustrp_del(pool, &sp1, 10));
  ASSERT(!ustrp_enomem(sp1));
  ASSERT(!TST_MC_GET_NUM());

  ASSERT(ustrp_set_cstr(pool, &sp1, "abcd"));
  if (ustrp_setf_share(sp1))
  {
    TST_MC_SET_NUM(1);
    ASSERT(ustrp_len(sp1) == 4);
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT( ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT( ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT((sp1 = ustrp_dup_empty(pool)));

    ASSERT(ustrp_setf_share(sp1));
    TST_MC_SET_NUM(1);
    ASSERT(!ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT( ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT( ustrp_sc_ensure_owner(pool, &sp1));
    
    ASSERT(ustrp_setf_share(sp1));
    TST_MC_SET_NUM(1);
    ASSERT(!ustrp_sc_wstr(pool, &sp1));
    ASSERT( ustrp_sc_wstr(pool, &sp1));
    ASSERT( ustrp_sc_wstr(pool, &sp1));

    ASSERT(ustrp_set_cstr(pool, &sp1, "abcd"));
    ASSERT(ustrp_setf_share(sp1));
    TST_MC_SET_NUM(1);
    ASSERT(ustrp_len(sp1) == 4);
    ASSERT(!ustrp_del_subustrp(pool, &sp1, 4, 1));
    ASSERT(!ustrp_enomem(sp1));
  }
  ASSERT(!TST_MC_GET_NUM());
  ASSERT( ustrp_del_subustrp(pool, &sp1, 4, 1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "abc"));

  if (!USTR_DEBUG)
  ASSERT(!ustrp_del_subustrp(pool, &sp1, 4, 1));
  
  sp1 = USTRP1(\x4, "abcd");
  TST_MC_SET_NUM(1);
  ASSERT(ustrp_len(sp1) == 4);
  ASSERT(!ustrp_enomem(sp1));
  ASSERT(!ustrp_del_subustrp(pool, &sp1, 3, 1));
  ASSERT(!ustrp_enomem(sp1));
  ASSERT(!TST_MC_GET_NUM());
  ASSERT( ustrp_del_subustrp(pool, &sp1, 3, 1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "abd"));

  sp1 = USTRP1(\x4, "abcd");
  TST_MC_SET_NUM(1);
  ASSERT(!ustrp_add_fmt(pool, &sp1, "%.500d", 4));
  TST_MC_SET_NUM(1);
  ASSERT(!ustrp_set_fmt(pool, &sp1, "%.500d", 4));
  TST_MC_SET_NUM(1);
  ASSERT(!ustrp_ins_fmt(pool, &sp1, 1, "%.500d", 4));
  TST_MC_SET_NUM(1);
  ASSERT(!ustrp_sub_fmt(pool, &sp1, 1, "%.500d", 4));
  TST_MC_SET_NUM(1);
  ASSERT(!ustrp_sc_sub_fmt(pool, &sp1, 1, 2, "%.500d", 4));
  TST_MC_SET_NUM(1);
  ASSERT(!(sp1 = ustrp_dup_fmt(pool, "%.500d", 4)));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_dup_subustrp(pool, sp3, 2, 3));
  }
  ASSERT((sp1 = ustrp_dup_subustrp(pool, sp3, 2, 3)));
  ASSERT(ustrp_cmp_subustrp_eq(sp1, sp3, 2, 3));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "bcd"));

  s1 = USTR1(\x8, "12345678");
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!(s1 = ustr_dupx_subustr(0, 1, 1, 1, USTR1(\7, "abcdefg"), 1, 7)));
  }
  ASSERT((s1 = ustr_dupx_subustr(0, 1, 1, 1, USTR1(\7, "abcdefg"), 1, 7)));
  ASSERT_EQ(s1, USTR1(\x7, "abcdefg"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustr_add_subustr(&s1, s1, 3, 4));
  }
  ASSERT(ustr_add_subustr(&s1, s1, 3, 4));
  ASSERT(ustr_cmp_cstr_eq(s1, "abcdefgcdef"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustr_sc_dupx(0, 0, 0, 0, &s1));
  }
  ASSERT((s3 = ustr_sc_dupx(0, 0, 0, 0, &s1)));
  ASSERT(ustr_cmp_cstr_eq(s1, "abcdefgcdef"));
  ASSERT(ustr_cmp_cstr_eq(s3, "abcdefgcdef"));
  ustr_sc_free(&s3);

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_dup_rep_chr(pool, '-', 4));
  }
  ASSERT((sp1 = ustrp_dup_rep_chr(pool, '-', 4)));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "----"));

  ASSERT((sp1 = ustrp_dupx(pool, 0, 0, 0, 0, sp1)));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_sc_dup(pool, &sp1));
  }
  ASSERT((sp3 = ustrp_sc_dup(pool, &sp1)));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "----"));
  ASSERT(ustrp_cmp_cstr_eq(sp3, "----"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_sc_dupx(pool, 0, 0, 0, 0, &sp1));
  }
  ASSERT((sp3 = ustrp_sc_dupx(pool, 0, 0, 0, 0, &sp1)));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "----"));
  ASSERT(ustrp_cmp_cstr_eq(sp3, "----"));

  ustr_pool_clear(pool);

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_dupx_rep_chr(pool, 0, 1, 1, 1, '=', 8));
  }
  ASSERT((sp1 = ustrp_dupx_rep_chr(pool, 0, 1, 1, 1, '=', 8)));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "========"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_add_rep_chr(pool, &sp1, '-', 2));
  }
  ASSERT(ustrp_add_rep_chr(pool, &sp1, '-', 2));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "========--"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_add_cstr(pool, &sp1, "xyz"));
  }
  ASSERT(ustrp_add_cstr(pool, &sp1, "xyz"));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "========--xyz"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_add(pool, &sp2, sp1));
  }
  ASSERT(ustrp_add(pool, &sp2, sp1));
  ASSERT(ustrp_cmp_cstr_eq(sp2, ".xyz========--xyz"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_add(pool, &sp2, sp1));
  }
  ASSERT(ustrp_add(pool, &sp2, sp1));
  ASSERT(ustrp_cmp_cstr_eq(sp2, ".xyz========--xyz========--xyz"));

  sp2 = USTRP("");
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_set(pool, &sp2, sp1));
  }
  ASSERT(ustrp_set(pool, &sp2, sp1));
  ASSERT(ustrp_cmp_cstr_eq(sp2, "========--xyz"));
  
  ASSERT((sp2 = ustrp_dupx_cstr(pool, 1, 1, 0, 0, "abcd")));
  ASSERT( ustrp_sized(sp2));
  ustrp_sc_del(pool, &sp2);
  ASSERT( ustrp_sized(sp2));
  ASSERT( ustrp_alloc(sp2));
  ASSERT(!ustrp_len(sp2));
  ASSERT((sp2 = ustrp_dupx_cstr(pool, 1, 1, 0, 0, "abcd")));
  if (!ustrp_setf_share(sp2))
    sp2 = USTRP("");
  else
  {
    ASSERT(ustrp_sized(sp2));
    TST_MC_SET_NUM(1);
    ustrp_sc_del(pool, &sp2); /* failure path makes it constant */
  }
  ASSERT(!ustrp_sized(sp2));
  ASSERT(!ustrp_alloc(sp2));
  ASSERT(!ustrp_len(sp2));

  ASSERT(ustrp_owner(sp1));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_add(pool, &sp2, sp1));
  }
  ASSERT(ustrp_add(pool, &sp2, sp1));
  ASSERT_PEQ(sp1, sp2);

  sp2 = USTRP("");
  /* convert sp1 to default config. */
  ASSERT((sp1 = ustrp_dup_buf(pool, ustrp_cstr(sp1), ustrp_len(sp1))));
  ASSERT(ustrp_owner(sp1));
  ASSERT(ustrp_add_rep_chr(pool, &sp1, '*', ustrp_size(sp1) - ustrp_len(sp1)));

  if (USTR_CONF_INCLUDE_CODEONLY_HEADERS)
  { /* no ref counts */
    ASSERT(ustrp_add(pool, &sp2, sp1));
    ASSERT_PEQ(sp1, sp2);
    ASSERT(sp1 != sp2);
  }
  else
  {
    TST_MC_SET_NUM(1); /* only adds ref. ... won't fail */
    ASSERT(ustrp_add(pool, &sp2, sp1));
    ASSERT_PEQ(sp1, sp2);
    ASSERT(sp1 == sp2);
  
    ASSERT(!ustrp_owner(sp2));
  }
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_add(pool, &sp2, sp2));
  }
  ASSERT(ustrp_add(pool, &sp2, sp2));
  ASSERT(ustrp_len(sp2) == (2 * ustrp_len(sp1)));
  ASSERT(!(ustrp_len(sp2) % 2));
  ASSERT(ustrp_cmp_prefix_buf_eq(sp2, ustrp_cstr(sp2), ustrp_len(sp2) / 2));
  ASSERT(ustrp_cmp_suffix_buf_eq(sp2, ustrp_cstr(sp2), ustrp_len(sp2) / 2));
  ASSERT(ustrp_cmp_subustrp_eq(sp1, sp2, 1, ustrp_len(sp2) / 2));

  /* make sure sp1 isn't the last thing allocated */
  ASSERT(pool->pool_sys_malloc(pool, 1));
  
  sp2 = sp1;
  ASSERT( ustrp_alloc(sp2));
  ASSERT( ustrp_owner(sp2));
  ASSERT( ustrp_size(sp2) == ustrp_len(sp2));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT(!ustrp_add(pool, &sp2, sp2));
  }
  ASSERT(ustrp_add(pool, &sp2, sp2));
  ASSERT(ustrp_len(sp2) == (2 * ustrp_len(sp1)));
  ASSERT(!(ustrp_len(sp2) % 2));
  ASSERT(ustrp_cmp_prefix_buf_eq(sp2, ustrp_cstr(sp2), ustrp_len(sp2) / 2));
  ASSERT(ustrp_cmp_suffix_buf_eq(sp2, ustrp_cstr(sp2), ustrp_len(sp2) / 2));
  ASSERT(ustrp_cmp_subustrp_eq(sp1, sp2, 1, ustrp_len(sp2) / 2));

  ustr_pool_free(pool);

  s3 = USTR1(\4, "1234");
  TST_MC_SET_NUM(1);
  ASSERT(!ustr_io_get(&s3, stdin,  1, NULL));
  TST_MC_SET_NUM(1);
  ASSERT(!ustr_io_putline(&s3, stdout, 2));

  sp1 = NULL;
  ustrp_sc_free_shared(pool, &sp1);

  ustr_sc_free2(&s1, USTR1(\x8, "12345678"));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\x8, "12345678"));
    ASSERT(!ustr_sc_reverse(&s1));
  }
  ASSERT( ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x8, "87654321"));

  ustr_sc_free2(&s1, USTR1(\x8, "12345678"));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\x8, "12345678"));
    ASSERT(!ustr_sc_utf8_reverse(&s1));
  }
  ASSERT( ustr_sc_utf8_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x8, "87654321"));

  ustr_sc_free2(&s1, USTR1(\x34, "abcdefghijklmnopqrstuvwxyz"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\x34, "abcdefghijklmnopqrstuvwxyz"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    ASSERT(!ustr_sc_tolower(&s1));
  }
  ASSERT( ustr_sc_tolower(&s1));
  ASSERT_EQ(s1, USTR1(\x34, "abcdefghijklmnopqrstuvwxyz"
                            "abcdefghijklmnopqrstuvwxyz"));

  ustr_sc_free2(&s1, USTR1(\x34, "abcdefghijklmnopqrstuvwxyz"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\x34, "abcdefghijklmnopqrstuvwxyz"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    ASSERT(!ustr_sc_toupper(&s1));
  }
  TST_MC_SET_NUM(scan);
  ASSERT( ustr_sc_toupper(&s1));
  ASSERT(TST_MC_GET_NUM() == 1);
  ASSERT_EQ(s1, USTR1(\x34, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  ustr_sc_free2(&s1, USTR1(\x34, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\x34, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    ASSERT(!ustr_sub_buf(&s1, 1, "xyz", 3));
  }
  TST_MC_SET_NUM(scan);
  ASSERT( ustr_sub_buf(&s1, 1, "xyz", 3));
  ASSERT_EQ(s1, USTR1(\x34, "xyzDEFGHIJKLMNOPQRSTUVWXYZ"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  ASSERT(TST_MC_GET_NUM() == 1);
  ustr_sc_free2(&s1, USTR1(\x34, "xyzDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\x34, "xyzDEFGHIJKLMNOPQRSTUVWXYZ"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    ASSERT(!ustr_sc_sub_rep_chr(&s1, 4, 0x31, '-', 8));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s1, USTR1(\x34, "xyzDEFGHIJKLMNOPQRSTUVWXYZ"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  ASSERT( ustr_sc_sub_rep_chr(&s1, 4, 0x31, '-', 8));
  ASSERT_EQ(s1, USTR1(\xb, "xyz--------"));
  ASSERT(TST_MC_GET_NUM() == 1);
  ustr_sc_free2(&s1, USTR1(\xb, "xyz--------"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\xb, "xyz--------"));
    ASSERT(!ustr_sub_rep_chr(&s1, 8, '=', 8));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s1, USTR1(\xb, "xyz--------"));
  ASSERT( ustr_sub_rep_chr(&s1, 8, '=', 8));
  ASSERT_EQ(s1, USTR1(\xf, "xyz----========"));
  ASSERT(TST_MC_GET_NUM() == 1);
  ustr_sc_free2(&s1, USTR1(\xf, "xyz----========"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\xf, "xyz----========"));
    ASSERT(!ustr_replace_cstr(&s1, "-", "#", 0));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s1, USTR1(\xf, "xyz----========"));
  ASSERT( ustr_replace_cstr(&s1, "-", "#", 0));
  ASSERT_EQ(s1, USTR1(\xf, "xyz####========"));
  ASSERT(TST_MC_GET_NUM() == 1);

  ustr_sc_free2(&s1, USTR1(\xf, "xyz----========"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\xf, "xyz----========"));
    ASSERT(!ustr_replace_rep_chr(&s1, '-', 1, '#', 1, 0));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s1, USTR1(\xf, "xyz----========"));
  ASSERT( ustr_replace_rep_chr(&s1, '-', 1, '#', 1, 0));
  ASSERT_EQ(s1, USTR1(\xf, "xyz####========"));
  ASSERT(TST_MC_GET_NUM() == 1);

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\xf, "xyz####========"));
    ASSERT(!ustr_replace_cstr(&s1, "x", "____", 0));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s1, USTR1(\xf, "xyz####========"));
  ASSERT( ustr_replace_cstr(&s1, "x", "____", 0));
  ASSERT_EQ(s1, USTR1(\x12, "____yz####========"));
  ASSERT(TST_MC_GET_NUM() == 1);

  ustr_sc_free2(&s2, USTR(""));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s2, USTR(""));
    off = 0;
    ASSERT(!ustr_split_cstr(s1, &off, "##", s2, 0));
  }
  TST_MC_SET_NUM(scan);
  off = 0;
  ASSERT_EQ(s2, USTR(""));
  ASSERT((s2 = ustr_split_cstr(s1, &off, "##", s2, 0)));
  ASSERT_EQ(s1, USTR1(\x12, "____yz####========"));
  ASSERT_EQ(s2, USTR1(\6, "____yz"));
  if ((ustr_size(s2) - ustr_len(s2)) >= 2)
  {
    ASSERT((s2 = ustr_split(s1, &off, USTR1(\2, "!+"), s2, 0)));
    ASSERT_EQ(s2, USTR1(\x8, "========"));
  }
  ASSERT(TST_MC_GET_NUM() == 1);
  
  ustr_sc_free2(&s2, USTR(""));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s2, USTR(""));
    off = 0;
    ASSERT(!ustr_split_spn(s1, &off, USTR1(\2, "#+"), s2, 0));
  }
  TST_MC_SET_NUM(scan);
  off = 0;
  ASSERT_EQ(s2, USTR(""));
  ASSERT((s2 = ustr_split_spn(s1, &off, USTR1(\2, "#+"), s2, 0)));
  ASSERT_EQ(s1, USTR1(\x12, "____yz####========"));
  ASSERT_EQ(s2, USTR1(\6, "____yz"));
  if ((ustr_size(s2) - ustr_len(s2)) >= 2)
  {
    ASSERT((s2 = ustr_split_spn(s1, &off, USTR1(\2, "!+"), s2, 0)));
    ASSERT_EQ(s2, USTR1(\x8, "========"));
  }
  ASSERT(TST_MC_GET_NUM() == 1);
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s1, USTR1(\x12, "____yz####========"));
    ASSERT(!ustr_replace_rep_chr(&s1, '_', 1, '-', 2, 0));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s1, USTR1(\x12, "____yz####========"));
  ASSERT( ustr_replace_rep_chr(&s1, '_', 1, '-', 2, 0));
  ASSERT_EQ(s1, USTR1(\x16, "--------yz####========"));
  ASSERT(TST_MC_GET_NUM() == 1);

  ustr_sc_free2(&s2, USTR1(\6, "____yz"));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s2, USTR1(\6, "____yz"));
    ASSERT(!ustr_ins_cstr(&s2, 1, "#"));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s2, USTR1(\6, "____yz"));
  ASSERT( ustr_ins_cstr(&s2, 1, "##"));
  ASSERT_EQ(s2, USTR1(\x8, "_##___yz"));
  ASSERT(TST_MC_GET_NUM() == 1);
  
  ustr_sc_free2(&s2, USTR1(\x8, "_##___yz"));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s2, USTR1(\x8, "_##___yz"));
    ASSERT(!ustr_ins_rep_chr(&s2, 2, '+', 2));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s2, USTR1(\x8, "_##___yz"));
  ASSERT( ustr_ins_rep_chr(&s2, 2, '+', 2));
  ASSERT_EQ(s2, USTR1(\xa, "_#++#___yz"));
  ASSERT(TST_MC_GET_NUM() == 1);
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s2, USTR1(\xa, "_#++#___yz"));
    ASSERT(!ustr_ins_fmt(&s2, 3, "%s", "================"));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s2, USTR1(\xa, "_#++#___yz"));
  ASSERT( ustr_ins_fmt(&s2, 3, "%s", "================"));
  ASSERT_EQ(s2, USTR1(\x1a, "_#+================+#___yz"));
  ASSERT(TST_MC_GET_NUM() == 1);

  s3 = ustr_dup(s2);
  if (!ustr_owner(s2)) {
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s2, USTR1(\x1a, "_#+================+#___yz"));
    ASSERT(!ustr_sub_fmt(&s2, 4, "%s", "----------------"));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s2, USTR1(\x1a, "_#+================+#___yz"));
  ASSERT( ustr_sub_fmt(&s2, 4, "%s", "----------------"));
  ASSERT_EQ(s2, USTR1(\x1a, "_#+----------------+#___yz"));
  ASSERT(TST_MC_GET_NUM() == 1);
  }
  ustr_sc_free2(&s3, ustr_dup(s2));
  
  if (!ustr_owner(s2)) {
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    TST_MC_SET_NUM(scan);
    ASSERT_EQ(s2, USTR1(\x1a, "_#+----------------+#___yz"));
    ASSERT(!ustr_sc_sub_fmt(&s2, 4, 16, "%s", "................"));
  }
  TST_MC_SET_NUM(scan);
  ASSERT_EQ(s2, USTR1(\x1a, "_#+----------------+#___yz"));
  ASSERT( ustr_sc_sub_fmt(&s2, 4, 16, "%s", "................"));
  ASSERT_EQ(s2, USTR1(\x1a, "_#+................+#___yz"));
  ASSERT(TST_MC_GET_NUM() == 1);
  }
  ustr_sc_free2(&s3, USTR(""));

  TST_MC_SET_NUM(0);
  
  ustr_sc_free2(&s2, USTR(""));
  ustr_set_rep_chr(&s2, '-', 65);
  ASSERT(ustr_size_alloc(s2) == 96);
  TST_MC_SET_NUM(1);
  ASSERT(ustr_del(&s2, 60));
  ASSERT(ustr_size_alloc(s2) < 96);
  ASSERT(!ustr_enomem(s2));
  ASSERT(ustr_len(s2) == 5);
  ASSERT(ustr_cmp_cstr_eq(s2, "-----"));
  assert(USTR_CNTL_MALLOC_CHECK_MEM(s2));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_MINSZ(s2, ustr_size_alloc(s2)));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_SZ(s2, ustr_size_alloc(s2)));

  ASSERT((pool = ustr_pool_ll_make()));

  sp2 = USTRP("");
  ustrp_set_rep_chr(pool, &sp2, '-', 65);
  ASSERT(ustrp_size_alloc(sp2) == 96);
  TST_MC_SET_NUM(1);
  ASSERT(ustrp_del(pool, &sp2, 60));
  ASSERT(ustrp_size_alloc(sp2) < 96);
  ASSERT(!ustrp_enomem(sp2));
  ASSERT(ustrp_len(sp2) == 5);
  ASSERT(ustrp_cmp_cstr_eq(sp2, "-----"));
  assert(USTR_CNTL_MALLOC_CHECK_MEM(sp2));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_MINSZ(sp2, ustrp_size_alloc(sp2)));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_SZ(sp2, 96));

  /* now with exact implicit SIZE */
  
  ustr_sc_free2(&s2, ustr_dupx_empty(0, 0, 1, 0));
  ustr_set_rep_chr(&s2, '-', 65);
  ASSERT(ustr_size_alloc(s2) < 96);
  off = ustr_size_alloc(s2);
  TST_MC_SET_NUM(1);
  ASSERT(ustr_del(&s2, 60));
  ASSERT(ustr_size_alloc(s2) < off);
  ASSERT(!ustr_enomem(s2));
  ASSERT(ustr_len(s2) == 5);
  ASSERT(ustr_cmp_cstr_eq(s2, "-----"));
  assert(USTR_CNTL_MALLOC_CHECK_MEM(s2));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_MINSZ(s2, ustr_size_alloc(s2)));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_SZ(s2, ustr_size_alloc(s2)));

  ASSERT((sp2 = ustrp_dupx_empty(pool, 0, 0, 1, 0)));
  ustrp_set_rep_chr(pool, &sp2, '-', 65);
  ASSERT(ustrp_size_alloc(sp2) < 96);
  off = ustrp_size_alloc(sp2);
  TST_MC_SET_NUM(1);
  ASSERT(ustrp_del(pool, &sp2, 60));
  ASSERT(ustrp_size_alloc(sp2) < off);
  ASSERT(!ustrp_enomem(sp2));
  ASSERT(ustrp_len(sp2) == 5);
  ASSERT(ustrp_cmp_cstr_eq(sp2, "-----"));
  assert(USTR_CNTL_MALLOC_CHECK_MEM(sp2));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_MINSZ(sp2, ustrp_size_alloc(sp2)));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_SZ(sp2, off));

  /* now with inexact explicit stored SIZE */
  
  ustr_sc_free2(&s2, ustr_dupx_empty(1, 0, 0, 0));
  ustr_set_rep_chr(&s2, '-', 65);
  ASSERT(ustr_size_alloc(s2) == 96);
  TST_MC_SET_NUM(1);
  ASSERT(ustr_del(&s2, 60));
  ASSERT(ustr_size_alloc(s2) == 96);
  ASSERT(!ustr_enomem(s2));
  ASSERT(ustr_len(s2) == 5);
  ASSERT(ustr_cmp_cstr_eq(s2, "-----"));
  assert(USTR_CNTL_MALLOC_CHECK_MEM(s2));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_MINSZ(s2, ustr_size_alloc(s2)));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_SZ(s2, ustr_size_alloc(s2)));

  TST_MC_SET_NUM(0);
  ASSERT((sp2 = ustrp_dupx_empty(pool, 1, 0, 0, 0)));
  ustrp_set_rep_chr(pool, &sp2, '-', 65);
  ASSERT(ustrp_size_alloc(sp2) == 96);
  TST_MC_SET_NUM(1);
  ASSERT(ustrp_del(pool, &sp2, 60));
  ASSERT(ustrp_size_alloc(sp2) == 96);
  ASSERT(!ustrp_enomem(sp2));
  ASSERT(ustrp_len(sp2) == 5);
  ASSERT(ustrp_cmp_cstr_eq(sp2, "-----"));
  assert(USTR_CNTL_MALLOC_CHECK_MEM(sp2));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_MINSZ(sp2, ustrp_size_alloc(sp2)));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_SZ(sp2, ustrp_size_alloc(sp2)));

  ustr_pool_free(pool);

  TST_MC_SET_NUM(0); errno = 0;
  ustr_sc_free2(&s1, USTR(""));
  ASSERT(ustr_add(&s1, USTR1(\1, "a")));
  ASSERT(ustr_add(&s1, USTR1(\1, "d")));
  ASSERT_EQ(s1, USTR1(\2, "ad"));
  TST_MC_SET_NUM(1); errno = 0;
  ASSERT(!ustr_ins(&s1, 1, s1));
  ASSERT(errno == ENOMEM);
  ASSERT_EQ(s1, USTR1(\2, "ad"));
  TST_MC_SET_NUM(2); errno = 0;
  ASSERT( ustr_ins(&s1, 1, s1));
  ASSERT_EQ(s1, USTR1(\4, "aadd"));
  TST_MC_SET_NUM(0); errno = 0;

  ASSERT(ustr_set_cstr(&s1, "1234567"));
  ASSERT_EQ(s1, USTR1(\7, "1234567"));
  TST_MC_SET_NUM(1); errno = 0;
  ASSERT(!ustr_ins_subustr(&s1, 2, s1, 2, 5));
  ASSERT(errno == ENOMEM);
  ASSERT_EQ(s1, USTR1(\7, "1234567"));
  TST_MC_SET_NUM(2); errno = 0;
  ASSERT( ustr_ins_subustr(&s1, 2, s1, 2, 5));
  ASSERT_EQ(s1, USTR1(\xC, "122345634567"));
  TST_MC_SET_NUM(0); errno = 0;

  ustr_free(s1);
  
  TST_MC_SET_NUM(1); errno = 0;
  ASSERT(!(s1 = ustr_sc_concat(USTR1(\2, "ab"), USTR1(\2, "cd"), USTR_NULL)));
  ASSERT(errno == ENOMEM);
#if USTR_CONF_HAVE_VA_COPY
  TST_MC_SET_NUM(2); errno = 0;
  ASSERT( (s1 = ustr_sc_concat(USTR1(\2, "ab"), USTR1(\2, "cd"), USTR_NULL)));
  ASSERT_EQ(s1, USTR1(\4, "abcd"));
#endif
  TST_MC_SET_NUM(0); errno = 0;

  ASSERT(ustr_owner(s1));
  ustr_free(s1);

  TST_MC_SET_NUM(1); errno = 0;
  ASSERT(!(s1 = ustr_sc_join(USTR1(\1, ","), USTR1(\1, "1"), USTR(""), USTR(""),
                             USTR1(\1, "4"), USTR1(\1, "5"), USTR_NULL)));
  ASSERT(errno == ENOMEM);
#if USTR_CONF_HAVE_VA_COPY
  TST_MC_SET_NUM(2); errno = 0;
  ASSERT( (s1 = ustr_sc_join(USTR1(\1, ","), USTR1(\1, "1"), USTR(""), USTR(""),
                             USTR1(\1, "4"), USTR1(\1, "5"), USTR_NULL)));
  ASSERT_EQ(s1, USTR1(\7, "1,,,4,5"));
#endif
  TST_MC_SET_NUM(0); errno = 0;

  ASSERT(ustr_owner(s1));
  
  TST_MC_SET_NUM(1); errno = 0;
  ASSERT(!ustr_sc_sub_subustr(&s1, 3, 3, s1, 5, 1));
  ASSERT(errno == ENOMEM);
  ASSERT_EQ(s1, USTR1(\7, "1,,,4,5"));
  TST_MC_SET_NUM(2); errno = 0;
  ASSERT( ustr_sc_sub_subustr(&s1, 3, 3, s1, 5, 1));
  ASSERT_EQ(s1, USTR1(\5, "1,4,5"));
  TST_MC_SET_NUM(0); errno = 0;

  ASSERT(ustr_add_rep_chr(&s1, '-', 2));
  ASSERT(ustr_ins_rep_chr(&s1, 0, '-', 2));
  ASSERT_EQ(s1, USTR1(\x9, "--1,4,5--"));
  TST_MC_SET_NUM(1); errno = 0;
  ASSERT(!ustr_sc_sub(&s1, 5, 1, s1));
  ASSERT(errno == ENOMEM);
  ASSERT_EQ(s1, USTR1(\x9, "--1,4,5--"));
  TST_MC_SET_NUM(2); errno = 0;
  ASSERT( ustr_sc_sub(&s1, 5, 1, s1));
  ASSERT_EQ(s1, USTR1(\x11, "--1,--1,4,5--,5--"));
  TST_MC_SET_NUM(0); errno = 0;
  
  TST_MC_SET_NUM(1); errno = 0;
  ASSERT(!ustr_sub_subustr(&s1, 5, s1, 9, 5));
  ASSERT_EQ(s1, USTR1(\x11, "--1,--1,4,5--,5--"));
  ASSERT( ustr_sub_subustr(&s1, 5, s1, 9, 5));
  ASSERT_EQ(s1, USTR1(\x11, "--1,4,5--,5--,5--"));
  TST_MC_SET_NUM(1); errno = 0;
  ASSERT(!ustr_sub_subustr(&s1, 17, s1, 2, 16));
  ASSERT_EQ(s1, USTR1(\x11, "--1,4,5--,5--,5--"));
  TST_MC_SET_NUM(2); errno = 0;
  ASSERT(!ustr_sub_subustr(&s1, 17, s1, 2, 16));
  ASSERT_EQ(s1, USTR1(\x11, "--1,4,5--,5--,5--"));
  TST_MC_SET_NUM(3); errno = 0;
  ASSERT( ustr_sub_subustr(&s1, 17, s1, 2, 16));
  ASSERT_EQ(s1, USTR1(\x20, "--1,4,5--,5--,5--1,4,5--,5--,5--"));
  TST_MC_SET_NUM(0); errno = 0;
  
  return (EXIT_SUCCESS);
}
