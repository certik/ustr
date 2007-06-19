#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make_pool();
  Ustrp *sp1 = USTRP1(\x4, "abcd");
  Ustrp *sp2 = USTRP1(\x4, ".xyz");
  Ustrp *sp3 = USTRP1(\x8, "abcd.xyz");

  ASSERT(!ustrp_cmp_cstr(sp1, "abcd"));
  ASSERT( ustrp_cmp_cstr(sp2, "abcd") < 0);
  ASSERT( ustrp_cmp_cstr(sp3, "abcd") > 0);
  ASSERT( ustrp_cmp_cstr(sp1, ".xyz") > 0);
  ASSERT(!ustrp_cmp_cstr(sp2, ".xyz"));
  ASSERT( ustrp_cmp_cstr(sp3, ".xyz") > 0);
  ASSERT( ustrp_cmp_cstr(sp1, "abcd.xyz") < 0);
  ASSERT( ustrp_cmp_cstr(sp2, "abcd.xyz") < 0);
  ASSERT(!ustrp_cmp_cstr(sp3, "abcd.xyz"));
  
  ASSERT(ustrp_cmp_buf_eq(sp1, "abcd.xyz", 4));

  ASSERT(ustrp_cmp_cstr_eq(sp1, ustrp_cstr(USTRP1(\x4, "abcd"))));
  ASSERT(ustrp_sc_ensure_owner(pool, &sp1));
  ASSERT(ustrp_cmp_cstr_eq(sp1, ustrp_wstr(sp1)));
  
  ASSERT(ustrp_cmp_cstr_eq(sp1, "abcd"));
  ASSERT(ustrp_cmp_cstr_eq(sp2, ".xyz"));
  ASSERT(ustrp_cmp_cstr_eq(sp3, "abcd.xyz"));
  
  ASSERT(!ustrp_cmp_fast_cstr(sp1, "abcd"));
  ASSERT(!ustrp_cmp_fast_cstr(sp2, ".xyz"));
  ASSERT(!ustrp_cmp_fast_cstr(sp3, "abcd.xyz"));
  
  ASSERT(ustrp_cmp_prefix_eq(sp3, sp3));
  ASSERT(ustrp_cmp_prefix_eq(sp3, sp1));
  ASSERT(ustrp_cmp_suffix_eq(sp3, sp2));
  ASSERT(ustrp_cmp_suffix_eq(sp3, sp3));

  ASSERT(!ustrp_cmp_prefix_cstr_eq(sp3, "z"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "a"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "ab"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abc"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd."));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd.x"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd.xy"));
  ASSERT(ustrp_cmp_prefix_cstr_eq(sp3, "abcd.xyz"));
  ASSERT(!ustrp_cmp_prefix_cstr_eq(sp3, "abcd.xyzz"));
  ASSERT(!ustrp_cmp_prefix_cstr_eq(sp3, "aabcd.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "abcd.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "bcd.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "cd.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "d.xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, ".xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "xyz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "yz"));
  ASSERT(ustrp_cmp_suffix_cstr_eq(sp3, "z"));
  ASSERT(!ustrp_cmp_suffix_cstr_eq(sp3, "a"));

  ASSERT(!ustr_cmp_prefix_cstr_eq(s1, "a"));
  ASSERT( ustr_cmp_prefix_cstr_eq(s1, ""));
  ASSERT(!ustr_cmp_suffix_cstr_eq(s1, "a"));
  ASSERT( ustr_cmp_suffix_cstr_eq(s1, ""));
  
  ASSERT(!ustr_cmp_prefix_cstr_eq(s2, "z"));
  ASSERT( ustr_cmp_prefix_cstr_eq(s2, ""));
  ASSERT( ustr_cmp_prefix_cstr_eq(s2, "s"));
  ASSERT( ustr_cmp_prefix_cstr_eq(s2, "s2"));
  ASSERT(!ustr_cmp_prefix_cstr_eq(s2, "s22"));
  ASSERT(!ustr_cmp_prefix_cstr_eq(s2, "ss2"));
  ASSERT( ustr_cmp_suffix_cstr_eq(s2, "s2"));
  ASSERT( ustr_cmp_suffix_cstr_eq(s2, "2"));
  ASSERT( ustr_cmp_suffix_cstr_eq(s2, ""));
  ASSERT(!ustr_cmp_suffix_cstr_eq(s2, "z"));
  
  ASSERT(!ustr_cmp_prefix_eq(s1, s2));
  ASSERT( ustr_cmp_prefix_eq(s2, s1));
  ASSERT(!ustr_cmp_suffix_eq(s1, s2));
  ASSERT( ustr_cmp_suffix_eq(s2, s1));
  
  ASSERT(ustrp_cmp(sp3, sp1) ==  1);
  ASSERT(ustrp_cmp(sp1, sp3) == -1);

  if (!USTR_DEBUG)
  ASSERT(ustrp_cmp_subustrp(sp3, sp1, 1, 8) ==  1);
  if (!USTR_DEBUG)
  ASSERT(ustrp_cmp_fast_subustrp(sp3, sp1, 1, 8) ==  1);
  ASSERT(ustrp_cmp_subustrp_eq(sp1, sp3, 1, 4));
  
  ustr_pool_free(pool);

  return (EXIT_SUCCESS);
}
