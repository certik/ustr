#define USTR_CONF_HAVE_ATTR_DEPRECATED 0 /* so we don't have warnings */

#include "tst.h"

static const char *rf = __FILE__;

static void *fail_malloc(size_t len)
{
  (void)len;
  return NULL;
}

int tst(void)
{
  Ustr_pool *pool = ustr_pool_ll_make();
  Ustr_pool *p1 = NULL;
  Ustr_pool *p2 = NULL;
  Ustr_pool *p3 = NULL;
  Ustrp *sp1 = ustrp_dup_empty(pool);
  Ustrp *sp2 = USTRP1(\x2, "s2");
  char buf_spa[1024];
  Ustrp *spa = USTRP_SC_INIT_AUTO(buf_spa, USTR_FALSE, 0);
  size_t off = 0;

  ustr_pool_free(ustr_pool_make_pool()); /* compat. call */
  
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT( ((struct Ustr__pool_ll_base *)pool)->beg);
  ASSERT( ((struct Ustr__pool_ll_base *)pool)->beg->ptr == sp1);
  ASSERT(!((struct Ustr__pool_ll_base *)pool)->beg->next);
#endif
  
  ASSERT(pool->pool_sys_realloc(pool, NULL, 0, 0));
  ASSERT(pool->pool_sys_realloc(pool, NULL, 0, 1));
  
  ASSERT( ustrp_size_alloc(sp1));
  ASSERT( ustrp_size_alloc(sp2) >= 4);
  ASSERT(spa == (void *)buf_spa);
  ASSERT(!ustrp_alloc(spa));
  ASSERT( ustrp_owner(spa));
  ASSERT( ustrp_fixed(spa));
  ASSERT(!ustrp_limited(spa));
  ASSERT(!ustrp_ro(spa));
  ASSERT((ustrp_size(spa) + ustrp_size_overhead(spa)) == sizeof(buf_spa));
  ASSERT(ustrp_size_alloc(spa) == sizeof(buf_spa));

  ASSERT( ustrp_set_rep_chr(pool, &spa, '-', 2000));
  ASSERT(!ustrp_enomem(spa));
  ASSERT(spa != (void *)buf_spa);
  ASSERT(ustrp_set_empty(pool, &spa));
  ASSERT(ustrp_set(pool, &spa, spa));

  ASSERT((spa = USTRP_SC_INIT_AUTO(buf_spa, USTR_TRUE, 0)));
  ASSERT( ustrp_size_alloc(sp1));
  ASSERT( ustrp_size_alloc(sp2) >= 4);
  ASSERT(spa == (void *)buf_spa);
  ASSERT(!ustrp_alloc(spa));
  ASSERT( ustrp_owner(spa));
  ASSERT( ustrp_fixed(spa));
  ASSERT( ustrp_limited(spa));
  ASSERT(!ustrp_ro(spa));
  ASSERT((ustrp_size(spa) + ustrp_size_overhead(spa)) == sizeof(buf_spa));
  ASSERT(ustrp_size_alloc(spa) == sizeof(buf_spa));

  ASSERT(!ustrp_set_rep_chr(pool, &spa, '-', 2000));
  ASSERT( ustrp_enomem(spa));
  ASSERT(spa == (void *)buf_spa);
  ASSERT(ustrp_set_empty(pool, &spa));
  ASSERT(ustrp_set(pool, &spa, spa));
  
  ASSERT((spa = ustrp_init_alloc(buf_spa, sizeof(buf_spa), sizeof(buf_spa),
                                 1, 1, 0, 0)));
  ASSERT(spa == (void *)buf_spa);
  ASSERT( ustrp_alloc(spa)); /* it _thinks so_ */
  ASSERT( ustrp_owner(spa));
  ASSERT(!ustrp_fixed(spa));
  ASSERT(!ustrp_limited(spa));
  ASSERT(!ustrp_ro(spa));
  ASSERT((ustrp_size(spa) + ustrp_size_overhead(spa)) == sizeof(buf_spa));
  ASSERT(ustrp_size_alloc(spa) == sizeof(buf_spa));

  ASSERT((spa = ustrp_dup_rep_chr(pool, '-', 2000)));
  ASSERT(!ustrp_enomem(spa));
  ASSERT(spa != (void *)buf_spa);
  ASSERT(ustrp_set_empty(pool, &spa));
  ASSERT(ustrp_set(pool, &spa, spa));
  
  ASSERT((spa = ustrp_dup_undef(pool, 0)));
  ASSERT(!ustrp_alloc(spa));
  ASSERT(!ustrp_owner(spa));
  ASSERT( ustrp_ro(spa));
  ASSERT(ustrp_set_empty(pool, &spa));
  ASSERT(ustrp_set(pool, &spa, spa));

  ASSERT((spa = ustrp_dup_undef(pool, 0)));
  ASSERT(!ustrp_alloc(spa));
  ASSERT(!ustrp_owner(spa));
  ASSERT( ustrp_ro(spa));
  ASSERT(ustrp_set(pool, &spa, spa));
  if (!USTR_DEBUG)
    ASSERT(!ustrp_set_subustrp(pool, &spa, USTRP1(\x4, "abcd"), 1, 8));
  ASSERT(ustrp_set_subustrp(pool, &spa, USTRP1(\x4, "abcd"), 1, 4));

  ASSERT((spa = ustrp_dupx_empty(pool, 0, 1, 0, 0)));
  {
    size_t esz;
    size_t ref;
    int exact;
    size_t lenn;
    size_t refc;

    ustrp_conf(spa, NULL,NULL,NULL, NULL,NULL);
    ustrp_conf(spa, &esz,&ref,&exact, &lenn,&refc);
    ASSERT(!esz);
    ASSERT(ref == 1);
    ASSERT(!exact);
    ASSERT(lenn == 1);
    ASSERT(refc == 1);
  }
  
  assert(USTR_CNTL_MALLOC_CHECK_MEM_USTRP(spa));
  
  ASSERT(ustrp_dup(pool, spa));
  {
    size_t esz;
    size_t ref;
    int exact;
    size_t lenn;
    size_t refc;

    ustrp_conf(spa, NULL,NULL,NULL, NULL,NULL);
    ustrp_conf(spa, &esz,&ref,&exact, &lenn,&refc);
    ASSERT(!esz);
    ASSERT(ref == 1);
    ASSERT(!exact);
    ASSERT(lenn == 1);
    ASSERT(refc == 2);
  }
  
  ASSERT(ustrp_set(pool, &spa, USTRP1(\x4, "abcd")));

  assert(USTR_CNTL_MALLOC_CHECK_MEM_USTRP(spa));
  
  ASSERT(pool);
  ASSERT(!ustrp_ro(sp1));
  ASSERT(ustrp_len(sp1) == 0);
  ASSERT((sp1 = ustrp_dup_fmt_lim(pool, 4000, "%.5000x", 0xdeadbeef)));
  ASSERT(!ustrp_ro(sp1));
  ASSERT(ustrp_len(sp1) == 4000);
  ASSERT((sp1 = ustrp_dup_fmt(pool,           "%.5000x", 0xdeadbeef)));
  ASSERT(ustrp_len(sp1) == 5000);

  ustrp_sc_del(pool, &sp1);
  
  ustr_pool_clear(pool);
  
  ASSERT(ustrp_len(sp1) == 0);
  ASSERT(ustrp_len(sp2) == 2);
  ASSERT(ustrp_assert_valid_subustrp(sp2, 1, 1));
  ASSERT(ustrp_assert_valid_subustrp(sp2, 1, 2));
  ASSERT(ustrp_assert_valid_subustrp(sp2, 2, 1));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 1, 1));
  ASSERT(ustrp_len(sp1) == 1);
  if (!USTR_DEBUG)
    ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 3, 1));
  ASSERT(ustrp_len(sp1) == 1);
  if (!USTR_DEBUG)
    ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 1, 3));
  ASSERT(ustrp_len(sp1) == 1);
  ASSERT(ustrp_add_subustrp(pool, &sp1, sp2, 1, 2));
  ASSERT(ustrp_len(sp1) == 3);
  ASSERT(ustrp_add_subustrp(pool, &sp1, sp1, 1, 3));
  ASSERT(ustrp_len(sp1) == 6);
  ASSERT(ustrp_add_subustrp(pool, &sp1, sp1, 1, 3));
  ASSERT(ustrp_len(sp1) == 9);
  
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 1, 0)));
  ASSERT(ustrp_len(sp1) == 0);
  {
    size_t esz;
    size_t ref;
    int exact;
    size_t lenn;
    size_t refc;

    ustrp_conf(sp1, NULL,NULL,NULL, NULL,NULL);
    ustrp_conf(sp1, &esz,&ref,&exact, &lenn,&refc);
    ASSERT(!esz);
    ASSERT(!ref);
    ASSERT(!exact);
    ASSERT(lenn == 1);
    ASSERT(refc == 0);
  }
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 1, 0)));
  ASSERT(ustrp_len(sp1) == 0);
  {
    size_t esz;
    size_t ref;
    int exact;
    size_t lenn;
    size_t refc;

    ustrp_conf(sp1, NULL,NULL,NULL, NULL,NULL);
    ustrp_conf(sp1, &esz,&ref,&exact, &lenn,&refc);
    ASSERT(esz == ustrp_size_alloc(sp1));
    ASSERT(ref == 2);
    ASSERT(exact);
    ASSERT(lenn == 2);
    ASSERT(refc == 1);
  }
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 1, 2)));
  ASSERT(ustrp_len(sp1) == 2);
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 1, 2)));
  ASSERT(ustrp_len(sp1) == 2);
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 2, 1)));
  ASSERT(ustrp_len(sp1) == 1);
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 2, 1)));
  ASSERT(ustrp_len(sp1) == 1);

  if (!USTR_DEBUG)
  {
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 3, 1)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 3, 1)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 1, 3)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 1, 3)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 2, 2)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 2, 2)));
  }
  
  ustrp_sc_free2(pool, &sp1, USTRP(""));

  if (!USTR_DEBUG)
  {
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 1, 3));
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 3, 1));
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 2, 2));
  }
  
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x1, "2"));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x2, "22"));
  ASSERT( ustrp_add_rep_chr(pool, &sp1, 'x', 18));
  ASSERT_PEQ(sp1, USTRP1(\x14, "22xxxxxxxxxxxxxxxxxx"));

  /* throw away a Ustrp, so sp1 isn't the last */
  ASSERT(ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 2, 1));

  ASSERT(ustrp_del(pool, &sp1, 15));
  ASSERT_PEQ(sp1, USTRP1(\x5, "22xxx"));
  ASSERT( ustrp_add_rep_chr(pool, &sp1, 'y', 13));
  ASSERT_PEQ(sp1, USTRP1(\x12, "22xxxyyyyyyyyyyyyy"));

  /* copy and paste between here and tst_2_small.c */
  
  /* test some of the data ... */

  /* spn */
  ASSERT_PEQ(sp1, USTRP1(\x12, "22xxxyyyyyyyyyyyyy"));
  ASSERT(ustrp_spn_chr_fwd(sp1, 0, 'x') == 0);
  ASSERT(ustrp_spn_chr_fwd(sp1, 0, '2') == 2);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "2") == 2);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "22") == 2);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "2x") == 5);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "x2") == 5);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "x2y") == 18);
  ASSERT(ustrp_spn_fwd(sp1, 0, USTRP1(\x2, "22")) == 2);
  ASSERT(ustrp_spn_fwd(sp1, 0, USTRP1(\x2, "2x")) == 5);
  ASSERT(ustrp_spn_fwd(sp1, 0, USTRP1(\x2, "x2")) == 5);
  ASSERT(ustrp_spn_fwd(sp1, 0, USTRP1(\x3, "x2y")) == 18);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "x2") == 0);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "x2y") == 18);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "2y") == 13);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "y") == 13);
  ASSERT(ustrp_spn_rev(sp1, 0, USTRP1(\x2, "x2")) == 0);
  ASSERT(ustrp_spn_rev(sp1, 0, USTRP1(\x3, "x2y")) == 18);
  ASSERT(ustrp_spn_rev(sp1, 0, USTRP1(\x2, "2y")) == 13);
  ASSERT(ustrp_spn_rev(sp1, 0, USTRP1(\x1, "y")) == 13);
  ASSERT(ustrp_spn_chr_rev(sp1, 0, 'x') == 0);
  ASSERT(ustrp_spn_chr_rev(sp1, 0, 'y') == 13);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "yx") == 16);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "xy") == 16);

  /* cspn */
  ASSERT_PEQ(sp1, USTRP1(\x12, "22xxxyyyyyyyyyyyyy"));
  ASSERT(ustrp_cspn_chr_fwd(sp1, 0, 'y') == 5);
  ASSERT(ustrp_cspn_chr_fwd(sp1, 0, 'x') == 2);
  ASSERT(ustrp_cspn_chr_fwd(sp1, 0, '2') == 0);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "2") == 0);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "y") == 5);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "22") == 0);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "x") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "xx") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "yx") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "xy") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "yz") == 5);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "zy") == 5);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x1, "2")) == 0);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x1, "y")) == 5);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "22")) == 0);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x1, "x")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "xx")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "yx")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "xy")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "yz")) == 5);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "zy")) == 5);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "2x") == 13);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "x2") == 13);
  ASSERT(ustrp_cspn_chr_rev(sp1, 0, '2') == 16);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "2") == 16);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "2a") == 16);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x2, "2x")) == 13);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x2, "x2")) == 13);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x1, "2")) == 16);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x2, "2a")) == 16);
  ASSERT(ustrp_cspn_chr_fwd(sp1, 0, 'a') == 18);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "a") == 18);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "a") == 18);
  ASSERT(ustrp_cspn_chr_rev(sp1, 0, 'a') == 18);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x1, "a")) == 18);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x1, "a")) == 18);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "abc") == 18);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "abc") == 18);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x3, "abc")) == 18);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x3, "abc")) == 18);
  
  /* srch */
  ASSERT_PEQ(sp1, USTRP1(\x12, "22xxxyyyyyyyyyyyyy"));
  ASSERT(ustrp_srch_cstr_fwd(sp1, 0, "xy") == 5);
  ASSERT(ustrp_srch_cstr_rev(sp1, 0, "xy") == 5);
  ASSERT(ustrp_srch_fwd(sp1, 0, USTRP1(\x2, "xy")) == 5);
  ASSERT(ustrp_srch_rev(sp1, 0, USTRP1(\x2, "xy")) == 5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, 0, USTRP1(\x2, "xy"), 1, 2) ==  5);
  ASSERT(ustrp_srch_subustrp_rev(sp1, 0, USTRP1(\x2, "xy"), 1, 2) ==  5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, 0, USTRP1(\x2, "xy"), 1, 1) ==  3);
  ASSERT(ustrp_srch_subustrp_rev(sp1, 0, USTRP1(\x2, "xy"), 1, 1) ==  5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, 0, USTRP1(\x2, "xy"), 2, 1) ==  6);
  ASSERT(ustrp_srch_subustrp_rev(sp1, 0, USTRP1(\x2, "xy"), 2, 1) == 18);
  ASSERT(ustrp_srch_chr_fwd(sp1, 0, 'x') ==  3);
  ASSERT(ustrp_srch_chr_rev(sp1, 0, 'x') ==  5);
  ASSERT(ustrp_srch_chr_fwd(sp1, 0, 'y') ==  6);
  ASSERT(ustrp_srch_chr_rev(sp1, 0, 'y') == 18);

  /* srch_case */
  ASSERT_PEQ(sp1, USTRP1(\x12, "22xxxyyyyyyyyyyyyy"));
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "Xy") == 5);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "xY") == 5);
  ASSERT(ustrp_srch_case_fwd(sp1, 0, USTRP1(\x2, "xY")) == 5);
  ASSERT(ustrp_srch_case_rev(sp1, 0, USTRP1(\x2, "Xy")) == 5);
  ASSERT(ustrp_srch_case_subustrp_fwd(sp1, 0, USTRP1(\x2, "Xy"), 1, 2) ==  5);
  ASSERT(ustrp_srch_case_subustrp_rev(sp1, 0, USTRP1(\x2, "xY"), 1, 2) ==  5);
  ASSERT(ustrp_srch_case_subustrp_fwd(sp1, 0, USTRP1(\x2, "Xy"), 1, 1) ==  3);
  ASSERT(ustrp_srch_case_subustrp_rev(sp1, 0, USTRP1(\x2, "xY"), 1, 1) ==  5);
  ASSERT(ustrp_srch_case_subustrp_fwd(sp1, 0, USTRP1(\x2, "XY"), 2, 1) ==  6);
  ASSERT(ustrp_srch_case_subustrp_rev(sp1, 0, USTRP1(\x2, "XY"), 2, 1) == 18);

  if (!USTR_DEBUG)
  ASSERT(ustrp_srch_case_subustrp_fwd(sp1, 2, USTRP1(\4, "1248"), 4, 2) ==  3);
  if (!USTR_DEBUG)
  ASSERT(ustrp_srch_case_subustrp_rev(sp1, 2, USTRP1(\4, "yyyy"), 4, 2) == 16);
  if (!USTR_DEBUG)
  ASSERT(ustrp_srch_case_subustrp_fwd(sp1, 0, USTRP1(\4, "1248"), 5, 1) ==  1);
  if (!USTR_DEBUG)
  ASSERT(ustrp_srch_case_subustrp_rev(sp1, 0, USTRP1(\4, "YYYY"), 5, 1) == 18);
  
  ASSERT(ustrp_srch_case_chr_fwd(sp1, 0, 'x') ==  3);
  ASSERT(ustrp_srch_case_chr_rev(sp1, 0, 'x') ==  5);
  ASSERT(ustrp_srch_case_chr_fwd(sp1, 0, 'y') ==  6);
  ASSERT(ustrp_srch_case_chr_rev(sp1, 0, 'y') == 18);
  ASSERT(ustrp_srch_case_chr_fwd(sp1, 0, 'X') ==  3);
  ASSERT(ustrp_srch_case_chr_rev(sp1, 0, 'X') ==  5);
  ASSERT(ustrp_srch_case_chr_fwd(sp1, 0, 'Y') ==  6);
  ASSERT(ustrp_srch_case_chr_rev(sp1, 0, 'Y') == 18);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'x', 1) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'x', 1) ==  5);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y', 1) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y', 1) == 18);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'X', 1) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'X', 1) ==  5);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y', 1) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y', 1) == 18);

  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'z', 1) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'z', 1) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Z', 1) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Z', 1) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'z', 2) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'z', 2) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Z', 2) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Z', 2) ==  0);

  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Z', 0) ==  1);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Z', 0) == 18);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 1, 'Z', 0) ==  2);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 1, 'Z', 0) == 17);

  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'x',  2) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'x',  2) ==  4);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'x',  3) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'x',  3) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'x',  4) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'x',  4) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'X',  2) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'X',  2) ==  4);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'X',  3) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'X',  3) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'X',  4) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'X',  4) ==  0);

  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "xx")   ==  3);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "xx")   ==  4);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "xxx")  ==  3);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "xxx")  ==  3);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "xxxx") ==  0);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "xxxx") ==  0);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "XX")   ==  3);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "XX")   ==  4);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "XXX")  ==  3);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "XXX")  ==  3);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "XXXX") ==  0);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "XXXX") ==  0);

  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y',  2) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y',  2) == 17);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y',  3) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y',  3) == 16);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y',  4) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y',  4) == 15);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y', 13) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y', 13) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y', 14) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y', 14) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y',  2) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y',  2) == 17);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y',  3) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y',  3) == 16);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y',  4) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y',  4) == 15);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y', 13) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y', 13) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y', 14) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y', 14) ==  0);
  
  /* try srch_case with other case */
  ustrp_sc_free2(pool, &sp1, USTRP1(\x12, "22XXXYYYYYYYYYYYYY"));
  ASSERT_PEQ(sp1, USTRP1(\x12, "22XXXYYYYYYYYYYYYY"));
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "Xy") == 5);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "xY") == 5);
  ASSERT(ustrp_srch_case_fwd(sp1, 0, USTRP1(\x2, "xY")) == 5);
  ASSERT(ustrp_srch_case_rev(sp1, 0, USTRP1(\x2, "Xy")) == 5);
  ASSERT(ustrp_srch_case_subustrp_fwd(sp1, 0, USTRP1(\x2, "Xy"), 1, 2) ==  5);
  ASSERT(ustrp_srch_case_subustrp_rev(sp1, 0, USTRP1(\x2, "xY"), 1, 2) ==  5);
  ASSERT(ustrp_srch_case_subustrp_fwd(sp1, 0, USTRP1(\x2, "Xy"), 1, 1) ==  3);
  ASSERT(ustrp_srch_case_subustrp_rev(sp1, 0, USTRP1(\x2, "xY"), 1, 1) ==  5);
  ASSERT(ustrp_srch_case_subustrp_fwd(sp1, 0, USTRP1(\x2, "XY"), 2, 1) ==  6);
  ASSERT(ustrp_srch_case_subustrp_rev(sp1, 0, USTRP1(\x2, "XY"), 2, 1) == 18);

  ASSERT(ustrp_srch_case_chr_fwd(sp1, 0, 'x') ==  3);
  ASSERT(ustrp_srch_case_chr_rev(sp1, 0, 'x') ==  5);
  ASSERT(ustrp_srch_case_chr_fwd(sp1, 0, 'y') ==  6);
  ASSERT(ustrp_srch_case_chr_rev(sp1, 0, 'y') == 18);
  ASSERT(ustrp_srch_case_chr_fwd(sp1, 0, 'X') ==  3);
  ASSERT(ustrp_srch_case_chr_rev(sp1, 0, 'X') ==  5);
  ASSERT(ustrp_srch_case_chr_fwd(sp1, 0, 'Y') ==  6);
  ASSERT(ustrp_srch_case_chr_rev(sp1, 0, 'Y') == 18);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'x', 1) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'x', 1) ==  5);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y', 1) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y', 1) == 18);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'X', 1) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'X', 1) ==  5);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y', 1) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y', 1) == 18);

  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'z', 1) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'z', 1) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Z', 1) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Z', 1) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'z', 2) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'z', 2) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Z', 2) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Z', 2) ==  0);

  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Z', 0) ==  1);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Z', 0) == 18);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 1, 'Z', 0) ==  2);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 1, 'Z', 0) == 17);

  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'x',  2) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'x',  2) ==  4);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'x',  3) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'x',  3) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'x',  4) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'x',  4) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'X',  2) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'X',  2) ==  4);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'X',  3) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'X',  3) ==  3);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'X',  4) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'X',  4) ==  0);
  
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "xx")   ==  3);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "xx")   ==  4);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "xxx")  ==  3);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "xxx")  ==  3);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "xxxx") ==  0);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "xxxx") ==  0);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "XX")   ==  3);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "XX")   ==  4);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "XXX")  ==  3);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "XXX")  ==  3);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "XXXX") ==  0);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "XXXX") ==  0);

  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y',  2) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y',  2) == 17);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y',  3) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y',  3) == 16);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y',  4) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y',  4) == 15);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y', 13) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y', 13) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'y', 14) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'y', 14) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y',  2) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y',  2) == 17);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y',  3) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y',  3) == 16);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y',  4) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y',  4) == 15);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y', 13) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y', 13) ==  6);
  ASSERT(ustrp_srch_case_rep_chr_fwd(sp1, 0, 'Y', 14) ==  0);
  ASSERT(ustrp_srch_case_rep_chr_rev(sp1, 0, 'Y', 14) ==  0);
  
  /* show srch still works this way */
  ASSERT(ustrp_srch_cstr_fwd(sp1, 0, "XY") == 5);
  ASSERT(ustrp_srch_cstr_rev(sp1, 0, "XY") == 5);
  ASSERT(ustrp_srch_fwd(sp1, 0, USTRP1(\x2, "XY")) == 5);
  ASSERT(ustrp_srch_rev(sp1, 0, USTRP1(\x2, "XY")) == 5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, 0, USTRP1(\x2, "XY"), 1, 2) ==  5);
  ASSERT(ustrp_srch_subustrp_rev(sp1, 0, USTRP1(\x2, "XY"), 1, 2) ==  5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, 0, USTRP1(\x2, "XY"), 1, 1) ==  3);
  ASSERT(ustrp_srch_subustrp_rev(sp1, 0, USTRP1(\x2, "XY"), 1, 1) ==  5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, 0, USTRP1(\x2, "XY"), 2, 1) ==  6);
  ASSERT(ustrp_srch_subustrp_rev(sp1, 0, USTRP1(\x2, "XY"), 2, 1) == 18);
  ASSERT(ustrp_srch_chr_fwd(sp1, 0, 'X') ==  3);
  ASSERT(ustrp_srch_chr_rev(sp1, 0, 'X') ==  5);
  ASSERT(ustrp_srch_chr_fwd(sp1, 0, 'Y') ==  6);
  ASSERT(ustrp_srch_chr_rev(sp1, 0, 'Y') == 18);

  ASSERT(ustrp_srch_cstr_fwd(sp1, 0, "") == 1);
  ASSERT(ustrp_srch_cstr_rev(sp1, 0, "") == ustrp_len(sp1));
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 0, "") == 1);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 0, "") == ustrp_len(sp1));
  
  ASSERT(ustrp_srch_cstr_fwd(sp1, 2, "") == 3);
  ASSERT(ustrp_srch_cstr_rev(sp1, 2, "") == ustrp_len(sp1) - 2);
  ASSERT(ustrp_srch_case_cstr_fwd(sp1, 2, "") == 3);
  ASSERT(ustrp_srch_case_cstr_rev(sp1, 2, "") == ustrp_len(sp1) - 2);
  
  /* do some more stuff... */
  ustrp_free(pool, sp1);
  sp1 = USTRP1(\x1, "c");
  
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 1, 1));
  ASSERT_PEQ(sp1, USTRP1(\x2, "cs"));
  ASSERT(ustrp_setf_share(sp1));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "cs2"));
  ustrp_sc_free2(pool, &sp1, ustrp_dupx(pool, 1, 1, 1, 1, USTRP("")));
