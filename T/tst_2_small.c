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
  
  ASSERT( ustr_srch_fwd(s1, 0, s1) == 0);
  ASSERT( ustr_srch_fwd(s1, 0, s2) == 0);
  ASSERT( ustr_srch_fwd(s2, 0, s1) == 1);
  ASSERT( ustr_srch_fwd(s2, 0, s2) == 1);
  ASSERT( ustr_srch_rev(s1, 0, s1) == 0);
  ASSERT( ustr_srch_rev(s1, 0, s2) == 0);
  ASSERT( ustr_srch_rev(s2, 0, s1) == 2);
  ASSERT( ustr_srch_rev(s2, 0, s2) == 1);
  
  ASSERT(!ustr_srch_cstr_fwd(s1, 0, "x"));
  ASSERT(!ustr_srch_cstr_fwd(s2, 0, "x"));
  ASSERT(!ustr_srch_cstr_fwd(s2, 0, "x"));
  ASSERT( ustr_srch_cstr_fwd(s1, 0, "") == 0);
  ASSERT( ustr_srch_cstr_fwd(s1, 0, "") == 0);
  ASSERT( ustr_srch_cstr_fwd(s2, 0, "s2") == 1);
  ASSERT( ustr_srch_cstr_fwd(s2, 0, "s")  == 1);
  ASSERT( ustr_srch_cstr_fwd(s2, 0, "2")  == 2);
  ASSERT( ustr_srch_cstr_fwd(s2, 1, "2")  == 2);
  ASSERT( ustr_srch_cstr_fwd(s2, 1, "s2") == 0);
  
  ASSERT(!ustr_srch_cstr_rev(s1, 0, "x"));
  ASSERT(!ustr_srch_cstr_rev(s2, 0, "x"));
  ASSERT(!ustr_srch_cstr_rev(s2, 0, "x"));
  ASSERT( ustr_srch_cstr_rev(s1, 0, "") == 0);
  ASSERT( ustr_srch_cstr_rev(s1, 0, "") == 0);
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
    size_t esz;
    size_t ref;
    int exact;
    size_t lenn;
    size_t refc;

    ustr_conf(s4, NULL,NULL,NULL, NULL,NULL);
    ustr_conf(s4, &esz,&ref,&exact, &lenn,&refc);

    ASSERT(!esz);
    ASSERT(ref == 1);
    ASSERT(!exact);
    
    ASSERT(lenn == 1);
    ASSERT(refc == 1);
    
    ASSERT(ustr_len(s4)  ==  1);
    ASSERT(ustr_size(s4) ==  2);
    ustr_sc_free(&s4);
  }

  ustr_sc_free2(&s1, USTR1(\x12, "22xxxyyyyyyyyyyyyy"));

  /* copy and paste between here and ctst_98_pool.c */
  
  /* test some of the data ... */

  /* spn */
  ASSERT_EQ(s1, USTR1(\x12, "22xxxyyyyyyyyyyyyy"));
  ASSERT(ustr_spn_chr_fwd(s1, 0, 'x') == 0);
  ASSERT(ustr_spn_chr_fwd(s1, 0, '2') == 2);
  ASSERT(ustr_spn_cstr_fwd(s1, 0, "2") == 2);
  ASSERT(ustr_spn_cstr_fwd(s1, 0, "22") == 2);
  ASSERT(ustr_spn_cstr_fwd(s1, 0, "2x") == 5);
  ASSERT(ustr_spn_cstr_fwd(s1, 0, "x2") == 5);
  ASSERT(ustr_spn_cstr_fwd(s1, 0, "x2y") == 18);
  ASSERT(ustr_spn_fwd(s1, 0, USTR1(\x2, "22")) == 2);
  ASSERT(ustr_spn_fwd(s1, 0, USTR1(\x2, "2x")) == 5);
  ASSERT(ustr_spn_fwd(s1, 0, USTR1(\x2, "x2")) == 5);
  ASSERT(ustr_spn_fwd(s1, 0, USTR1(\x3, "x2y")) == 18);
  ASSERT(ustr_spn_cstr_rev(s1, 0, "x2") == 0);
  ASSERT(ustr_spn_cstr_rev(s1, 0, "x2y") == 18);
  ASSERT(ustr_spn_cstr_rev(s1, 0, "2y") == 13);
  ASSERT(ustr_spn_cstr_rev(s1, 0, "y") == 13);
  ASSERT(ustr_spn_rev(s1, 0, USTR1(\x2, "x2")) == 0);
  ASSERT(ustr_spn_rev(s1, 0, USTR1(\x3, "x2y")) == 18);
  ASSERT(ustr_spn_rev(s1, 0, USTR1(\x2, "2y")) == 13);
  ASSERT(ustr_spn_rev(s1, 0, USTR1(\x1, "y")) == 13);
  ASSERT(ustr_spn_chr_rev(s1, 0, 'x') == 0);
  ASSERT(ustr_spn_chr_rev(s1, 0, 'y') == 13);
  ASSERT(ustr_spn_cstr_rev(s1, 0, "yx") == 16);
  ASSERT(ustr_spn_cstr_rev(s1, 0, "xy") == 16);

  /* cspn */
  ASSERT_EQ(s1, USTR1(\x12, "22xxxyyyyyyyyyyyyy"));
  ASSERT(ustr_cspn_chr_fwd(s1, 0, 'y') == 5);
  ASSERT(ustr_cspn_chr_fwd(s1, 0, 'x') == 2);
  ASSERT(ustr_cspn_chr_fwd(s1, 0, '2') == 0);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "2") == 0);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "y") == 5);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "22") == 0);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "x") == 2);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "xx") == 2);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "yx") == 2);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "xy") == 2);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "yz") == 5);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "zy") == 5);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x1, "2")) == 0);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x1, "y")) == 5);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x2, "22")) == 0);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x1, "x")) == 2);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x2, "xx")) == 2);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x2, "yx")) == 2);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x2, "xy")) == 2);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x2, "yz")) == 5);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x2, "zy")) == 5);
  ASSERT(ustr_cspn_cstr_rev(s1, 0, "2x") == 13);
  ASSERT(ustr_cspn_cstr_rev(s1, 0, "x2") == 13);
  ASSERT(ustr_cspn_chr_rev(s1, 0, '2') == 16);
  ASSERT(ustr_cspn_cstr_rev(s1, 0, "2") == 16);
  ASSERT(ustr_cspn_cstr_rev(s1, 0, "2a") == 16);
  ASSERT(ustr_cspn_rev(s1, 0, USTR1(\x2, "2x")) == 13);
  ASSERT(ustr_cspn_rev(s1, 0, USTR1(\x2, "x2")) == 13);
  ASSERT(ustr_cspn_rev(s1, 0, USTR1(\x1, "2")) == 16);
  ASSERT(ustr_cspn_rev(s1, 0, USTR1(\x2, "2a")) == 16);
  ASSERT(ustr_cspn_chr_fwd(s1, 0, 'a') == 18);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "a") == 18);
  ASSERT(ustr_cspn_cstr_rev(s1, 0, "a") == 18);
  ASSERT(ustr_cspn_chr_rev(s1, 0, 'a') == 18);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x1, "a")) == 18);
  ASSERT(ustr_cspn_rev(s1, 0, USTR1(\x1, "a")) == 18);
  ASSERT(ustr_cspn_cstr_fwd(s1, 0, "abc") == 18);
  ASSERT(ustr_cspn_cstr_rev(s1, 0, "abc") == 18);
  ASSERT(ustr_cspn_fwd(s1, 0, USTR1(\x3, "abc")) == 18);
  ASSERT(ustr_cspn_rev(s1, 0, USTR1(\x3, "abc")) == 18);
  
  /* srch */
  ASSERT_EQ(s1, USTR1(\x12, "22xxxyyyyyyyyyyyyy"));
  ASSERT(ustr_srch_cstr_fwd(s1, 0, "xy") == 5);
  ASSERT(ustr_srch_cstr_rev(s1, 0, "xy") == 5);
  ASSERT(ustr_srch_fwd(s1, 0, USTR1(\x2, "xy")) == 5);
  ASSERT(ustr_srch_rev(s1, 0, USTR1(\x2, "xy")) == 5);
  ASSERT(ustr_srch_subustr_fwd(s1, 0, USTR1(\x2, "xy"), 1, 2) ==  5);
  ASSERT(ustr_srch_subustr_rev(s1, 0, USTR1(\x2, "xy"), 1, 2) ==  5);
  ASSERT(ustr_srch_subustr_fwd(s1, 0, USTR1(\x2, "xy"), 1, 1) ==  3);
  ASSERT(ustr_srch_subustr_rev(s1, 0, USTR1(\x2, "xy"), 1, 1) ==  5);
  ASSERT(ustr_srch_subustr_fwd(s1, 0, USTR1(\x2, "xy"), 2, 1) ==  6);
  ASSERT(ustr_srch_subustr_rev(s1, 0, USTR1(\x2, "xy"), 2, 1) == 18);
  ASSERT(ustr_srch_chr_fwd(s1, 0, 'x') ==  3);
  ASSERT(ustr_srch_chr_rev(s1, 0, 'x') ==  5);
  ASSERT(ustr_srch_chr_fwd(s1, 0, 'y') ==  6);
  ASSERT(ustr_srch_chr_rev(s1, 0, 'y') == 18);

  /* srch_case */
  ASSERT_EQ(s1, USTR1(\x12, "22xxxyyyyyyyyyyyyy"));
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "Xy") == 5);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "xY") == 5);
  ASSERT(ustr_srch_case_fwd(s1, 0, USTR1(\x2, "xY")) == 5);
  ASSERT(ustr_srch_case_rev(s1, 0, USTR1(\x2, "Xy")) == 5);
  ASSERT(ustr_srch_case_subustr_fwd(s1, 0, USTR1(\x2, "Xy"), 1, 2) ==  5);
  ASSERT(ustr_srch_case_subustr_rev(s1, 0, USTR1(\x2, "xY"), 1, 2) ==  5);
  ASSERT(ustr_srch_case_subustr_fwd(s1, 0, USTR1(\x2, "Xy"), 1, 1) ==  3);
  ASSERT(ustr_srch_case_subustr_rev(s1, 0, USTR1(\x2, "xY"), 1, 1) ==  5);
  ASSERT(ustr_srch_case_subustr_fwd(s1, 0, USTR1(\x2, "XY"), 2, 1) ==  6);
  ASSERT(ustr_srch_case_subustr_rev(s1, 0, USTR1(\x2, "XY"), 2, 1) == 18);

  if (!USTR_DEBUG)
  ASSERT(ustr_srch_case_subustr_fwd(s1, 0, USTR1(\4, "YYYY"), 4, 2) ==  1);
  if (!USTR_DEBUG)
  ASSERT(ustr_srch_case_subustr_rev(s1, 0, USTR1(\4, "1248"), 4, 2) == 18);
  if (!USTR_DEBUG)
  ASSERT(ustr_srch_case_subustr_fwd(s1, 2, USTR1(\4, "yyyy"), 5, 1) ==  3);
  if (!USTR_DEBUG)
  ASSERT(ustr_srch_case_subustr_rev(s1, 2, USTR1(\4, "1248"), 5, 1) == 16);

  ASSERT(ustr_srch_case_chr_fwd(s1, 0, 'x') ==  3);
  ASSERT(ustr_srch_case_chr_rev(s1, 0, 'x') ==  5);
  ASSERT(ustr_srch_case_chr_fwd(s1, 0, 'y') ==  6);
  ASSERT(ustr_srch_case_chr_rev(s1, 0, 'y') == 18);
  ASSERT(ustr_srch_case_chr_fwd(s1, 0, 'X') ==  3);
  ASSERT(ustr_srch_case_chr_rev(s1, 0, 'X') ==  5);
  ASSERT(ustr_srch_case_chr_fwd(s1, 0, 'Y') ==  6);
  ASSERT(ustr_srch_case_chr_rev(s1, 0, 'Y') == 18);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'x', 1) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'x', 1) ==  5);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y', 1) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y', 1) == 18);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'X', 1) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'X', 1) ==  5);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y', 1) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y', 1) == 18);

  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'z', 1) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'z', 1) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Z', 1) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Z', 1) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'z', 2) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'z', 2) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Z', 2) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Z', 2) ==  0);

  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Z', 0) ==  1);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Z', 0) == 18);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 1, 'Z', 0) ==  2);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 1, 'Z', 0) == 17);

  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'x',  2) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'x',  2) ==  4);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'x',  3) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'x',  3) ==  3);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'x',  4) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'x',  4) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'X',  2) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'X',  2) ==  4);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'X',  3) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'X',  3) ==  3);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'X',  4) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'X',  4) ==  0);

  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "xx")   ==  3);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "xx")   ==  4);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "xxx")  ==  3);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "xxx")  ==  3);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "xxxx") ==  0);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "xxxx") ==  0);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "XX")   ==  3);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "XX")   ==  4);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "XXX")  ==  3);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "XXX")  ==  3);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "XXXX") ==  0);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "XXXX") ==  0);

  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y',  2) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y',  2) == 17);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y',  3) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y',  3) == 16);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y',  4) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y',  4) == 15);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y', 13) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y', 13) ==  6);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y', 14) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y', 14) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y',  2) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y',  2) == 17);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y',  3) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y',  3) == 16);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y',  4) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y',  4) == 15);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y', 13) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y', 13) ==  6);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y', 14) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y', 14) ==  0);
  
  /* try srch_case with other case */
  ustr_sc_free2(&s1, USTR1(\x12, "22XXXYYYYYYYYYYYYY"));
  ASSERT_EQ(s1, USTR1(\x12, "22XXXYYYYYYYYYYYYY"));
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "Xy") == 5);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "xY") == 5);
  ASSERT(ustr_srch_case_fwd(s1, 0, USTR1(\x2, "xY")) == 5);
  ASSERT(ustr_srch_case_rev(s1, 0, USTR1(\x2, "Xy")) == 5);
  ASSERT(ustr_srch_case_subustr_fwd(s1, 0, USTR1(\x2, "Xy"), 1, 2) ==  5);
  ASSERT(ustr_srch_case_subustr_rev(s1, 0, USTR1(\x2, "xY"), 1, 2) ==  5);
  ASSERT(ustr_srch_case_subustr_fwd(s1, 0, USTR1(\x2, "Xy"), 1, 1) ==  3);
  ASSERT(ustr_srch_case_subustr_rev(s1, 0, USTR1(\x2, "xY"), 1, 1) ==  5);
  ASSERT(ustr_srch_case_subustr_fwd(s1, 0, USTR1(\x2, "XY"), 2, 1) ==  6);
  ASSERT(ustr_srch_case_subustr_rev(s1, 0, USTR1(\x2, "XY"), 2, 1) == 18);
  
  ASSERT(ustr_srch_case_chr_fwd(s1, 0, 'x') ==  3);
  ASSERT(ustr_srch_case_chr_rev(s1, 0, 'x') ==  5);
  ASSERT(ustr_srch_case_chr_fwd(s1, 0, 'y') ==  6);
  ASSERT(ustr_srch_case_chr_rev(s1, 0, 'y') == 18);
  ASSERT(ustr_srch_case_chr_fwd(s1, 0, 'X') ==  3);
  ASSERT(ustr_srch_case_chr_rev(s1, 0, 'X') ==  5);
  ASSERT(ustr_srch_case_chr_fwd(s1, 0, 'Y') ==  6);
  ASSERT(ustr_srch_case_chr_rev(s1, 0, 'Y') == 18);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'x', 1) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'x', 1) ==  5);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y', 1) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y', 1) == 18);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'X', 1) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'X', 1) ==  5);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y', 1) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y', 1) == 18);
  
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'z', 1) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'z', 1) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Z', 1) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Z', 1) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'z', 2) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'z', 2) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Z', 2) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Z', 2) ==  0);

  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Z', 0) ==  1);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Z', 0) == 18);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 1, 'Z', 0) ==  2);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 1, 'Z', 0) == 17);

  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'x',  2) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'x',  2) ==  4);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'x',  3) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'x',  3) ==  3);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'x',  4) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'x',  4) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'X',  2) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'X',  2) ==  4);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'X',  3) ==  3);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'X',  3) ==  3);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'X',  4) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'X',  4) ==  0);

  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "xx")   ==  3);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "xx")   ==  4);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "xxx")  ==  3);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "xxx")  ==  3);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "xxxx") ==  0);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "xxxx") ==  0);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "XX")   ==  3);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "XX")   ==  4);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "XXX")  ==  3);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "XXX")  ==  3);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "XXXX") ==  0);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "XXXX") ==  0);

  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y',  2) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y',  2) == 17);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y',  3) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y',  3) == 16);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y',  4) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y',  4) == 15);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y', 13) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y', 13) ==  6);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'y', 14) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'y', 14) ==  0);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y',  2) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y',  2) == 17);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y',  3) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y',  3) == 16);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y',  4) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y',  4) == 15);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y', 13) ==  6);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y', 13) ==  6);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 0, 'Y', 14) ==  0);
  ASSERT(ustr_srch_case_rep_chr_rev(s1, 0, 'Y', 14) ==  0);
  
  /* show srch still works this way */
  ASSERT(ustr_srch_cstr_fwd(s1, 0, "XY") == 5);
  ASSERT(ustr_srch_cstr_rev(s1, 0, "XY") == 5);
  ASSERT(ustr_srch_fwd(s1, 0, USTR1(\x2, "XY")) == 5);
  ASSERT(ustr_srch_rev(s1, 0, USTR1(\x2, "XY")) == 5);
  ASSERT(ustr_srch_subustr_fwd(s1, 0, USTR1(\x2, "XY"), 1, 2) ==  5);
  ASSERT(ustr_srch_subustr_rev(s1, 0, USTR1(\x2, "XY"), 1, 2) ==  5);
  ASSERT(ustr_srch_subustr_fwd(s1, 0, USTR1(\x2, "XY"), 1, 1) ==  3);
  ASSERT(ustr_srch_subustr_rev(s1, 0, USTR1(\x2, "XY"), 1, 1) ==  5);
  ASSERT(ustr_srch_subustr_fwd(s1, 0, USTR1(\x2, "XY"), 2, 1) ==  6);
  ASSERT(ustr_srch_subustr_rev(s1, 0, USTR1(\x2, "XY"), 2, 1) == 18);
  ASSERT(ustr_srch_chr_fwd(s1, 0, 'X') ==  3);
  ASSERT(ustr_srch_chr_rev(s1, 0, 'X') ==  5);
  ASSERT(ustr_srch_chr_fwd(s1, 0, 'Y') ==  6);
  ASSERT(ustr_srch_chr_rev(s1, 0, 'Y') == 18);
  
  ASSERT(ustr_srch_cstr_fwd(s1, 0, "") == 1);
  ASSERT(ustr_srch_cstr_rev(s1, 0, "") == ustr_len(s1));
  ASSERT(ustr_srch_case_cstr_fwd(s1, 0, "") == 1);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "") == ustr_len(s1));

  ASSERT(ustr_srch_cstr_fwd(s1, 2, "") == 3);
  ASSERT(ustr_srch_cstr_rev(s1, 2, "") == ustr_len(s1) - 2);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 2, "") == 3);
  ASSERT(ustr_srch_case_cstr_rev(s1, 2, "") == ustr_len(s1) - 2);
  
  return (EXIT_SUCCESS);
}
