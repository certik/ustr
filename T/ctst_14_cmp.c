#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make();
  Ustrp *sp1 = USTRP1(\x4, "abcd");
  Ustrp *sp2 = USTRP1(\x4, ".xyz");
  Ustrp *sp3 = USTRP1(\x8, "abcd.xyz");

  ASSERT(ustrp_cmp_prefix_eq(sp3, sp3));
  ASSERT(ustrp_cmp_prefix_eq(sp3, sp1));
  ASSERT(ustrp_cmp_suffix_eq(sp3, sp2));
  ASSERT(ustrp_cmp_suffix_eq(sp3, sp3));

  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "a"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "ab"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abc"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd."));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd.x"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd.xy"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "abcd.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "bcd.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "cd.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "d.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, ".xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "yz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "z"));

  ASSERT(ustrp_cmp(sp3, sp1) ==  1);
  ASSERT(ustrp_cmp(sp1, sp3) == -1);
  
  ASSERT(ustrp_cmp_subustrp(sp3, sp1, 1, 8) ==  1);
  ASSERT(ustrp_cmp_fast_subustrp(sp3, sp1, 1, 8) ==  1);
  
  ustr_pool_free(pool);

  return (EXIT_SUCCESS);
}