#if 0
  #define DBG(x)                                                       \
      printf("DBG(%s): %x:%zu:%s\n", #x ,                              \
             *(char*)sp1, ustrp_len(sp1), ustrp_cstr(sp1));            \
      fflush(NULL)
#else
  #define DBG(x) /* do nothing */
#endif
  DBG(0a);
  ASSERT_PEQ(sp1, USTRP(""));
  DBG(0b);
  ASSERT(ustrp_setf_share(sp1));
  DBG(0c);
  ASSERT(!ustrp_owner(sp1));
  DBG(0d);
  ASSERT( ustrp_shared(sp1));
  DBG(1);
  ASSERT( ustrp_enomem(sp1));
  DBG(2);
  ASSERT(!ustrp_realloc(pool, &sp1, 0));
  DBG(0e);
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  DBG(3);
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT( ustrp_owner(sp1));
  ASSERT(!ustrp_shared(sp1));
  DBG(4);
  ASSERT( ustrp_enomem(sp1));
  DBG(5);
  ASSERT(!ustrp_realloc(pool, &sp1, 1));
  ASSERT( ustrp_realloc(pool, &sp1, 2));
  ASSERT( ustrp_realloc(pool, &sp1, 2));
  ASSERT_PEQ(sp1, USTRP1(\x2, "22"));
  
  ustrp_sc_free2(pool, &sp1, USTRP(""));

  /* test subpool API */
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(!((struct Ustr__pool_ll_base *)pool)->sbeg);
  ASSERT(!((struct Ustr__pool_ll_base *)pool)->next);
  ASSERT(!((struct Ustr__pool_ll_base *)pool)->prev);
  ASSERT(!((struct Ustr__pool_ll_base *)pool)->base);
