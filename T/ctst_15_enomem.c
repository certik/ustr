#include "tst.h"

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
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!(pool = ustr_pool_make_pool()));
  }
  ASSERT((pool = ustr_pool_make_pool()));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustr_pool_make_subpool(pool));
  }
  ASSERT(ustr_pool_make_subpool(pool));
  ASSERT(ustr_pool_make_subpool(pool));
  
  lim  = 2;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!pool->pool_sys_malloc(pool, 1));
  }
  ASSERT(pool->pool_sys_malloc(pool, 1));

  ASSERT(!ustr_dup_undef(-2));
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_dup_buf(pool, "xy", 1));
  }
  ASSERT((sp1 = ustrp_dup_buf(pool, "xy", 1)));
  ASSERT_PEQ(sp1, USTRP1(\1, "x"));

  if (ustrp_setf_share(sp1))
  {
    MALLOC_CHECK_STORE.mem_fail_num = 1;
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_set_buf(pool, &sp1, "ab", 1));
    ASSERT( ustrp_enomem(sp1));
    ASSERT( ustrp_setf_enomem_clr(sp1));
    ASSERT( ustrp_set_buf(pool, &sp1, "ab", 1));
    ASSERT(!ustrp_enomem(sp1));
    ASSERT_PEQ(sp1, USTRP1(\1, "a"));

    sp1 = USTRP1(\4, "abcd");
    ASSERT(ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT(ustrp_setf_share(sp1));
    MALLOC_CHECK_STORE.mem_fail_num = 1;
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_set_subustrp(pool, &sp1, sp1, 3, 1));
    ASSERT( ustrp_enomem(sp1));
    ASSERT( ustrp_setf_enomem_clr(sp1));
    ASSERT( ustrp_set_subustrp(pool, &sp1, sp1, 3, 1));
    ASSERT(!ustrp_enomem(sp1));
    ASSERT( ustrp_len(sp1) == 1);
    ASSERT( ustrp_alloc(sp1));
    ASSERT_PEQ(sp1, USTRP1(\1, "c"));

    ASSERT(ustrp_setf_share(sp1));
    MALLOC_CHECK_STORE.mem_fail_num = 1;
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_set_empty(pool, &sp1));
    ASSERT( ustrp_enomem(sp1));
    ASSERT( ustrp_setf_enomem_clr(sp1));
    ASSERT( ustrp_set_empty(pool, &sp1));
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_len(sp1));
    ASSERT( ustrp_alloc(sp1));
  }
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_dup_empty(pool));
  }
  ASSERT((sp1 = ustrp_dupx_undef(pool, 1, 0, 0, 0, 1256)));
  ASSERT(ustrp_del(pool, &sp1, 1000));
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
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
  
  MALLOC_CHECK_STORE.mem_fail_num = 1;
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
  MALLOC_CHECK_STORE.mem_fail_num = 1;
  ASSERT(ustrp_len(sp1) == 16);
  ASSERT(!ustrp_enomem(sp1));
  ASSERT( ustrp_del(pool, &sp1, 10));
  ASSERT(!ustrp_enomem(sp1));
  ASSERT(!MALLOC_CHECK_STORE.mem_fail_num);

  ASSERT(ustrp_set_cstr(pool, &sp1, "abcd"));
  if (ustrp_setf_share(sp1))
  {
    MALLOC_CHECK_STORE.mem_fail_num = 1;
    ASSERT(ustrp_len(sp1) == 4);
    ASSERT(!ustrp_enomem(sp1));
    ASSERT(!ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT( ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT( ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT((sp1 = ustrp_dup_empty(pool)));

    ASSERT(ustrp_setf_share(sp1));
    MALLOC_CHECK_STORE.mem_fail_num = 1;
    ASSERT(!ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT( ustrp_sc_ensure_owner(pool, &sp1));
    ASSERT( ustrp_sc_ensure_owner(pool, &sp1));
    
    ASSERT(ustrp_setf_share(sp1));
    MALLOC_CHECK_STORE.mem_fail_num = 1;
    ASSERT(!ustrp_sc_wstr(pool, &sp1));
    ASSERT( ustrp_sc_wstr(pool, &sp1));
    ASSERT( ustrp_sc_wstr(pool, &sp1));

    ASSERT(ustrp_set_cstr(pool, &sp1, "abcd"));
    ASSERT(ustrp_setf_share(sp1));
    MALLOC_CHECK_STORE.mem_fail_num = 1;
    ASSERT(ustrp_len(sp1) == 4);
    ASSERT(!ustrp_del_subustrp(pool, &sp1, 4, 1));
    ASSERT(!ustrp_enomem(sp1));
  }
  ASSERT(!MALLOC_CHECK_STORE.mem_fail_num);
  ASSERT( ustrp_del_subustrp(pool, &sp1, 4, 1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "abc"));

  if (!USTR_DEBUG)
  ASSERT(!ustrp_del_subustrp(pool, &sp1, 4, 1));
  
  sp1 = USTRP1(\x4, "abcd");
  MALLOC_CHECK_STORE.mem_fail_num = 1;
  ASSERT(ustrp_len(sp1) == 4);
  ASSERT(!ustrp_enomem(sp1));
  ASSERT(!ustrp_del_subustrp(pool, &sp1, 3, 1));
  ASSERT(!ustrp_enomem(sp1));
  ASSERT(!MALLOC_CHECK_STORE.mem_fail_num);
  ASSERT( ustrp_del_subustrp(pool, &sp1, 3, 1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "abd"));

  sp1 = USTRP1(\x4, "abcd");
  MALLOC_CHECK_STORE.mem_fail_num = 1;
  ASSERT(!ustrp_add_fmt(pool, &sp1, "%.500d", 4));
  MALLOC_CHECK_STORE.mem_fail_num = 1;
  ASSERT(!ustrp_set_fmt(pool, &sp1, "%.500d", 4));
  MALLOC_CHECK_STORE.mem_fail_num = 1;
  ASSERT(!(sp1 = ustrp_dup_fmt(pool, "%.500d", 4)));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
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
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!(s1 = ustr_dupx_subustr(0, 1, 1, 1, USTR1(\7, "abcdefg"), 1, 7)));
  }
  ASSERT((s1 = ustr_dupx_subustr(0, 1, 1, 1, USTR1(\7, "abcdefg"), 1, 7)));
  ASSERT_EQ(s1, USTR1(\x7, "abcdefg"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustr_add_subustr(&s1, s1, 3, 4));
  }
  ASSERT(ustr_add_subustr(&s1, s1, 3, 4));
  ASSERT(ustr_cmp_cstr_eq(s1, "abcdefgcdef"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
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
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_dup_rep_chr(pool, '-', 4));
  }
  ASSERT((sp1 = ustrp_dup_rep_chr(pool, '-', 4)));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "----"));

  ASSERT((sp1 = ustrp_dupx(pool, 0, 0, 0, 0, sp1)));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_sc_dup(pool, &sp1));
  }
  ASSERT((sp3 = ustrp_sc_dup(pool, &sp1)));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "----"));
  ASSERT(ustrp_cmp_cstr_eq(sp3, "----"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
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
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_dupx_rep_chr(pool, 0, 1, 1, 1, '=', 8));
  }
  ASSERT((sp1 = ustrp_dupx_rep_chr(pool, 0, 1, 1, 1, '=', 8)));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "========"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_add_rep_chr(pool, &sp1, '-', 2));
  }
  ASSERT(ustrp_add_rep_chr(pool, &sp1, '-', 2));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "========--"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_add_cstr(pool, &sp1, "xyz"));
  }
  ASSERT(ustrp_add_cstr(pool, &sp1, "xyz"));
  ASSERT(ustrp_cmp_cstr_eq(sp1, "========--xyz"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_add(pool, &sp2, sp1));
  }
  ASSERT(ustrp_add(pool, &sp2, sp1));
  ASSERT(ustrp_cmp_cstr_eq(sp2, ".xyz========--xyz"));

  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_add(pool, &sp2, sp1));
  }
  ASSERT(ustrp_add(pool, &sp2, sp1));
  ASSERT(ustrp_cmp_cstr_eq(sp2, ".xyz========--xyz========--xyz"));

  sp2 = USTRP("");
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
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
    MALLOC_CHECK_STORE.mem_fail_num = 1;
    ustrp_sc_del(pool, &sp2);
  }
  ASSERT(!ustrp_sized(sp2));
  ASSERT(!ustrp_alloc(sp2));
  ASSERT(!ustrp_len(sp2));
  
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT(!ustrp_add(pool, &sp2, sp1));
  }
  ASSERT(ustrp_add(pool, &sp2, sp1));
  ASSERT_PEQ(sp1, sp2);

  ustr_pool_free(pool);

  s3 = USTR1(\4, "1234");
  MALLOC_CHECK_STORE.mem_fail_num = 1;
  ASSERT(!ustr_io_get(&s3, stdin,  1, NULL));
  MALLOC_CHECK_STORE.mem_fail_num = 1;
  ASSERT(!ustr_io_putline(&s3, stdout, 2));

  sp1 = NULL;
  ustrp_sc_free_shared(pool, &sp1);

  ustr_sc_free2(&s1, USTR1(\x8, "12345678"));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT_EQ(s1, USTR1(\x8, "12345678"));
    ASSERT(!ustr_sc_reverse(&s1));
  }
  ASSERT( ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x8, "87654321"));

  ustr_sc_free2(&s1, USTR1(\x34, "abcdefghijklmnopqrstuvwxyz"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  lim  = 1;
  scan = 0;
  while (scan++ < lim)
  {
    MALLOC_CHECK_STORE.mem_fail_num = scan;
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
    MALLOC_CHECK_STORE.mem_fail_num = scan;
    ASSERT_EQ(s1, USTR1(\x34, "abcdefghijklmnopqrstuvwxyz"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    ASSERT(!ustr_sc_toupper(&s1));
  }
  ASSERT( ustr_sc_toupper(&s1));
  ASSERT_EQ(s1, USTR1(\x34, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));

  return (EXIT_SUCCESS);
}
