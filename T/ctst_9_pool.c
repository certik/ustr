#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make_pool();
  Ustr_pool *p1 = NULL;
  Ustr_pool *p2 = NULL;
  Ustr_pool *p3 = NULL;
  Ustrp *sp1 = ustrp_dup_empty(pool);
  Ustrp *sp2 = USTRP1(\x2, "s2");
  char buf_spa[1024];
  Ustrp *spa = USTRP_SC_INIT_AUTO(buf_spa, USTR_FALSE, 0);

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT( ((struct Ustr__pool_si_base *)pool)->beg);
  ASSERT( ((struct Ustr__pool_si_base *)pool)->beg->ptr == sp1);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->beg->next);
#endif
  
  ASSERT(pool->pool_sys_realloc(pool, NULL, 0, 0));
  ASSERT(pool->pool_sys_realloc(pool, NULL, 0, 1));
  
  ASSERT( ustrp_size_alloc(sp1));
  ASSERT(!ustrp_size_alloc(sp2));
  ASSERT(spa == (void *)buf_spa);
  ASSERT(!ustrp_alloc(spa));
  ASSERT( ustrp_owner(spa));
  ASSERT( ustrp_fixed(spa));
  ASSERT(!ustrp_limited(spa));
  ASSERT(!ustrp_ro(spa));
  ASSERT((ustrp_size(spa) + ustrp_overhead(spa)) == sizeof(buf_spa));
  ASSERT(ustrp_size_alloc(spa) == sizeof(buf_spa));

  ASSERT( ustrp_set_rep_chr(pool, &spa, '-', 2000));
  ASSERT(!ustrp_enomem(spa));
  ASSERT(spa != (void *)buf_spa);
  ASSERT(ustrp_set_empty(pool, &spa));
  ASSERT(ustrp_set(pool, &spa, spa));

  ASSERT((spa = USTRP_SC_INIT_AUTO(buf_spa, USTR_TRUE, 0)));
  ASSERT( ustrp_size_alloc(sp1));
  ASSERT(!ustrp_size_alloc(sp2));
  ASSERT(spa == (void *)buf_spa);
  ASSERT(!ustrp_alloc(spa));
  ASSERT( ustrp_owner(spa));
  ASSERT( ustrp_fixed(spa));
  ASSERT( ustrp_limited(spa));
  ASSERT(!ustrp_ro(spa));
  ASSERT((ustrp_size(spa) + ustrp_overhead(spa)) == sizeof(buf_spa));
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
  ASSERT((ustrp_size(spa) + ustrp_overhead(spa)) == sizeof(buf_spa));
  ASSERT(ustrp_size_alloc(spa) == sizeof(buf_spa));

  /* this is a huge hack based on pool_free() being a noop when it wasn't
   * the last ptr. */
  ASSERT( ustrp_set_rep_chr(pool, &spa, '-', 2000));
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
  ASSERT(ustrp_dup(pool, spa));
  ASSERT(ustrp_set(pool, &spa, USTRP1(\x4, "abcd")));
  
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
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 1, 0)));
  ASSERT(ustrp_len(sp1) == 0);
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

  /* test some of the data ... */

  /* spn */
  ASSERT(ustrp_spn_chr_fwd(sp1, 'x') == 0);
  ASSERT(ustrp_spn_chr_fwd(sp1, '2') == 2);
  ASSERT(ustrp_spn_cstr_fwd(sp1, "2") == 2);
  ASSERT(ustrp_spn_cstr_fwd(sp1, "22") == 2);
  ASSERT(ustrp_spn_cstr_fwd(sp1, "2x") == 5);
  ASSERT(ustrp_spn_cstr_fwd(sp1, "x2") == 5);
  ASSERT(ustrp_spn_cstr_fwd(sp1, "x2y") == 18);
  ASSERT(ustrp_spn_fwd(sp1, USTRP1(\x2, "22")) == 2);
  ASSERT(ustrp_spn_fwd(sp1, USTRP1(\x2, "2x")) == 5);
  ASSERT(ustrp_spn_fwd(sp1, USTRP1(\x2, "x2")) == 5);
  ASSERT(ustrp_spn_fwd(sp1, USTRP1(\x3, "x2y")) == 18);
  ASSERT(ustrp_spn_cstr_rev(sp1, "x2") == 0);
  ASSERT(ustrp_spn_cstr_rev(sp1, "x2y") == 18);
  ASSERT(ustrp_spn_cstr_rev(sp1, "2y") == 13);
  ASSERT(ustrp_spn_cstr_rev(sp1, "y") == 13);
  ASSERT(ustrp_spn_rev(sp1, USTRP1(\x2, "x2")) == 0);
  ASSERT(ustrp_spn_rev(sp1, USTRP1(\x3, "x2y")) == 18);
  ASSERT(ustrp_spn_rev(sp1, USTRP1(\x2, "2y")) == 13);
  ASSERT(ustrp_spn_rev(sp1, USTRP1(\x1, "y")) == 13);
  ASSERT(ustrp_spn_chr_rev(sp1, 'x') == 0);
  ASSERT(ustrp_spn_chr_rev(sp1, 'y') == 13);
  ASSERT(ustrp_spn_cstr_rev(sp1, "yx") == 16);
  ASSERT(ustrp_spn_cstr_rev(sp1, "xy") == 16);

  /* cspn */
  ASSERT(ustrp_cspn_chr_fwd(sp1, 'y') == 5);
  ASSERT(ustrp_cspn_chr_fwd(sp1, 'x') == 2);
  ASSERT(ustrp_cspn_chr_fwd(sp1, '2') == 0);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "2") == 0);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "y") == 5);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "22") == 0);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "x") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "xx") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "yx") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "xy") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "yz") == 5);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "zy") == 5);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x1, "2")) == 0);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x1, "y")) == 5);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x2, "22")) == 0);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x1, "x")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x2, "xx")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x2, "yx")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x2, "xy")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x2, "yz")) == 5);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x2, "zy")) == 5);
  ASSERT(ustrp_cspn_cstr_rev(sp1, "2x") == 13);
  ASSERT(ustrp_cspn_cstr_rev(sp1, "x2") == 13);
  ASSERT(ustrp_cspn_chr_rev(sp1, '2') == 16);
  ASSERT(ustrp_cspn_cstr_rev(sp1, "2") == 16);
  ASSERT(ustrp_cspn_cstr_rev(sp1, "2a") == 16);
  ASSERT(ustrp_cspn_rev(sp1, USTRP1(\x2, "2x")) == 13);
  ASSERT(ustrp_cspn_rev(sp1, USTRP1(\x2, "x2")) == 13);
  ASSERT(ustrp_cspn_rev(sp1, USTRP1(\x1, "2")) == 16);
  ASSERT(ustrp_cspn_rev(sp1, USTRP1(\x2, "2a")) == 16);
  ASSERT(ustrp_cspn_chr_fwd(sp1, 'a') == 18);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "a") == 18);
  ASSERT(ustrp_cspn_cstr_rev(sp1, "a") == 18);
  ASSERT(ustrp_cspn_chr_rev(sp1, 'a') == 18);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x1, "a")) == 18);
  ASSERT(ustrp_cspn_rev(sp1, USTRP1(\x1, "a")) == 18);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, "abc") == 18);
  ASSERT(ustrp_cspn_cstr_rev(sp1, "abc") == 18);
  ASSERT(ustrp_cspn_fwd(sp1, USTRP1(\x3, "abc")) == 18);
  ASSERT(ustrp_cspn_rev(sp1, USTRP1(\x3, "abc")) == 18);
  
  /* srch */
  ASSERT(ustrp_srch_cstr_fwd(sp1, "xy") == 5);
  ASSERT(ustrp_srch_cstr_rev(sp1, "xy") == 5);
  ASSERT(ustrp_srch_fwd(sp1, USTRP1(\x2, "xy")) == 5);
  ASSERT(ustrp_srch_rev(sp1, USTRP1(\x2, "xy")) == 5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, USTRP1(\x2, "xy"), 1, 2) ==  5);
  ASSERT(ustrp_srch_subustrp_rev(sp1, USTRP1(\x2, "xy"), 1, 2) ==  5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, USTRP1(\x2, "xy"), 1, 1) ==  3);
  ASSERT(ustrp_srch_subustrp_rev(sp1, USTRP1(\x2, "xy"), 1, 1) ==  5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, USTRP1(\x2, "xy"), 2, 1) ==  6);
  ASSERT(ustrp_srch_subustrp_rev(sp1, USTRP1(\x2, "xy"), 2, 1) == 18);
  ASSERT(ustrp_srch_chr_fwd(sp1, 'x') ==  3);
  ASSERT(ustrp_srch_chr_rev(sp1, 'x') ==  5);
  ASSERT(ustrp_srch_chr_fwd(sp1, 'y') ==  6);
  ASSERT(ustrp_srch_chr_rev(sp1, 'y') == 18);
  
  /* do some more stuff... */
  ustrp_free(pool, sp1);
  sp1 = USTRP1(\x1, "c");
  
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 1, 1));
  ASSERT_PEQ(sp1, USTRP1(\x2, "cs"));
  ASSERT(ustrp_setf_share(sp1));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "cs2"));
  ustrp_sc_free2(pool, &sp1, ustrp_dupx(pool, 1, 1, 1, 1, USTRP("")));
  ASSERT_PEQ(sp1, USTRP(""));
  ASSERT(ustrp_setf_share(sp1));
  ASSERT(!ustrp_owner(sp1));
  ASSERT( ustrp_shared(sp1));
  ASSERT( ustrp_enomem(sp1));
  ASSERT(!ustrp_reallocx(pool, &sp1, USTR_FALSE));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT( ustrp_owner(sp1));
  ASSERT(!ustrp_shared(sp1));
  ASSERT( ustrp_enomem(sp1));
  ASSERT( ustrp_reallocx(pool, &sp1, USTR_FALSE));
  ASSERT_PEQ(sp1, USTRP1(\x1, "2"));
  
  ustrp_sc_free2(pool, &sp1, USTRP(""));

  /* test subpool API */
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(!((struct Ustr__pool_si_base *)pool)->sbeg);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->next);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->prev);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->base);
#endif
  
  ASSERT((p1 = ustr_pool_make_subpool(pool)));
  ASSERT((p2 = ustr_pool_make_subpool(pool)));
  ASSERT((p3 = ustr_pool_make_subpool(pool)));

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(&((struct Ustr__pool_si_base *)pool)->sbeg->cbs == p3);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->next);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->prev);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->base);

  ASSERT(!((struct Ustr__pool_si_base *)p3)->sbeg);
  ASSERT(&((struct Ustr__pool_si_base *)p3)->next->cbs == p2);
  ASSERT(!((struct Ustr__pool_si_base *)p3)->prev);
  ASSERT(&((struct Ustr__pool_si_base *)p3)->base->cbs == pool);
  ASSERT(!((struct Ustr__pool_si_base *)p2)->sbeg);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->next->cbs == p1);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->prev->cbs == p3);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->base->cbs == pool);
  ASSERT(!((struct Ustr__pool_si_base *)p1)->sbeg);
  ASSERT(!((struct Ustr__pool_si_base *)p1)->next);
  ASSERT(&((struct Ustr__pool_si_base *)p1)->prev->cbs == p2);
  ASSERT(&((struct Ustr__pool_si_base *)p1)->base->cbs == pool);
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
  ASSERT( ((struct Ustr__pool_si_base *)p1)->beg);
  ASSERT( ((struct Ustr__pool_si_base *)p2)->beg);
  ASSERT( ((struct Ustr__pool_si_base *)p3)->beg);
