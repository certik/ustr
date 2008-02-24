#include "tst.h"

static const char *rf = __FILE__;

#include <wchar.h>

#if USTR_CONF_HAVE_VA_COPY
static Ustr *my_dup_fmt(const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_A() USTR__COMPILE_ATTR_FMT(1, 2);
static Ustr *my_dup_fmt(const char *fmt, ...)
{
  va_list ap;
  Ustr *ret = NULL;
  
  va_start(ap, fmt);
  ret = ustr_dupx_vfmt(0, 0, 1, 0, fmt, ap);
  va_end(ap);

  return (ret);
}
static int my_fmt(Ustr **, const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_A() USTR__COMPILE_ATTR_FMT(2, 3);
static int my_fmt(Ustr **ps1, const char *fmt, ...)
{
  va_list ap;
  int ret = 0;
  
  va_start(ap, fmt);
  ret = ustr_set_vfmt(ps1, fmt, ap);
  va_end(ap);

  return (ret);
}
#endif

static void *fail_malloc(size_t len)
{
  (void)len;
  return NULL;
}

int tst(void)
{
  unsigned int num = 65534;
  unsigned int i;
  struct Ustr *tmp = NULL;
  Ustr *s3 = USTR1(\xb, "abcd 42 xyz");
  Ustr *s4 = USTR_NULL;

  assert(!USTR_CONF_USE_DYNAMIC_CONF ||
         ustr_cntl_opt(USTR_CNTL_OPT_SET_REF_BYTES, 2));

  if (USTR_CONF_USE_DYNAMIC_CONF)
  ustr_sc_free2(&s2, ustr_dup_buf(ustr_cstr(s2), ustr_len(s2)));

  ASSERT((s4 = ustr_dup_undef(18)));
  ASSERT(ustr_len(s1) == 0);
  ASSERT(ustr_len(s2) == strlen("s2"));

  ASSERT(ustr_size(s1) == 0);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size_overhead(s2) == 5); /* info[1] + ref[2] + len[1] ... NIL */
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s2) == 3); /* ustr__ns(1 + 2 + 1 + 2 + 1) -
                             *            1 + 2 + 1     + 1 */
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size_overhead(s4) == 5);

  ustr_sc_free(&s4);

  ASSERT( ustr_cmp_eq(s1, s1));
  ASSERT( ustr_cmp_eq(s2, s2));
  ASSERT(!ustr_cmp_eq(s1, s2));
  ASSERT( ustr_cmp_cstr_eq(s1, ""));
  ASSERT( ustr_cmp_cstr_eq(s2, "s2"));
  ASSERT( ustr_cmp_eq(s2, USTR1(\x0002, "s2")));
  ASSERT(!ustr_cmp(   s2, USTR1(\x2, "s2")));
  
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
  
  ASSERT(!ustr_srch_cstr_rev(s1, 0, "x"));
  ASSERT(!ustr_srch_cstr_rev(s2, 0, "x"));
  ASSERT(!ustr_srch_cstr_rev(s2, 0, "x"));
  ASSERT( ustr_srch_cstr_rev(s1, 0, "") == 0);
  ASSERT( ustr_srch_cstr_rev(s1, 0, "") == 0);
  ASSERT( ustr_srch_cstr_rev(s2, 0, "s2") == 1);
  ASSERT( ustr_srch_cstr_rev(s2, 0, "s")  == 1);
  ASSERT( ustr_srch_cstr_rev(s2, 0, "2")  == 2);
  ASSERT( ustr_srch_cstr_rev(s2, 1, "s")  == 1);

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
    struct Ustr *s99 = ustr_dup_cstr("x");
    
    ASSERT(ustr_len(s99)  ==  1);
    if (!USTR_CONF_USE_EOS_MARK)
    ASSERT(ustr_size(s99) ==  1);
    ustr_sc_free(&s99);
  }

  s2 = USTR1(\x02, "xy");
  ASSERT(ustr_size(s2) == 2);
  ASSERT(ustr_len(s2) == 2);
  ASSERT( ustr_ro(s2));
  ASSERT( ustr_shared(s2));
  ASSERT(!ustr_enomem(s2));
  ASSERT(!ustr_setf_enomem_err(s2));
  ASSERT(!ustr_enomem(s2));
  ASSERT(!ustr_setf_enomem_clr(s2));
  ASSERT(!ustr_enomem(s2));
  
