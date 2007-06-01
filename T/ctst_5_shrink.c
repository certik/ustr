#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  struct Ustr *s3 = NULL;
  struct Ustr *s4 = NULL;

  assert(!USTR_CONF_USE_DYNAMIC_CONF ||
         ustr_cntl_opt(USTR_CNTL_OPT_SET_REF_BYTES, 1));
  
  /* move to the new "default" conf */
  ustr_sc_free2(&s2, ustr_dup_buf(ustr_cstr(s2), ustr_len(s2)));
  ASSERT(s2);

  s3 = ustr_dup_cstr("s3 abcd s2");
  s4 = ustr_dup_rep_chr('x', 40);
  
  ASSERT(s3);
  ASSERT(s4);
  
  ASSERT(ustr_len(s1)  ==  0);
  ASSERT(ustr_len(s2)  ==  2);
  ASSERT(ustr_len(s3)  == 10);
  ASSERT(ustr_len(s4)  == 40);
  
  ASSERT(ustr_size(s1) ==  0);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s2) ==  2); /* ustr__ns(1 + 1 + 1 + 2 + 1) -
                              *            1 + 1 + 1     + 1 */
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s3) == 12); /* ustr__ns(1 + 1 + 1 + 10 + 1) -
                              *            1 + 1 + 1      + 1 */
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s4) == 44);

  ASSERT(ustr_add_rep_chr(&s4, '-', 8));
  ASSERT(ustr_len(s4)  == 48);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s4) == 60);
  
  ASSERT(ustr_del(&s4, 4));
  ASSERT(ustr_len(s4)  == 44);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s4) == 44);
  
  ASSERT(ustr_del(&s4, 20));
  ASSERT(ustr_len(s4)  == 24);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s4) == 28);
  
  ustr_sc_free(&s3);

  ASSERT((s3 = ustr_dup(s4)));
  ASSERT(ustr_len(s3)  == 24);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s3) == 28);
  ASSERT(ustr_len(s4)  == 24);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s4) == 28);
  
  ASSERT(ustr_del(&s4, 15));
  ASSERT(ustr_len(s3)  == 24);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s3) == 28);
  ASSERT(ustr_len(s4)  ==  9);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s4) == 12);

  ASSERT(ustr_cmp_cstr_eq(s3,   "xxxxxxxxxxxxxxxxxxxxxxxx"));
  ASSERT(!strcmp(ustr_cstr(s3), "xxxxxxxxxxxxxxxxxxxxxxxx"));

  ASSERT(ustr_cmp_cstr_eq(s4,   "xxxxxxxxx"));
  ASSERT(!strcmp(ustr_cstr(s4), "xxxxxxxxx"));

  ASSERT(!ustr_del(&s4, 15));
  ASSERT(ustr_del(&s4, 8));
  ASSERT(ustr_len(s4)  ==  1);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s4) ==  2);
  ASSERT(ustr_cmp_cstr_eq(s4,   "x"));
  ASSERT(!strcmp(ustr_cstr(s4), "x"));
  ASSERT(ustr_add_rep_chr(&s4, '-', 8));
  ASSERT(ustr_len(s4)  ==  9);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s4) == 12);
  ASSERT(ustr_cmp_cstr_eq(s4,   "x--------"));
  ASSERT(!strcmp(ustr_cstr(s4), "x--------"));

  ASSERT(ustr_set_cstr(&s4,     "x12345678"));
  ustr_sc_free2(&s3, s4);
  ASSERT(ustr_cmp_cstr_eq(s4,   "x12345678"));
  ASSERT( ustr_owner(s4));
  ASSERT(!ustr_shared(s4));
  ASSERT(s3 == s4);
  ASSERT( ustr_setf_share(s4));
  ASSERT(!ustr_owner(s4));
  ASSERT(ustr_del_subustr(&s4, 4, 4));
  ASSERT(ustr_cmp_cstr_eq(s3,   "x12345678"));
  ASSERT(ustr_cmp_cstr_eq(s4,   "x1278"));

  ustr_sc_free_shared(&s3);
  ustr_sc_free(&s4);
  
  return (EXIT_SUCCESS);
}