#endif
  
  ustr_pool_clear(p2);

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT( ((struct Ustr__pool_si_base *)p1)->beg);
  ASSERT(!((struct Ustr__pool_si_base *)p2)->beg);
  ASSERT( ((struct Ustr__pool_si_base *)p3)->beg);
#endif
  
  ustr_pool_free(p3);
  
  ASSERT((p3 = ustr_pool_make_subpool(p2)));
  
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(!((struct Ustr__pool_si_base *)p1)->next);
  ASSERT(&((struct Ustr__pool_si_base *)p1)->prev->cbs == p2);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->next->cbs == p1);
  ASSERT(!((struct Ustr__pool_si_base *)p2)->prev);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->sbeg->cbs == p3);
  ASSERT(!((struct Ustr__pool_si_base *)p3)->next);
  ASSERT(!((struct Ustr__pool_si_base *)p3)->prev);
  ASSERT(&((struct Ustr__pool_si_base *)p3)->base->cbs == p2);
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
  ASSERT(p1 && ((struct Ustr__pool_si_base *)p1)->beg &&
         ((struct Ustr__pool_si_base *)p1)->beg->ptr);
  ASSERT(p2 && ((struct Ustr__pool_si_base *)p2)->beg &&
         ((struct Ustr__pool_si_base *)p2)->beg->ptr);
  ASSERT(p3 && ((struct Ustr__pool_si_base *)p3)->beg &&
         ((struct Ustr__pool_si_base *)p3)->beg->ptr);
#endif
  
  ustr_pool_clear(pool);

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(p1 && !((struct Ustr__pool_si_base *)p1)->beg);
  ASSERT(p2 && !((struct Ustr__pool_si_base *)p2)->beg);
  ASSERT(p3 && !((struct Ustr__pool_si_base *)p3)->beg);
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
  ASSERT(ustrp_size_alloc(sp1) == ustrp_len(sp1) + ustrp_overhead(sp1));
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
  
  ustr_pool_free(pool);
  ustr_pool_free(NULL);
  
  return (EXIT_SUCCESS);
}