#endif
  
  ASSERT((p1 = ustr_pool_make_subpool(pool)));
  ASSERT((p2 = ustr_pool_make_subpool(pool)));
  ASSERT((p3 = ustr_pool_make_subpool(pool)));

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(&((struct Ustr__pool_ll_base *)pool)->sbeg->cbs == p3);
  ASSERT(!((struct Ustr__pool_ll_base *)pool)->next);
  ASSERT(!((struct Ustr__pool_ll_base *)pool)->prev);
  ASSERT(!((struct Ustr__pool_ll_base *)pool)->base);

  ASSERT(!((struct Ustr__pool_ll_base *)p3)->sbeg);
  ASSERT(&((struct Ustr__pool_ll_base *)p3)->next->cbs == p2);
  ASSERT(!((struct Ustr__pool_ll_base *)p3)->prev);
  ASSERT(&((struct Ustr__pool_ll_base *)p3)->base->cbs == pool);
  ASSERT(!((struct Ustr__pool_ll_base *)p2)->sbeg);
  ASSERT(&((struct Ustr__pool_ll_base *)p2)->next->cbs == p1);
  ASSERT(&((struct Ustr__pool_ll_base *)p2)->prev->cbs == p3);
  ASSERT(&((struct Ustr__pool_ll_base *)p2)->base->cbs == pool);
  ASSERT(!((struct Ustr__pool_ll_base *)p1)->sbeg);
  ASSERT(!((struct Ustr__pool_ll_base *)p1)->next);
  ASSERT(&((struct Ustr__pool_ll_base *)p1)->prev->cbs == p2);
  ASSERT(&((struct Ustr__pool_ll_base *)p1)->base->cbs == pool);
