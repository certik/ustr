/* this compiles with:
   gcc -Wall -W -lustr $(pkg-config --cflags --libs vstr) \
       tst_vstr_chk.c -o tst_vstr_chk
*/
#include <vstr.h>
#include <ustr.h>
#include <assert.h>

static int ufmt__ustr_cb(Vstr_base *base, size_t pos, Vstr_fmt_spec *spec)
{
  Ustr *us1 = VSTR_FMT_CB_ARG_PTR(spec, 0);
  size_t sf_len = ustr_len(us1);

  if (!vstr_sc_fmt_cb_beg(base, &pos, spec, &sf_len,
                          VSTR_FLAG_SC_FMT_CB_BEG_OBJ_STR))
    return (USTR_FALSE);
  
  if (!vstr_add_buf(base, pos, ustr_cstr(us1), sf_len))
    return (USTR_FALSE);
  
  if (!vstr_sc_fmt_cb_end(base, pos, spec, sf_len))
    return (USTR_FALSE);

  return (USTR_TRUE);
}

static int ufmt_add_ustr(struct Vstr_conf *conf, const char *name)
{
  return (vstr_fmt_add(conf, name, ufmt__ustr_cb,
                       VSTR_TYPE_FMT_PTR_VOID,
                       VSTR_TYPE_FMT_END));
}

static Vstr_base *vs1 = NULL;
int ufmt_beg(char *buf, size_t len, const char *fmt, va_list ap)
{
  size_t ret;
  
  vstr_del(vs1, 1, vs1->len);

  if (!vstr_add_vfmt(vs1, 0, fmt, ap))
    return -1;

  ret = vs1->len;
  vstr_export_cstr_buf(vs1, 1, vs1->len, buf, len);
  if (vs1->len > len)
    vstr_del(vs1, 1, vs1->len);
  
  return ret;
}

int ufmt_end(char *buf, size_t len, const char *fmt, va_list ap)
{
  size_t ret = vs1->len;
  
  vstr_export_cstr_buf(vs1, 1, vs1->len, buf, len);

  vstr_del(vs1, 1, vs1->len);

  return ret;
}

int main(void)
{
  Ustr *us1 = USTR("");
  struct Ustr_cntl_fmt ufmt;
  int bad = 1;
  
  ufmt.sys_vsnprintf_beg = ufmt_beg;
  ufmt.sys_vsnprintf_end = ufmt_end;

  if (!ustr_cntl_opt(USTR_CNTL_OPT_SET_FMT, &ufmt))
    return bad; ++bad;
  
  if (!vstr_init())
    return bad; ++bad;

  if (!(vs1 = vstr_make_base(NULL)))
    return bad; ++bad;

  if (!vstr_cntl_conf(vs1->conf, VSTR_CNTL_CONF_SET_FMT_CHAR_ESC, '$'))
    return bad; ++bad;

  if (!VSTR_SC_FMT_ADD(NULL, ufmt_add_ustr, "<ustr", "p", ">"))
    return bad; ++bad;


  if (!ustr_add_fmt(&us1, "a%sb", " | "))
    return bad; ++bad;

  assert(ustr_cmp_eq(USTR1(\5, "a | b"), us1));
  ustr_sc_del(&us1);
  assert(!ustr_len(us1));
  
  if (!ustr_add_fmt(&us1, "a$<ustr:%p>b", USTR1(\3, " | ")))
    return bad; ++bad;

  assert(ustr_cmp_eq(USTR1(\5, "a | b"), us1));
  
  vstr_free_base(vs1);

  vstr_exit();
  
  ustr_sc_free(&us1);
  
  return 0;
}