#if USTR_CONF_HAVE_VA_COPY
  ASSERT((s2 = my_dup_fmt("abcd %d xyz", 42)));
  ASSERT(my_fmt(&s2, "abcd %.200d xyz", 42));
  ASSERT(s2);
  ASSERT(ustr_len(s2) == 209);
  ASSERT(ustr_cmp_subustr_eq(USTR1(\x4, "abcd"), s2, 1, 4));
  ASSERT(!ustr_cmp_subustr(USTR1(\x7, "042 xyz"), s2, ustr_len(s2) - 6, 7));
  ASSERT(!ustr_setf_share(s2));
  ASSERT( ustr_add_undef(&s2, 8));
  ASSERT(ustr_len(s2) == (209 + 8));
  ASSERT(ustr_cmp_subustr_eq(USTR1(\x4, "abcd"), s2, 1, 4));
  ASSERT(!ustr_cmp_subustr(USTR1(\x7, "042 xyz"), s2, ustr_len(s2) - (6+8), 7));
  ustr_sc_free2(&s2, USTR(""));
  ASSERT(my_fmt(&s2, "abcd %.200d xyz", 42));
  ASSERT(s2);
  ASSERT(ustr_len(s2) == 209);
  ASSERT(ustr_cmp_subustr_eq(USTR1(\x4, "abcd"), s2, 1, 4));
  ASSERT(!ustr_cmp_subustr(USTR1(\x7, "042 xyz"), s2, ustr_len(s2) - 6, 7));
  ASSERT( ustr_setf_share(s2));
  ASSERT( ustr_setf_owner(s2));
  ASSERT( ustr_add(&s2, USTR("")));
#ifdef __linux__
  {
    wchar_t *wt = (wchar_t *)"\xFF\xFF\xFF\xFF";
  /* test multi-byte conversion failure */
  ASSERT(!ustr_add_fmt(&s2, "%ls", wt));
  ASSERT(!ustr_dup_fmt(     "%ls", wt));
  ASSERT(!ustr_set_fmt(&s2, "%ls", wt));
  ASSERT(!ustr_ins_fmt(&s2, 1, "%ls", wt));
  ASSERT(!ustr_sub_fmt(&s2, 1, "%ls", wt));
  ASSERT(!ustr_sc_sub_fmt(&s2, 1, 2, "%ls", wt));
  }
