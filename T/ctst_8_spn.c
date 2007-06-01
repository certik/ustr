#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr *s3 = ustr_dup_cstr("abcdefg xxxx xxxx xxxx xxxx1234yyyy yyyy yyyy zzz");
  Ustr *s4 = NULL;
  Ustr *s5 = NULL;
  
  ASSERT(s3);

  ASSERT(ustr_spn_cstr_chrs_fwd(s3, "abcd") == 4);
  ASSERT(ustr_spn_cstr_chrs_fwd(s3, "abcdefg ") == 8);
  ASSERT(ustr_spn_cstr_chrs_fwd(s3, "abcdefg x") == 27);
  
  ASSERT(ustr_cspn_cstr_chrs_fwd(s3, "x") == 8);
  ASSERT(ustr_cspn_cstr_chrs_fwd(s3, "21") == 27);
  
  ASSERT(ustr_spn_cstr_chrs_rev(s3, "z")  == 3);
  ASSERT(ustr_spn_cstr_chrs_rev(s3, "az") == 3);
  ASSERT(ustr_spn_cstr_chrs_rev(s3, "ayz") == 3);
  ASSERT(ustr_spn_cstr_chrs_rev(s3, "ayz ") == 18);

  ASSERT(ustr_cspn_cstr_chrs_rev(s3, "34") == 18);
  ASSERT(ustr_cspn_cstr_chrs_rev(s3, " ") == 3);
  ASSERT(ustr_cspn_cstr_chrs_rev(s3, "x") == 22);
  
  ASSERT(ustr_spn_cstr_chrs_fwd(s3, " xayzafg1234bcde ") == ustr_len(s3));
  ASSERT(ustr_spn_cstr_chrs_rev(s3, "1234abcdefg xayz ") == ustr_len(s3));

  ASSERT(ustr_cspn_cstr_chrs_fwd(s3, "!")   == ustr_len(s3));
  ASSERT(ustr_cspn_cstr_chrs_rev(s3, "!")   == ustr_len(s3));
  ASSERT(ustr_cspn_cstr_chrs_fwd(s3, "!@#") == ustr_len(s3));
  ASSERT(ustr_cspn_cstr_chrs_rev(s3, "!@#") == ustr_len(s3));

  ASSERT((s4 = ustr_dup_subustr(s3, 1,
                                ustr_spn_cstr_chrs_fwd(s3, "abcdefg "))));
  ASSERT(ustr_del_subustr(&s3, 1, ustr_len(s4)));
  ASSERT((s5 = ustr_dup(s4)));
  ASSERT((s5 = ustr_dup(s4)));
  
  ASSERT(ustr_set_subustr(&s5, s3, 1, ustr_spn_cstr_chrs_fwd(s3, "x ")));
  ASSERT(ustr_del_subustr(&s3, 1, ustr_len(s5)));
  
  ASSERT(ustr_spn_cstr_chrs_fwd(s4, "abcd") == 4);
  ASSERT(ustr_spn_cstr_chrs_fwd(s4, "abcdefg ") == 8);
  
  ASSERT(ustr_spn_cstr_chrs_rev(s3, "z")  == 3);
  ASSERT(ustr_spn_cstr_chrs_rev(s3, "az") == 3);
  ASSERT(ustr_spn_cstr_chrs_rev(s3, "ayz") == 3);
  ASSERT(ustr_spn_cstr_chrs_rev(s3, "ayz ") == 18);

  ASSERT(ustr_cspn_cstr_chrs_rev(s3, "34") == 18);
  ASSERT(ustr_cspn_cstr_chrs_rev(s3, " ") == 3);
  ASSERT(ustr_cspn_cstr_chrs_rev(s3, "x") == 22);
  
  ASSERT(ustr_spn_cstr_chrs_fwd(s3, "1")     == 1);
  ASSERT(ustr_spn_cstr_chrs_fwd(s3, "12")    == 2);
  ASSERT(ustr_spn_cstr_chrs_fwd(s3, "123")   == 3);
  ASSERT(ustr_spn_cstr_chrs_fwd(s3, "1234")  == 4);
  ASSERT(ustr_spn_cstr_chrs_fwd(s3, "1234y") == 8);
  
  ASSERT(ustr_del_subustr(&s3, 4, 4));
  ASSERT(ustr_spn_cstr_chrs_fwd(s3, "1234y") == 4);  
  
  ustr_free(s3);
  ustr_free(s4); /* leaked, see above */
  ustr_free(s4);
  ustr_free(s5);
  
  return (EXIT_SUCCESS);
}