#endif
  
  ustr_pool_clear(p2);
  ustr_pool_clear(p1);
  ustr_pool_clear(p3);

  ASSERT(ustrp_dup_undef(p1, 2));
  ASSERT(ustrp_dup_undef(p1, 2));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT( ((struct Ustr__pool_ll_base *)p1)->beg);
  ASSERT( ((struct Ustr__pool_ll_base *)p2)->beg);
  ASSERT( ((struct Ustr__pool_ll_base *)p3)->beg);
#endif
  
  ustr_pool_clear(p2);

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT( ((struct Ustr__pool_ll_base *)p1)->beg);
  ASSERT(!((struct Ustr__pool_ll_base *)p2)->beg);
  ASSERT( ((struct Ustr__pool_ll_base *)p3)->beg);
#endif
  
  ustr_pool_free(p3);
  
  ASSERT((p3 = ustr_pool_make_subpool(p2)));
  
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(!((struct Ustr__pool_ll_base *)p1)->next);
  ASSERT(&((struct Ustr__pool_ll_base *)p1)->prev->cbs == p2);
  ASSERT(&((struct Ustr__pool_ll_base *)p2)->next->cbs == p1);
  ASSERT(!((struct Ustr__pool_ll_base *)p2)->prev);
  ASSERT(&((struct Ustr__pool_ll_base *)p2)->sbeg->cbs == p3);
  ASSERT(!((struct Ustr__pool_ll_base *)p3)->next);
  ASSERT(!((struct Ustr__pool_ll_base *)p3)->prev);
  ASSERT(&((struct Ustr__pool_ll_base *)p3)->base->cbs == p2);
