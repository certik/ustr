#include "tst.h"

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make();
  Ustrp *sp1 = ustrp_dup_empty(pool);
  Ustrp *sp2 = USTRP1(\x2, "s2");
  
  ASSERT(pool);
  ASSERT(ustrp_rw(sp1));
  ASSERT(ustrp_len(sp1) == 0);
  ASSERT((sp1 = ustrp_dup_fmt_lim(pool, 4000, "%.5000x", 0xdeadbeef)));
  ASSERT(ustrp_rw(sp1));
  ASSERT(ustrp_len(sp1) == 4000);
  ASSERT((sp1 = ustrp_dup_fmt(pool,           "%.5000x", 0xdeadbeef)));
  ASSERT(ustrp_len(sp1) == 5000);

  ustrp_sc_del(pool, &sp1);
  
  ustr_pool_clear(pool);
  
  ASSERT(ustrp_len(sp1) == 0);
  ASSERT(ustrp_len(sp2) == 2);
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 1, 1));
  ASSERT(ustrp_len(sp1) == 1);
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 3, 1));
  ASSERT(ustrp_len(sp1) == 1);
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

  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 3, 1)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 3, 1)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 1, 3)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 1, 3)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 2, 2)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 2, 2)));

  ustrp_sc_free2(pool, &sp1, USTRP(""));
  
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 1, 3));
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 3, 1));
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 2, 2));

  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x1, "2"));
  ustrp_sc_free2(pool, &sp1, USTRP1(\x1, "c"));
  
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 1, 1));
  ASSERT_PEQ(sp1, USTRP1(\x2, "cs"));
  ASSERT(ustrp_set_share(sp1));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "cs2"));
  ustrp_sc_free2(pool, &sp1, ustrp_dupx(pool, 0, 1, 0, 1, USTRP("")));
  ASSERT_PEQ(sp1, USTRP(""));
  ASSERT(ustrp_set_share(sp1));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  
  ustrp_sc_free2(pool, &sp1, USTRP(""));
  
  ustr_pool_free(pool);
  
  return (EXIT_SUCCESS);
}
