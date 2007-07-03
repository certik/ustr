#undef  USTR_CONF_REF_BYTES
#define USTR_CONF_REF_BYTES 1
#define USTR_CONF_USE_EOS_MARK 0
#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  unsigned int num = 254;
  unsigned int i;
  struct Ustr *tmp = NULL;
  
  ASSERT(ustr_len(s1) == 0);
  ASSERT(ustr_len(s2) == strlen("s2"));

  ASSERT(ustr_size(s1) == 0);
  ASSERT(ustr_size(s2) == 2); /* ustr__ns(1 + 1 + 1 + 2 + 1) -
                               *          1 + 1 + 1     + 1 */

  ASSERT(ustr_len(s2) == strlen("s2"));
  
  ASSERT( ustr_cmp_eq(s1, s1));
  ASSERT( ustr_cmp_eq(s2, s2));
  ASSERT(!ustr_cmp_eq(s1, s2));
  ASSERT( ustr_cmp_cstr_eq(s1, ""));
  ASSERT( ustr_cmp_cstr_eq(s2, "s2"));
  ASSERT( ustr_cmp_eq(s2, USTR1(\x02, "s2")));
  ASSERT(!ustr_cmp(   s2, USTR1(\x02, "s2")));
  
  ASSERT(!ustr_cmp_cstr_eq(s1, "s2"));
  ASSERT(!ustr_cmp_cstr_eq(s2, ""));
  ASSERT(!ustr_cmp_cstr_eq(s2, "s3"));
  ASSERT(!ustr_cmp_cstr_eq(s2, "s2x"));

  ASSERT(!ustr_cmp_buf_eq(s2, "s2", 1));
  ASSERT( ustr_cmp_buf_eq(s2, "s2", 2));
  ASSERT(!ustr_cmp_buf_eq(s2, "s2", 3));

  ASSERT( ustr_cmp(s1, s1)          == 0);
  ASSERT( ustr_cmp(s2, s2)          == 0);
  ASSERT( ustr_cmp(s1, s2)          <  0);
  ASSERT( ustr_cmp_fast(s1, s2)     <  0);
  ASSERT( ustr_cmp(s2, s1)          >  0);
  ASSERT( ustr_cmp_fast(s2, s1)     >  0);
  ASSERT( ustr_cmp_cstr(s2, "s2")   == 0);
  ASSERT( ustr_cmp_buf(s2, "s2", 2) == 0);
  
  ASSERT( ustr_srch_fwd(s1, 0, s1) == 1);
  ASSERT( ustr_srch_fwd(s1, 0, s2) == 0);
  ASSERT( ustr_srch_fwd(s2, 0, s1) == 1);
  ASSERT( ustr_srch_fwd(s2, 0, s2) == 1);
  ASSERT( ustr_srch_rev(s1, 0, s1) == 1);
  ASSERT( ustr_srch_rev(s1, 0, s2) == 0);
  ASSERT( ustr_srch_rev(s2, 0, s1) == 2);
  ASSERT( ustr_srch_rev(s2, 0, s2) == 1);
  
  ASSERT(!ustr_srch_cstr_fwd(s1, 0, "x"));
  ASSERT(!ustr_srch_cstr_fwd(s2, 0, "x"));
  ASSERT(!ustr_srch_cstr_fwd(s2, 0, "x"));
  ASSERT( ustr_srch_cstr_fwd(s1, 0, "") == 1);
  ASSERT( ustr_srch_cstr_fwd(s1, 0, "") == 1);
  ASSERT( ustr_srch_cstr_fwd(s2, 0, "s2") == 1);
  ASSERT( ustr_srch_cstr_fwd(s2, 0, "s")  == 1);
  ASSERT( ustr_srch_cstr_fwd(s2, 0, "2")  == 2);
  ASSERT( ustr_srch_cstr_fwd(s2, 1, "2")  == 1);
  
  ASSERT(!ustr_srch_cstr_rev(s1, 0, "x"));
  ASSERT(!ustr_srch_cstr_rev(s2, 0, "x"));
  ASSERT(!ustr_srch_cstr_rev(s2, 0, "x"));
  ASSERT( ustr_srch_cstr_rev(s1, 0, "") == 1);
  ASSERT( ustr_srch_cstr_rev(s1, 0, "") == 1);
  ASSERT( ustr_srch_cstr_rev(s2, 0, "s2") == 1);
  ASSERT( ustr_srch_cstr_rev(s2, 0, "s")  == 1);
  ASSERT( ustr_srch_cstr_rev(s2, 0, "2")  == 2);

  for (i = 0; i < num; ++i)
  {
    tmp = ustr_dup(s1);
    ASSERT(tmp == s1);
    tmp = ustr_dup(s2);
    ASSERT(tmp == s2);
  }
  ASSERT(ustr_sc_dup(&s2));
  ASSERT(tmp && (tmp != s2));
  ASSERT(ustr_cmp_eq(tmp, s2));
  ustr_sc_free(&s2);
  s2 = tmp;
  
  for (i = 0; i < num; ++i)
  {
    ustr_free(s1);
    ustr_free(s1);
    ustr_free(s2);
  }
  ustr_sc_free(&s2);

  {
    struct Ustr *s4 = ustr_dup_cstr("x");
    
    ASSERT(ustr_len(s4)  ==  1);
    ASSERT(ustr_size(s4) ==  2);
    ustr_sc_free(&s4);
  }
  
  return (EXIT_SUCCESS);
}