#endif
  
  ASSERT(ustrp_dup_undef(p3, 2));
  ASSERT(ustrp_dup_undef(p3, 2));
  ASSERT(ustrp_dup_undef(p3, 4));
  ASSERT(ustrp_dup_undef(p3, 4));
  ASSERT(ustrp_dup_undef(p3, 4));
  ASSERT(ustrp_dup_undef(p3, 4));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  
  ASSERT((p3 = ustr_pool_make_subpool(pool)));
  
  ustr_pool_free(p2);
  
  ASSERT((p1 = ustr_pool_make_subpool(p3)));
  ASSERT((p2 = ustr_pool_make_subpool(p3)));
  ASSERT((p3 = ustr_pool_make_subpool(p3)));
  
  ASSERT(ustrp_dup_undef(p1, 2));
  ASSERT(ustrp_dup_undef(p1, 2));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(p1 && ((struct Ustr__pool_ll_base *)p1)->beg &&
         ((struct Ustr__pool_ll_base *)p1)->beg->ptr);
  ASSERT(p2 && ((struct Ustr__pool_ll_base *)p2)->beg &&
         ((struct Ustr__pool_ll_base *)p2)->beg->ptr);
  ASSERT(p3 && ((struct Ustr__pool_ll_base *)p3)->beg &&
         ((struct Ustr__pool_ll_base *)p3)->beg->ptr);
