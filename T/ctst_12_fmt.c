#if defined(USTR_CONF_INCLUDE_CODEONLY_HEADERS) &&      \
    USTR_CONF_INCLUDE_CODEONLY_HEADERS
#define _GNU_SOURCE 1
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

# define USTR_CONF_VSNPRINTF_BEG tst_12_fmt_beg
# define USTR_CONF_VSNPRINTF_END tst_12_fmt_end
static int tst_12_fmt_beg(char *buf, size_t len, const char *fmt, va_list ap);
static int tst_12_fmt_end(char *buf, size_t len, const char *fmt, va_list ap);
#endif
#include "tst.h"

static const char *rf = __FILE__;

static unsigned int tst__12_fmt_beg_count = 0;
static int tst_12_fmt_beg(char *buf, size_t len, const char *fmt, va_list ap)
{
  ++tst__12_fmt_beg_count;
  return vsnprintf(buf, len, fmt, ap);
}
static unsigned int tst__12_fmt_end_count = 0;
static int tst_12_fmt_end(char *buf, size_t len, const char *fmt, va_list ap)
{
  ++tst__12_fmt_end_count;
  return vsnprintf(buf, len, fmt, ap);
}


int tst(void)
{
  Ustr_pool *pool = ustr_pool_ll_make();
  Ustrp *sp1 = USTRP("");
  Ustr  *s3  = ustr_dupx_empty(0, 1, 0, 0);
  Ustr  *s4  = USTR("");
  Ustrp *sp3 = ustrp_dupx_empty(pool, 0, 1, 0, 0);
  Ustrp *sp4 = USTRP("");
#if !USTR_CONF_INCLUDE_CODEONLY_HEADERS
  Ustr_cntl_fmt ofmt;
  Ustr_cntl_fmt nfmt = {tst_12_fmt_beg, tst_12_fmt_end};
  
  assert(ustr_cntl_opt(USTR_CNTL_OPT_GET_FMT, &ofmt) &&
         ofmt.sys_vsnprintf_beg == vsnprintf && 
         ofmt.sys_vsnprintf_end == vsnprintf);
#endif

  ASSERT(pool);
  ASSERT(s3);

  ASSERT(ustr_set_rep_chr(&s4, '0', 1024));
  ASSERT(ustrp_set_rep_chr(pool, &sp4, '0', 1024));

  ustr_sc_free2(&s1, ustr_dup(s3));
  ustr_sc_free2(&s1, ustr_dup(s3));
  ustr_sc_free2(&s1, ustr_dup(s3));
  ustr_sc_free2(&s1, ustr_dup(s3));
  
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  
  ASSERT(ustr_add_fmt_lim(&s1, 1, "%s", "12"));
  ASSERT_EQ(s1, USTR1(\x1, "1"));
  
  ASSERT(ustrp_add_fmt_lim(pool, &sp1, 1024, "%.2000d", 0));
  ASSERT_PEQ(sp1, sp4);
  
  ustrp_sc_free2(pool, &sp1, ustrp_dup(pool, sp3));
  ASSERT(ustrp_add_fmt(pool, &sp1, "%.1024d", 0));
  ASSERT_PEQ(sp1, sp4);
  
  ustr_sc_free(&s1);
  ASSERT((s1 = ustr_dupx_fmt_lim(1, 1, 1, 0, 4, "%s", "12345678")));
  ASSERT_EQ(s1, USTR1(\x4, "1234"));
  
  ustrp_sc_free(pool, &sp1);
  ASSERT((sp1 = ustrp_dupx_fmt_lim(pool, 1, 1, 1, 0, 4, "%s", "12345678")));
  ASSERT_PEQ(sp1, USTRP1(\x4, "1234"));
  
  ustr_sc_free(&s1);
  ASSERT((s1 = ustr_dupx_fmt(1, 1, 1, 0, "%s", "12345678")));
  ASSERT_EQ(s1, USTR1(\x8, "12345678"));
  
  ustrp_sc_free(pool, &sp1);
  ASSERT((sp1 = ustrp_dupx_fmt(pool, 1, 1, 1, 0, "%s", "12345678")));
  ASSERT_PEQ(sp1, USTRP1(\x8, "12345678"));
  
  ustr_sc_free(&s1);
  ASSERT((s1 = ustr_dup_fmt_lim(4, "%s", "12345678")));
  ASSERT_EQ(s1, USTR1(\x4, "1234"));
  
  ustrp_sc_free(pool, &sp1);
  ASSERT((sp1 = ustrp_dup_fmt_lim(pool, 4, "%s", "12345678")));
  ASSERT_PEQ(sp1, USTRP1(\x4, "1234"));
  
  ustr_sc_free(&s1);
  ASSERT((s1 = ustr_dup_fmt("%s", "12345678")));
  ASSERT_EQ(s1, USTR1(\x8, "12345678"));
  
  ustrp_sc_free(pool, &sp1);
  ASSERT((sp1 = ustrp_dup_fmt(pool, "%s", "12345678")));
  ASSERT_PEQ(sp1, USTRP1(\x8, "12345678"));

  ASSERT(ustr_set_undef(&s1, 400));
  ASSERT(ustrp_set_undef(pool, &sp1, 400));
  
  ASSERT(ustr_set_fmt(&s1, "%s", "12345678"));
  ASSERT_EQ(s1, USTR1(\x8, "12345678"));

  ASSERT(ustrp_set_fmt(pool, &sp1, "%s", "12345678"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "12345678"));
  
  ASSERT(ustr_set_fmt_lim(&s1, 2, "%s", "12345678"));
  ASSERT_EQ(s1, USTR1(\x2, "12"));

  ASSERT(ustrp_set_fmt_lim(pool, &sp1, 2, "%s", "12345678"));
  ASSERT_PEQ(sp1, USTRP1(\x2, "12"));
  
  ASSERT(ustr_set_fmt_lim(&s1, 4, "%s", "12345678"));
  ASSERT_EQ(s1, USTR1(\x4, "1234"));

  ASSERT(ustrp_set_fmt_lim(pool, &sp1, 4, "%s", "12345678"));
  ASSERT_PEQ(sp1, USTRP1(\x4, "1234"));
  
  ASSERT(ustr_set_empty(&s1));
  ASSERT(ustrp_set_empty(pool, &sp1));

  ASSERT(ustrp_set_cstr(pool, &sp1, "abcd"));
  ASSERT_PEQ(sp1, USTRP1(\x4, "abcd"));
  ASSERT(ustrp_set(pool, &sp1, USTRP1(\x3, "xyz")));
  ASSERT_PEQ(sp1, USTRP1(\x3, "xyz"));

  ustr_sc_free2(&s1, USTR1(\x3, "xyz"));
  ASSERT(ustr_set_subustr(&s1, s1, 2, 1));
  ASSERT_EQ(s1, USTR1(\x1, "y"));

  ASSERT(ustrp_set_subustrp(pool, &sp1, sp1, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x1, "y"));
  
  ustr_pool_free(pool);

  ustr_sc_free(&s3);
  ustr_sc_free(&s4);

  ASSERT(ustr_set_fmt(&s1, "%s%d%*s%d%s", "a", 4, 128, "b", 8, "c"));
  ASSERT(ustr_set_fmt(&s2, "%s%d%s%d%s",  "a", 4,      "b", 8, "c"));

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
  ASSERT(tst__12_fmt_beg_count && tst__12_fmt_end_count);
#else
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_FMT, &nfmt));
  assert(ustr_cntl_opt(USTR_CNTL_OPT_GET_FMT, &ofmt) &&
         ofmt.sys_vsnprintf_beg == nfmt.sys_vsnprintf_beg && 
         ofmt.sys_vsnprintf_end == nfmt.sys_vsnprintf_end);
  ASSERT(!tst__12_fmt_beg_count && !tst__12_fmt_end_count);
#endif

  tst__12_fmt_beg_count = 0;
  tst__12_fmt_end_count = 0;
  ASSERT((s3 = ustr_dup_fmt("%s%d%*s%d%s", "a", 4, 128, "b", 8, "c")));
  ASSERT(tst__12_fmt_beg_count == 1);
  ASSERT(tst__12_fmt_end_count == 1);
  ASSERT_EQ(s1, s3);
  ASSERT((s4 = ustr_dup_fmt("%s%d%s%d%s",  "a", 4,      "b", 8, "c")));
  ASSERT(tst__12_fmt_beg_count == 2);
  ASSERT(tst__12_fmt_end_count == 1);
  ASSERT_EQ(s2, s4);

  ustr_sc_free(&s3);
  ustr_sc_free(&s4);
  
  return (EXIT_SUCCESS);
}
