#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr *s3 = ustr_dup_cstr("abcdefg xxxx xxxx xxxx xxxx1234yyyy yyyy yyyy zzz");
  Ustr *s4 = NULL;
  Ustr *s5 = NULL;
  
  ASSERT(s3);

  ASSERT(ustr_spn_cstr_fwd(s3, 0, "abcd") == 4);
  ASSERT(ustr_spn_cstr_fwd(s3, 0, "abcdefg ") == 8);
  ASSERT(ustr_spn_cstr_fwd(s3, 0, "abcdefg x") == 27);
  
  ASSERT(ustr_cspn_cstr_fwd(s3, 0, "x") == 8);
  ASSERT(ustr_cspn_cstr_fwd(s3, 0, "21") == 27);
  
  ASSERT(ustr_spn_cstr_rev(s3, 0, "z")  == 3);
  ASSERT(ustr_spn_cstr_rev(s3, 0, "az") == 3);
  ASSERT(ustr_spn_cstr_rev(s3, 0, "ayz") == 3);
  ASSERT(ustr_spn_cstr_rev(s3, 0, "ayz ") == 18);

  ASSERT(ustr_cspn_cstr_rev(s3, 0, "34") == 18);
  ASSERT(ustr_cspn_cstr_rev(s3, 0, " ") == 3);
  ASSERT(ustr_cspn_cstr_rev(s3, 0, "x") == 22);
  
  ASSERT(ustr_spn_cstr_fwd(s3, 0, " xayzafg1234bcde ") == ustr_len(s3));
  ASSERT(ustr_spn_cstr_rev(s3, 0, "1234abcdefg xayz ") == ustr_len(s3));

  ASSERT(ustr_cspn_cstr_fwd(s3, 0, "!")   == ustr_len(s3));
  ASSERT(ustr_cspn_cstr_rev(s3, 0, "!")   == ustr_len(s3));
  ASSERT(ustr_cspn_cstr_fwd(s3, 0, "!@#") == ustr_len(s3));
  ASSERT(ustr_cspn_cstr_rev(s3, 0, "!@#") == ustr_len(s3));

  ASSERT((s4 = ustr_dup_subustr(s3, 1,
                                ustr_spn_cstr_fwd(s3, 0, "abcdefg "))));
  ASSERT(ustr_del_subustr(&s3, 1, ustr_len(s4)));
  ASSERT((s5 = ustr_dup(s4)));
  ASSERT((s5 = ustr_dup(s4)));
  
  ASSERT(ustr_set_subustr(&s5, s3, 1, ustr_spn_cstr_fwd(s3, 0, "x ")));
  ASSERT(ustr_del_subustr(&s3, 1, ustr_len(s5)));
  
  ASSERT(ustr_spn_cstr_fwd(s4, 0, "abcd") == 4);
  ASSERT(ustr_spn_cstr_fwd(s4, 0, "abcdefg ") == 8);
  ASSERT(ustr_spn_cstr_fwd(s4, 1, "abcdefg ") == 7);
  ASSERT(ustr_spn_cstr_fwd(s4, 2, "abcdefg ") == 6);
  ASSERT(ustr_spn_cstr_fwd(s4, 3, "abcdefg ") == 5);
  ASSERT(ustr_spn_cstr_fwd(s4, 4, "abcdefg ") == 4);
  ASSERT(ustr_spn_cstr_fwd(s4, 5, "abcdefg ") == 3);
  ASSERT(ustr_spn_cstr_fwd(s4, 6, "abcdefg ") == 2);
  ASSERT(ustr_spn_cstr_fwd(s4, 7, "abcdefg ") == 1);
  ASSERT(ustr_spn_cstr_fwd(s4, 8, "abcdefg ") == 0);
  
  ASSERT(ustr_spn_cstr_rev(s3, 0, "z")  == 3);
  ASSERT(ustr_spn_cstr_rev(s3, 4, "y")  == 4);
  ASSERT(ustr_spn_cstr_rev(s3, 0, "az") == 3);
  ASSERT(ustr_spn_cstr_rev(s3, 0, "ayz") == 3);
  ASSERT(ustr_spn_cstr_rev(s3, 0, "ayz ") == 18);

  ASSERT(ustr_spn_cstr_rev(s3, 0, "y ") ==  0);
  ASSERT(ustr_spn_cstr_rev(s3, 1, "y ") ==  0);
  ASSERT(ustr_spn_cstr_rev(s3, 2, "y ") ==  0);
  ASSERT(ustr_spn_cstr_rev(s3, 3, "y ") == 15);
  ASSERT(ustr_spn_cstr_rev(s3, 4, "y ") == 14);
  ASSERT(ustr_spn_cstr_rev(s3, 5, "y ") == 13);
  ASSERT(ustr_spn_cstr_rev(s3, 6, "y ") == 12);
  ASSERT(ustr_spn_cstr_rev(s3, 7, "y ") == 11);
  ASSERT(ustr_spn_cstr_rev(s3, 8, "y ") == 10);
  ASSERT(ustr_spn_cstr_rev(s3, 9, "y ") ==  9);
  
  ASSERT(ustr_cspn_cstr_fwd(s4, 0, "e") == 4);
  ASSERT(ustr_cspn_cstr_fwd(s4, 0, "!-") == 8);
  ASSERT(ustr_cspn_cstr_fwd(s4, 1, "!-") == 7);
  ASSERT(ustr_cspn_cstr_fwd(s4, 2, "!-") == 6);
  ASSERT(ustr_cspn_cstr_fwd(s4, 3, "!-") == 5);
  ASSERT(ustr_cspn_cstr_fwd(s4, 4, "!-") == 4);
  ASSERT(ustr_cspn_cstr_fwd(s4, 5, "!-") == 3);
  ASSERT(ustr_cspn_cstr_fwd(s4, 6, "!-") == 2);
  ASSERT(ustr_cspn_cstr_fwd(s4, 7, "!-") == 1);
  ASSERT(ustr_cspn_cstr_fwd(s4, 8, "!-") == 0);
  ASSERT(ustr_cspn_cstr_fwd(s4, 0, "!")  == 8);
  ASSERT(ustr_cspn_cstr_fwd(s4, 1, "!")  == 7);
  ASSERT(ustr_cspn_cstr_fwd(s4, 2, "!")  == 6);
  ASSERT(ustr_cspn_cstr_fwd(s4, 3, "!")  == 5);
  ASSERT(ustr_cspn_cstr_fwd(s4, 4, "!")  == 4);
  ASSERT(ustr_cspn_cstr_fwd(s4, 5, "!")  == 3);
  ASSERT(ustr_cspn_cstr_fwd(s4, 6, "!")  == 2);
  ASSERT(ustr_cspn_cstr_fwd(s4, 7, "!")  == 1);
  ASSERT(ustr_cspn_cstr_fwd(s4, 8, "!")  == 0);
  
  ASSERT(ustr_cspn_cstr_rev(s4, 0, "e") == 3);
  ASSERT(ustr_cspn_cstr_rev(s4, 0, "!-") == 8);
  ASSERT(ustr_cspn_cstr_rev(s4, 1, "!-") == 7);
  ASSERT(ustr_cspn_cstr_rev(s4, 2, "!-") == 6);
  ASSERT(ustr_cspn_cstr_rev(s4, 3, "!-") == 5);
  ASSERT(ustr_cspn_cstr_rev(s4, 4, "!-") == 4);
  ASSERT(ustr_cspn_cstr_rev(s4, 5, "!-") == 3);
  ASSERT(ustr_cspn_cstr_rev(s4, 6, "!-") == 2);
  ASSERT(ustr_cspn_cstr_rev(s4, 7, "!-") == 1);
  ASSERT(ustr_cspn_cstr_rev(s4, 8, "!-") == 0);
  ASSERT(ustr_cspn_cstr_rev(s4, 0, "!")  == 8);
  ASSERT(ustr_cspn_cstr_rev(s4, 1, "!")  == 7);
  ASSERT(ustr_cspn_cstr_rev(s4, 2, "!")  == 6);
  ASSERT(ustr_cspn_cstr_rev(s4, 3, "!")  == 5);
  ASSERT(ustr_cspn_cstr_rev(s4, 4, "!")  == 4);
  ASSERT(ustr_cspn_cstr_rev(s4, 5, "!")  == 3);
  ASSERT(ustr_cspn_cstr_rev(s4, 6, "!")  == 2);
  ASSERT(ustr_cspn_cstr_rev(s4, 7, "!")  == 1);
  ASSERT(ustr_cspn_cstr_rev(s4, 8, "!")  == 0);
  
  ASSERT(ustr_cspn_cstr_rev(s3, 0, "34") == 18);
  ASSERT(ustr_cspn_cstr_rev(s3, 0, " ") == 3);
  ASSERT(ustr_cspn_cstr_rev(s3, 0, "x") == 22);
  
  ASSERT(ustr_spn_cstr_fwd(s3, 0, "1")     == 1);
  ASSERT(ustr_spn_cstr_fwd(s3, 0, "12")    == 2);
  ASSERT(ustr_spn_cstr_fwd(s3, 0, "123")   == 3);
  ASSERT(ustr_spn_cstr_fwd(s3, 0, "1234")  == 4);
  ASSERT(ustr_spn_cstr_fwd(s3, 0, "1234y") == 8);
  
  ASSERT(ustr_del_subustr(&s3, 4, 4));
  ASSERT(ustr_spn_cstr_fwd(s3, 0, "1234y") == 4);  
  
  ustr_free(s3);
  ustr_free(s4); /* leaked, see above */
  ustr_free(s4);
  ustr_free(s5);
  
  return (EXIT_SUCCESS);
}