#endif
  
  ustr_pool_clear(pool);

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(p1 && !((struct Ustr__pool_ll_base *)p1)->beg);
  ASSERT(p2 && !((struct Ustr__pool_ll_base *)p2)->beg);
  ASSERT(p3 && !((struct Ustr__pool_ll_base *)p3)->beg);
#endif
  
  ASSERT_PEQ(USTRP1(\x8, "1234567z"), ustrp_dup_cstr(pool, "1234567z"));
  ASSERT(ustrp_assert_valid(sp1));
  ASSERT((sp1 = ustrp_dupx_cstr(pool, 0, 0, USTR_TRUE, USTR_TRUE, "1234567z")));
  ASSERT_PEQ(USTRP1(\x8, "1234567z"), sp1);
  ASSERT(!ustrp_sized(sp1));
  ASSERT(!ustrp_setf_share(sp1));
  ASSERT( ustrp_setf_owner(sp1));
  ASSERT(ustrp_exact(sp1));
  ASSERT(ustrp_size(sp1) == ustrp_len(sp1));
  ASSERT(ustrp_size_alloc(sp1) == ustrp_len(sp1) + ustrp_size_overhead(sp1));
  ASSERT( ustrp_enomem(sp1));
  ASSERT( ustrp_setf_enomem_clr(sp1));
  ASSERT(!ustrp_enomem(sp1));
  ASSERT( ustrp_setf_enomem_err(sp1));
  ASSERT( ustrp_enomem(sp1));
  ASSERT(ustrp_add_cstr(pool, &sp1, "abcd"));
  ASSERT_PEQ(USTRP1(\xc, "1234567zabcd"), sp1);

  ustrp_sc_free2(pool, &sp1, ustrp_dupx_cstr(pool, 1, 1, 1, 1, "1234abcd"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "1234abcd"));
  ASSERT(ustrp_enomem(sp1));
  ASSERT(ustrp_sized(sp1));
  ASSERT(ustrp_exact(sp1));
  ASSERT(ustrp_setf_enomem_clr(sp1));

  ASSERT(ustrp_sc_toupper(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x8, "1234ABCD"));
  ASSERT(ustrp_sc_tolower(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x8, "1234abcd"));

  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x8, "dcba4321"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x7, "234abcd"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x6, "cba432"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x5, "34abc"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x4, "ba43"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "4ab"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x2, "a4"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x1, "a"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP(""));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP(""));  
  
  ustr_pool_clear(pool);

  sp1 = USTRP("");
  ASSERT_PEQ(sp1, USTRP(""));  
  ASSERT(ustrp_ins(pool, &sp1, 0, USTRP("")));
  ASSERT(ustrp_ins(pool, &sp1, 0, USTRP("")));
  ASSERT_PEQ(sp1, USTRP(""));  
  ASSERT(ustrp_ins(pool, &sp1, 0, USTRP1(\4, "abcd")));
  ASSERT_PEQ(sp1, USTRP1(\4, "abcd"));
  ASSERT(ustrp_ins(pool, &sp1, 2, USTRP1(\1, "x")));
  ASSERT_PEQ(sp1, USTRP1(\5, "abxcd"));
  ASSERT(ustrp_ins_cstr(pool, &sp1, 2, "y"));
  ASSERT_PEQ(sp1, USTRP1(\6, "abyxcd"));
  ASSERT(ustrp_ins_buf(pool, &sp1, 1, "Zabcd", 1));
  ASSERT_PEQ(sp1, USTRP1(\7, "aZbyxcd"));
  ASSERT(ustrp_ins_rep_chr(pool, &sp1, 0, '-', 2));
  ASSERT_PEQ(sp1, USTRP1(\x9, "--aZbyxcd"));
  
  ASSERT(ustrp_sc_sub_rep_chr(pool, &sp1, 1, 4, '=', 2));
  ASSERT_PEQ(sp1, USTRP1(\7, "==byxcd"));
  ASSERT(ustrp_sc_sub_cstr(pool, &sp1, 6, 2, "___"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "==byx___"));
  ASSERT(ustrp_sc_sub_buf(pool, &sp1, 1, 2, "___", 1));
  ASSERT_PEQ(sp1, USTRP1(\7, "_byx___"));
  ASSERT(ustrp_sc_sub_fmt(pool, &sp1, 2, 6, "%2s%4d%10s", "x", 4, "y"));
  ASSERT_PEQ(sp1, USTRP1(\x11, "_ x   4         y"));
  ASSERT(ustrp_sc_sub_fmt_lim(pool, &sp1, 2, 16, 1, "%s%4d%10s", "!", 4, "y"));
  ASSERT_PEQ(sp1, USTRP1(\2, "_!"));
  ASSERT(ustrp_ins_fmt(pool, &sp1, 1, "%s%2d%s", "x", 4, "y"));
  ASSERT_PEQ(sp1, USTRP1(\6, "_x 4y!"));
  ASSERT(ustrp_ins_fmt_lim(pool, &sp1, 0, 2, "%s%2d%s", "x", 4, "y"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "x _x 4y!"));

  ASSERT(ustrp_sub_rep_chr(pool, &sp1, 1, '=', 2));
  ASSERT_PEQ(sp1, USTRP1(\x8, "==_x 4y!"));
  ASSERT(ustrp_sub_cstr(pool, &sp1, 4, "abcd"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "==_abcd!"));
  ASSERT(ustrp_sub_buf(pool, &sp1, 2, "___abcd", 3));
  ASSERT_PEQ(sp1, USTRP1(\x8, "=___bcd!"));
  ASSERT(ustrp_sub(pool, &sp1, 1, USTRP1(\3, "---")));
  ASSERT_PEQ(sp1, USTRP1(\x8, "---_bcd!"));
  ASSERT(ustrp_sc_sub(pool, &sp1, 2, 4, USTRP1(\3, "===")));
  ASSERT_PEQ(sp1, USTRP1(\7, "-===cd!"));

  ASSERT(ustrp_sub_fmt(pool, &sp1, 5, "%s", "xyz"));
  ASSERT_PEQ(sp1, USTRP1(\7, "-===xyz"));
  ASSERT(ustrp_sub_subustrp(pool, &sp1, 2, USTRP1(\4, "1234"), 2, 2));
  ASSERT_PEQ(sp1, USTRP1(\7, "-23=xyz"));
  ASSERT(ustrp_sc_sub_subustrp(pool, &sp1, 2, 4, USTRP1(\4, "12*&"), 3, 2));
  ASSERT_PEQ(sp1, USTRP1(\5, "-*&yz"));
  ASSERT(ustrp_sub_fmt_lim(pool, &sp1, 5, 2, "%s", "123456789"));
  ASSERT_PEQ(sp1, USTRP1(\6, "-*&y12"));
  
  ustrp_sc_free2(pool, &sp1, USTRP1(\x10, "123456789 123456"));
  ASSERT(!ustrp_replace(pool, &sp1, USTRP1(\1, "!"), USTRP1(\4, "abcd"), 0));
  ASSERT(!ustrp_replace_buf(pool, &sp1, "1", 2, "abcd", 4, 0));
  ASSERT_PEQ(sp1, USTRP1(\x10, "123456789 123456"));
  ASSERT(ustrp_replace_cstr(pool, &sp1, "123", "xyz", 1));
  ASSERT_PEQ(sp1, USTRP1(\x10, "xyz456789 123456"));
  ASSERT(ustrp_replace_cstr(pool, &sp1, "123", "xyz", 1));
  ASSERT_PEQ(sp1, USTRP1(\x10, "xyz456789 xyz456"));
  ASSERT(!ustrp_replace_cstr(pool, &sp1, "123", "xyz", 1));
  ASSERT_PEQ(sp1, USTRP1(\x10, "xyz456789 xyz456"));

  off = 0;
  ASSERT_PEQ(ustrp_split_cstr(pool, sp1, &off,"xyz",0,0),USTRP1(\7, "456789 "));
  ASSERT_PEQ(ustrp_split_cstr(pool, sp1, &off,"xyz",0,0),USTRP1(\3, "456"));
  ASSERT(!ustrp_split_cstr(pool, sp1, &off, "xyz", NULL, 0));
  ASSERT(!ustrp_split_cstr(pool, sp1, &off, "xyz", NULL, 0));
  off = 0;
  ASSERT_PEQ(ustrp_split(pool, sp1, &off, USTRP1(\3, "xyz"), NULL, 0),
             USTRP1(\7, "456789 "));
  ASSERT_PEQ(ustrp_split(pool, sp1, &off, USTRP1(\3, "xyz"), NULL, 0),
             USTRP1(\3, "456"));
  ASSERT(!ustrp_split(pool, sp1, &off, USTRP1(\3, "xyz"), NULL, 0));
  ASSERT(!ustrp_split(pool, sp1, &off, USTRP1(\3, "xyz"), NULL, 0));

  ASSERT(ustrp_len(sp1) == 16);
  ASSERT(ustrp_ins_undef(pool, &sp1, 1, 2));
  ASSERT(ustrp_len(sp1) == 18);
  ASSERT(ustrp_sub_undef(pool, &sp1, 3, 2));
  ASSERT(ustrp_len(sp1) == 18);
  ASSERT(ustrp_sc_sub_undef(pool, &sp1, 1, 2, 2));
  ASSERT(ustrp_len(sp1) == 18);
        
  if (!USTR_DEBUG)
  ASSERT(!ustrp_ins_subustrp(pool, &sp1, 2, USTRP1(\4, "1248"), 4, 2));
  if (!USTR_DEBUG)
  ASSERT(!ustrp_ins_subustrp(pool, &sp1, 2, USTRP1(\4, "1248"), 5, 1));
  
  ustrp_sc_free2(pool, &sp1, USTRP1(\x10, "xyz456789 xyz456"));
  ASSERT_PEQ(sp1, USTRP1(\x10, "xyz456789 xyz456"));
  ASSERT(ustrp_replace_rep_chr(pool, &sp1, 'x', 1, 'y', 1, 0) == 2);
  ASSERT_PEQ(sp1, USTRP1(\x10, "yyz456789 yyz456"));
  ASSERT(ustrp_replace_rep_chr(pool, &sp1, 'y', 1, 'z', 1, 0) == 4);
  ASSERT_PEQ(sp1, USTRP1(\x10, "zzz456789 zzz456"));
  ASSERT(ustrp_replace_rep_chr(pool, &sp1, 'z', 3, 'x', 3, 0) == 2);
  ASSERT_PEQ(sp1, USTRP1(\x10, "xxx456789 xxx456"));

  ASSERT(!ustrp_srch_rep_chr_fwd(sp1, 0, 'x', 4));
  ASSERT(!ustrp_srch_rep_chr_rev(sp1, 0, 'x', 4));
  ASSERT(ustrp_srch_rep_chr_fwd(sp1, 0, 'x', 3) ==  1);
  ASSERT(ustrp_srch_rep_chr_rev(sp1, 0, 'x', 3) == 11);
  ASSERT(ustrp_srch_rep_chr_fwd(sp1, 0, 'x', 2) ==  1);
  ASSERT(ustrp_srch_rep_chr_rev(sp1, 0, 'x', 2) == 12);
  ASSERT(ustrp_srch_rep_chr_fwd(sp1, 0, 'x', 1) ==  1);
  ASSERT(ustrp_srch_rep_chr_rev(sp1, 0, 'x', 1) == 13);
  ASSERT(ustrp_srch_rep_chr_fwd(sp1, 0, 'x', 0) ==  1);
  ASSERT(ustrp_srch_rep_chr_rev(sp1, 0, 'x', 0) == 16);
  
  ASSERT(ustrp_replace_rep_chr(pool, &sp1, 'x', 3, '!', 3, 1) == 1);
  ASSERT_PEQ(sp1, USTRP1(\x10, "!!!456789 xxx456"));

  off = 0;
  ASSERT_PEQ(ustrp_split_spn_cstr(pool, sp1, &off, "xx", NULL, 0),
             USTRP1(\xa, "!!!456789 "));
  ASSERT(off == 13);
  ASSERT_PEQ(ustrp_split_spn_cstr(pool, sp1, &off, "xx", NULL, 0),
             USTRP1(\3, "456"));
  off = 13;
  ASSERT_PEQ(ustrp_split_spn(pool, sp1, &off, USTRP1(\2, "xx"), NULL, 0),
             USTRP1(\3, "456"));
  
  sp1 = USTRP1(\x14, "123456789912345xxxxx");
  {
    size_t scan = 0x15;
    char *cstr = ustrp_sc_export(pool, sp1, malloc);
    
    ASSERT(cstr);
    ASSERT(strlen(cstr) == ustrp_len(sp1));
    ASSERT(ustrp_cmp_cstr_eq(sp1, cstr));
    free(cstr);

    while (scan--)
    {
      cstr = ustrp_sc_export_subustrp(pool, sp1, 1, scan, malloc);
      ASSERT(cstr);
      ASSERT(strlen(cstr) == scan);
      ASSERT(ustrp_cmp_prefix_cstr_eq(sp1, cstr));
      free(cstr);
    }
    scan = 0x14;
    while (scan--)
    {
      cstr = ustrp_sc_export_subustrp(pool, sp1, scan + 1, ustrp_len(sp1) -scan,
                                      malloc);
      ASSERT(cstr);
      ASSERT(strlen(cstr) == ustrp_len(sp1) - scan);
      ASSERT(ustrp_cmp_suffix_cstr_eq(sp1, cstr));
      free(cstr);
    }

    cstr = ustrp_sc_export_subustrp(pool, sp1, 4, 3, malloc);
    ASSERT(cstr);
    ASSERT(strlen(cstr) == 3);
    ASSERT(ustrp_cmp_cstr_eq(USTRP1(\3, "456"), cstr));
    free(cstr);

    cstr = ustrp_sc_export_subustrp(pool, sp1, 4, 0, malloc);
    ASSERT(cstr);
    ASSERT(strlen(cstr) == 0);
    free(cstr);

    if (!USTR_DEBUG)
    ASSERT(!ustrp_sc_export_subustrp(pool, sp1, 1, 0x15, malloc));
    if (!USTR_DEBUG)
    ASSERT(!ustrp_sc_export_subustrp(pool, sp1, 2, 0x14, malloc));
  }
  
  {
    size_t scan = 0x15;
    char *cstr = ustrp_sc_export(pool, sp1, NULL);
    
    ASSERT(cstr);
    ASSERT(strlen(cstr) == ustrp_len(sp1));
    ASSERT(ustrp_cmp_cstr_eq(sp1, cstr));

    while (scan--)
    {
      cstr = ustrp_sc_export_subustrp(pool, sp1, 1, scan, NULL);
      ASSERT(cstr);
      ASSERT(strlen(cstr) == scan);
      ASSERT(ustrp_cmp_prefix_cstr_eq(sp1, cstr));
    }
    scan = 0x14;
    while (scan--)
    {
      cstr = ustrp_sc_export_subustrp(pool, sp1, scan + 1, ustrp_len(sp1) -scan,
                                      NULL);
      ASSERT(cstr);
      ASSERT(strlen(cstr) == ustrp_len(sp1) - scan);
      ASSERT(ustrp_cmp_suffix_cstr_eq(sp1, cstr));
    }

    cstr = ustrp_sc_export_subustrp(pool, sp1, 4, 3, NULL);
    ASSERT(cstr);
    ASSERT(strlen(cstr) == 3);
    ASSERT(ustrp_cmp_cstr_eq(USTRP1(\3, "456"), cstr));

    cstr = ustrp_sc_export_subustrp(pool, sp1, 4, 0, NULL);
    ASSERT(cstr);
    ASSERT(strlen(cstr) == 0);
    
    if (!USTR_DEBUG)
    ASSERT(!ustrp_sc_export_subustrp(pool, sp1, 1, 0x15, NULL));
    if (!USTR_DEBUG)
    ASSERT(!ustrp_sc_export_subustrp(pool, sp1, 2, 0x14, NULL));
  }

  ASSERT(!ustrp_sc_export(pool, sp1, fail_malloc));
  ASSERT(!ustrp_sc_export_subustrp(pool, sp1, 1, 4, fail_malloc));
  
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT( ((struct Ustr__pool_ll_base *)pool)->free_num == 2);
  ASSERT( ((struct Ustr__pool_ll_base *)pool)->call_realloc);
#endif

  {
    unsigned int num = 0;
             int tog = 0;
    ASSERT(ustr_pool_ll_cntl(pool, USTR_POOL_LL_CNTL_GET_FREE_CMP, &num));
    ASSERT(num == 2);
    ASSERT(ustr_pool_ll_cntl(pool, USTR_POOL_LL_CNTL_SET_FREE_CMP, 8));
    ASSERT(ustr_pool_ll_cntl(pool, USTR_POOL_LL_CNTL_GET_FREE_CMP, &num));
    ASSERT(num == 8);

    ASSERT(ustr_pool_ll_cntl(pool, USTR_POOL_LL_CNTL_GET_REALLOC, &tog));
    ASSERT( tog);
    ASSERT(ustr_pool_ll_cntl(pool, USTR_POOL_LL_CNTL_SET_REALLOC, USTR_FALSE));
    ASSERT(ustr_pool_ll_cntl(pool, USTR_POOL_LL_CNTL_GET_REALLOC, &tog));
    ASSERT(!tog);
  }
  
  ustr_pool_free(pool);
  ustr_pool_free(NULL);

  return (EXIT_SUCCESS);
}