#endif
#endif
  ustr_sc_free2(&s2, ustr_dupx(1, 0, 0, 0, s2));
  
  ASSERT(!ustr_exact(s2));
  ASSERT(ustr_len(s2) == 209);
  ASSERT(ustr_size(s2) > 209);
  ASSERT( ustr_setf_share(s2));
  ASSERT( ustr_setf_owner(s2));
  ASSERT(ustr_len(s2) == 209);
  ASSERT(ustr_size(s2) > 209);
  ASSERT(!ustr_exact(s2));
  ASSERT( ustr_realloc(&s2, 209));
  ASSERT(ustr_len(s2) == 209);
  ASSERT(ustr_size(s2) == 209);
  ASSERT(!ustr_exact(s2));
  ASSERT( ustr_realloc(&s2, 210));
  ASSERT(ustr_len(s2) == 209);
  ASSERT(ustr_size(s2) == 210);
  ASSERT(!ustr_exact(s2));
  ASSERT(ustr_cmp_subustr_eq(USTR1(\x4, "abcd"), s2, 1, 4));
  ASSERT(!ustr_cmp_subustr(USTR1(\x7, "042 xyz"), s2, ustr_len(s2) - 6, 7));

  ustr_sc_free2(&s2, USTR(""));
  ASSERT(ustr_sc_ensure_owner(&s2));
  
  ustr_sc_free2(&s2, USTR(""));
  ASSERT(ustr_sc_wstr(&s2));
  
  ustr_sc_free2(&s2, USTR1(\1, "1"));
  ASSERT(ustr_sc_wstr(&s2));
  
  ASSERT( ustr_setf_share(USTR("")));
  ASSERT(!ustr_setf_owner(USTR("")));
  
  ASSERT(ustr_set_cstr(&s2, "abcd 42 xyz"));
  ASSERT(!ustr_cmp_fast_cstr(s2, "abcd 42 xyz"));
  ASSERT( ustr_owner(s2));
  ASSERT(s3 != s2);
  assert_eq(s3, s2);
  ASSERT(ustr_set(&s3, s2));
  ASSERT_EQ(s3, s2);
  ASSERT(s3 == s2);
  ASSERT(!ustr_owner(s2));
  
  ASSERT(ustr_set_fmt(&s2, "abcd %.5000d xyz", 42));
  ASSERT( ustr_owner(s2));
  ASSERT((tmp = ustr_dup(s2)));
  ASSERT(tmp == s2);
  ASSERT(!ustr_owner(s2));
  ASSERT(ustr_set(&tmp, s3));
  assert_eq(s3, tmp);
  ASSERT(ustr_set_rep_chr(&tmp, 'x', 0));
  ASSERT(ustr_len(tmp) == 0);
  ustr_free(tmp);
  ustr_free(s3);
  
  ASSERT(!ustr_enomem(s2));
  ASSERT(ustr_setf_enomem_err(s2));
  ASSERT( ustr_enomem(s2));
  ASSERT(ustr_setf_enomem_clr(s2));
  ASSERT(!ustr_enomem(s2));
  
  ASSERT(!ustr_shared(s2));
  ASSERT(ustr_set_fmt(&s2, "%x", 0xf0));
  ASSERT( ustr_setf_share(s2));
  ASSERT( ustr_shared(s2));
  
  for (i = 0; i < 70000; ++i)
  {
    tmp = ustr_dup(s2);
    ASSERT(tmp == s2);
  }
  
  for (i = 0; i < 80000; ++i)
    ustr_free(s2);
  
  ASSERT( ustr_setf_owner(s2));
  ASSERT(!ustr_shared(s2));
  ASSERT( ustr_owner(s2));

  ASSERT(tmp == s2);
  ASSERT( ustr_set_fmt(&s2, "%x", 0xff));
  ASSERT(tmp == s2); /* realloc's down */
  ASSERT(!ustr_shared(s2));
  ASSERT( ustr_owner(s2));
  
  tmp = ustr_dup(s2);
  ASSERT(tmp == s2);
  ASSERT(!ustr_shared(s2));
  ASSERT(!ustr_owner(s2));
  ASSERT(!ustr_owner(tmp));
  ASSERT( ustr_set_fmt(&s2, "%x", 0xff));
  ASSERT(tmp != s2);
  ASSERT(ustr_cmp_eq(tmp, s2));
  ASSERT( ustr_owner(s2));
  ASSERT( ustr_owner(tmp));
  ustr_free(tmp);

  ustr_sc_free2(&s1, ustr_dupx_cstr(1, 1, 1, 1, "1234abcd"));
  ASSERT_EQ(s1, USTR1(\x8, "1234abcd"));
  ASSERT(ustr_enomem(s1));
  ASSERT(ustr_sized(s1));
  ASSERT(ustr_exact(s1));
  ASSERT(ustr_setf_enomem_clr(s1));

  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x8, "dcba4321"));
  ASSERT(ustr_del(&s1, 1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x7, "234abcd"));
  ASSERT(ustr_del(&s1, 1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x6, "cba432"));
  ASSERT(ustr_del(&s1, 1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x5, "34abc"));
  ASSERT(ustr_del(&s1, 1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x4, "ba43"));
  ASSERT(ustr_del(&s1, 1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x3, "4ab"));
  ASSERT(ustr_del(&s1, 1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x2, "a4"));
  ASSERT(ustr_del(&s1, 1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR1(\x1, "a"));
  ASSERT(ustr_del(&s1, 1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR(""));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT(ustr_sc_reverse(&s1));
  ASSERT_EQ(s1, USTR(""));  
  
  ustr_sc_free2(&s1, ustr_dupx_cstr(1, 1, 1, 1, "aBcDeFgHiJ"));
  ASSERT(ustr_sc_tolower(&s1));
  ASSERT_EQ(s1, USTR1(\xa, "abcdefghij"));
  ASSERT(ustr_sc_toupper(&s1));
  ASSERT_EQ(s1, USTR1(\xa, "ABCDEFGHIJ"));
  ustr_sc_free2(&s1, ustr_dupx_cstr(1, 1, 1, 1, "aBcDeFgHiJ"));
  ASSERT(ustr_sc_toupper(&s1));
  ASSERT_EQ(s1, USTR1(\xa, "ABCDEFGHIJ"));
  ASSERT(ustr_sc_tolower(&s1));
  ASSERT_EQ(s1, USTR1(\xa, "abcdefghij"));

  ASSERT(!ustr_srch_rep_chr_fwd(s1, 0, 'j', 2));
  ASSERT(!ustr_srch_rep_chr_rev(s1, 0, 'j', 2));
  
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, 'j', 1) == 10);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, 'j', 1) == 10);
  
  {
    size_t esz;
    size_t ref;
    int exact;
    size_t lenn;
    size_t refc;

    s4 = ustr_dup_cstr("x");
    
    ustr_conf(s4, NULL,NULL,NULL, NULL,NULL);
    ustr_conf(s4, &esz,&ref,&exact, &lenn,&refc);

    ASSERT(!esz);
    ASSERT(ref == 2);
    ASSERT(!exact);
    
    ASSERT(lenn == 1);
    ASSERT(refc == 1);
    
    ASSERT(ustr_len(s4)  ==  1);
    if (!USTR_DEBUG)
    ASSERT(ustr_size(s4) ==  1);
    ustr_sc_free(&s4);
  }

  { /* bug */
    int scan = 0;
    Ustr *os1 = s1;
    
    while (scan++ < 65534)
    {
      s1 = ustr_dupx(1, 1, 1, 1, s1);
      ASSERT(s1 == os1);
    }
    ASSERT((s1 = ustr_dupx(1, 1, 1, 1, s1)));
    ASSERT(s1 != os1);
    ASSERT(s1->data[0] == os1->data[0]);

    ustr_sc_free2(&s1, os1);
    scan = 0;
    while (scan++ < 65534)
      ustr_free(s1);

    if (USTR_CONF_USE_DYNAMIC_CONF)
    {
      assert(ustr_cntl_opt(USTR_CNTL_OPT_SET_REF_BYTES, 0));
      assert(ustr_cntl_opt(USTR_CNTL_OPT_SET_HAS_SIZE,  1));
  
      ASSERT((os1 = ustr_dupx_undef(1, 2, 0, 0, 0)));
      ASSERT(ustr_ro(os1));
    }
  }
  
  ustr_sc_free2(&s1, USTR1(\x14, "123456789912345xxxxx"));
  
  ASSERT(!ustr_srch_rep_chr_fwd(s1, 0, 'x', 6));
  ASSERT(!ustr_srch_rep_chr_rev(s1, 0, 'x', 6));
  
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, 'x', 5) == 16);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, 'x', 5) == 16);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, 'x', 4) == 16);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, 'x', 4) == 17);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, 'x', 3) == 16);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, 'x', 3) == 18);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, 'x', 2) == 16);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, 'x', 2) == 19);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, 'x', 1) == 16);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, 'x', 1) == 20);
  
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, '9', 2) ==  9);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, '9', 2) ==  9);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, '9', 1) ==  9);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, '9', 1) == 10);
  
  ASSERT(!ustr_srch_rep_chr_fwd(s1, 0, '5', 2));
  ASSERT(!ustr_srch_rep_chr_rev(s1, 0, '5', 2));
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, '5', 1) ==  5);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, '5', 1) == 15);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 0, '5', 0) ==  1);
  ASSERT(ustr_srch_rep_chr_rev(s1, 0, '5', 0) == 20);
  
  {
    size_t scan = 0x15;
    char *cstr = ustr_sc_export(s1, malloc);
    
    ASSERT(cstr);
    ASSERT(strlen(cstr) == ustr_len(s1));
    ASSERT(ustr_cmp_cstr_eq(s1, cstr));
    free(cstr);

    while (scan--)
    {
      cstr = ustr_sc_export_subustr(s1, 1, scan, malloc);
      ASSERT(cstr);
      ASSERT(strlen(cstr) == scan);
      ASSERT(ustr_cmp_prefix_cstr_eq(s1, cstr));
      free(cstr);
    }
    scan = 0x14;
    while (scan--)
    {
      cstr = ustr_sc_export_subustr(s1, scan + 1, ustr_len(s1) - scan, malloc);
      ASSERT(cstr);
      ASSERT(strlen(cstr) == ustr_len(s1) - scan);
      ASSERT(ustr_cmp_suffix_cstr_eq(s1, cstr));
      free(cstr);
    }

    cstr = ustr_sc_export_subustr(s1, 4, 3, malloc);
    ASSERT(cstr);
    ASSERT(strlen(cstr) == 3);
    ASSERT(ustr_cmp_cstr_eq(USTR1(\3, "456"), cstr));
    free(cstr);

    cstr = ustr_sc_export_subustr(s1, 4, 0, malloc);
    ASSERT(cstr);
    ASSERT(strlen(cstr) == 0);
    free(cstr);

    if (!USTR_DEBUG)
    ASSERT(!ustr_sc_export_subustr(s1, 1, 0x15, malloc));
    if (!USTR_DEBUG)
    ASSERT(!ustr_sc_export_subustr(s1, 2, 0x14, malloc));
  }

  ASSERT(!ustr_sc_export(s1, fail_malloc));
  ASSERT(!ustr_sc_export_subustr(s1, 1, 4, fail_malloc));
  
  return (EXIT_SUCCESS);
}

