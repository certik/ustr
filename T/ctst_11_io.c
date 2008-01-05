#include "tst.h"

static const char *rf = __FILE__;

/* 1234567 */

static const Ustr *line1 = USTR1(\x11, "#include \"tst.h\"\n");
static const Ustr *line2 = USTR1(\x1, "\n");
static const Ustr *line3 = USTR1(\x22, "static const char *rf = __FILE__;\n");
static const Ustr *line4 = USTR1(\x1, "\n");
static const Ustr *line5 = USTR1(\xE, "/* 1234567 */\n");
static const Ustr *line999 = USTR1(\xa0, /* 160 */
"123456789 " "123456789 " "123456789 " "123456789 " "123456789 "
"123456789 " "123456789 " "123456789 " "123456789 " "123456789 "
"123456789 " "123456789 " "123456789 " "123456789 " "123456789 "
"123456789 ");

static void tst_getline(FILE *fp, int use_delim)
{
  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  if (!use_delim)
    ASSERT(ustr_io_getline(&s1, fp));
  else
  {
    ASSERT(ustr_io_getdelim(&s1, fp, '"'));
    ASSERT(ustr_cmp_cstr_eq(s1, "#include \""));
    ASSERT(ustr_io_getdelim(&s1, fp, '"'));
    ASSERT(ustr_cmp_cstr_eq(s1, "#include \"tst.h\""));
    ASSERT(ustr_io_getdelim(&s1, fp, '\n'));
  }
  ASSERT_EQ(s1, line1);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line2);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  if (!use_delim)
    ASSERT(ustr_io_getline(&s1, fp));
  else
  {
    ASSERT(ustr_io_getdelim(&s1, fp, ' '));
    ASSERT(ustr_cmp_cstr_eq(s1, "static "));
    ASSERT(ustr_io_getdelim(&s1, fp, ' '));
    ASSERT(ustr_cmp_cstr_eq(s1, "static const "));
    ASSERT(ustr_io_getdelim(&s1, fp, ' '));
    ASSERT(ustr_cmp_cstr_eq(s1, "static const char "));
    ASSERT(ustr_io_getdelim(&s1, fp, '='));
    ASSERT(ustr_cmp_cstr_eq(s1, "static const char *rf ="));
    ASSERT(ustr_io_getdelim(&s1, fp, ' '));
    ASSERT(ustr_cmp_cstr_eq(s1, "static const char *rf = "));
    ASSERT(ustr_io_getdelim(&s1, fp, ';'));
    ASSERT(ustr_cmp_cstr_eq(s1, "static const char *rf = __FILE__;"));
    ASSERT(ustr_io_getdelim(&s1, fp, '\n'));
  }
  ASSERT_EQ(s1, line3);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line4);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  if (!use_delim)
    ASSERT(ustr_io_getline(&s1, fp));
  else
  {
    ASSERT(ustr_io_getdelim(&s1, fp, '/'));
    ASSERT(ustr_cmp_cstr_eq(s1, "/"));
    ASSERT(ustr_io_getdelim(&s1, fp, '/'));
    ASSERT(ustr_cmp_cstr_eq(s1, "/* 1234567 */"));
    ASSERT(ustr_io_getdelim(&s1, fp, '\n'));
  }
  ASSERT_EQ(s1, line5);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
}

#include <errno.h>
int tst(void)
{
  Ustr_pool *pool = ustr_pool_ll_make();
  Ustrp *sp1 = USTRP("");
  FILE *fp = fopen("T/ctst_11_io.c", "rb");
  size_t got = 1;
  int ret = EXIT_SUCCESS;
  
  ASSERT(pool);
  ASSERT(fp);

  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_get(&s1, fp, 0, &got));
  ASSERT(!got);
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_get(&s1, fp, 11, &got));
  ASSERT(ustr_len(s1) == 11);
  ASSERT(got == 11);
  ASSERT(ustr_io_get(&s1, fp, 20, &got));
  ASSERT(ustr_len(s1) == 31);
  ASSERT(got == 20);
  ASSERT(ustr_io_get(&s1, fp, 40, &got));
  ASSERT(ustr_len(s1) == 71);
  ASSERT(got == 40);
  ASSERT(71 >= (ustr_len(line1) + ustr_len(line2) + ustr_len(line3) +
                ustr_len(line4) + ustr_len(line5)));

  ASSERT(ustr_cmp_subustr_eq(line1, s1, 1, ustr_len(line1)));
  ASSERT(ustr_del_subustr(&s1, 1, ustr_len(line1)));
  ASSERT(ustr_cmp_subustr_eq(line2, s1, 1, ustr_len(line2)));
  ASSERT(ustr_del_subustr(&s1, 1, ustr_len(line2)));
  ASSERT(ustr_cmp_subustr_eq(line3, s1, 1, ustr_len(line3)));
  ASSERT(ustr_del_subustr(&s1, 1, ustr_len(line3)));
  ASSERT(ustr_cmp_subustr_eq(line4, s1, 1, ustr_len(line4)));
  ASSERT(ustr_del_subustr(&s1, 1, ustr_len(line4)));
  ASSERT(ustr_cmp_subustr_eq(line5, s1, 1, ustr_len(line5)));
  ASSERT(ustr_del_subustr(&s1, 1, ustr_len(line5)));
  
  rewind(fp);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  
  ASSERT(ustr_io_getfilename(&s1, "T/ctst_11_io.c"));
  ASSERT(ustr_len(s1) >= 71);

  /* delete line3 first... */
  ASSERT(ustr_cmp_subustr_eq(line3, s1, 1 + ustr_len(line1) + ustr_len(line2),
                             ustr_len(line3)));
  ASSERT(ustr_del_subustr(&s1, 1 + ustr_len(line1) + ustr_len(line2),
                          ustr_len(line3)));
  
  ASSERT(ustr_cmp_subustr_eq(line1, s1, 1, ustr_len(line1)));
  ASSERT(ustr_del_subustr(&s1, 1, ustr_len(line1)));
  ASSERT(ustr_cmp_subustr_eq(line2, s1, 1, ustr_len(line2)));
  ASSERT(ustr_del_subustr(&s1, 1, ustr_len(line2)));
  /* line 3 already gone */
  ASSERT(ustr_cmp_subustr_eq(line4, s1, 1, ustr_len(line4)));
  ASSERT(ustr_del_subustr(&s1, 1, ustr_len(line4)));
  ASSERT(ustr_cmp_subustr_eq(line5, s1, 1, ustr_len(line5)));
  ASSERT(ustr_del_subustr(&s1, 1, ustr_len(line5)));
  
  rewind(fp); tst_getline(fp, USTR_FALSE);
  rewind(fp); tst_getline(fp, USTR_TRUE);
  ASSERT(ustr_io_getfile(&s1, fp));
  ASSERT( ustr_len(s1));
  ustr_sc_del(&s1);
  
  fclose(fp);

  ASSERT(ustr_set_fmt(&s2, "%s.out-tst-tmp", rf));
  
  fp = fopen(ustr_cstr(s2), "w+b");
  ASSERT(fp);

  ASSERT(ustr_set(&s1, line1));
  ASSERT(ustr_io_put(&s1, fp, 1));
  ASSERT(!ustr_cmp_subustr(s1, line1, 2, ustr_len(line1) - 1));
  ASSERT(ustr_io_put(&s1, fp, 1));
  ASSERT(ustr_cmp_subustr_eq(s1, line1, 3, ustr_len(line1) - 2));
  ASSERT(ustr_io_putfile(&s1, fp));
  ASSERT(!ustr_len(s1));
  ustr_sc_free2(&s1, (Ustr *)line2); /* remove const, hackish */
  ASSERT(ustr_io_putfile(&s1, fp));
  ASSERT(!ustr_len(s1));  
  ASSERT(ustr_set(&s1, line3));
  ASSERT(ustr_add(&s1, line4));
  ASSERT(ustr_add(&s1, line5));
  ASSERT(ustr_del(&s1, 1)); /* remove \n */
  ASSERT( ustr_len(s1));  
  ASSERT(ustr_io_putfileline(&s1, fp));
  ASSERT(!ustr_len(s1));  
  ASSERT((sp1 = ustrp_dup_empty(pool)));
  ASSERT(!ustrp_len(sp1));
  ASSERT(ustrp_add(pool, &sp1, USTRP(line999)));
  ASSERT_PEQ(sp1, USTRP(line999));
  ASSERT( ustrp_len(sp1));
  ASSERT(ustrp_io_putfileline(pool, &sp1, fp));
  ASSERT(!ustrp_len(sp1));
  ASSERT(ustrp_io_putfile(pool, &sp1, fp));
  
  rewind(fp); tst_getline(fp, USTR_FALSE);
  rewind(fp); tst_getline(fp, USTR_TRUE);

  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT(ustr_del(&s1, 1)); /* remove \n */
  ASSERT_EQ(s1, line999);
  ustr_sc_del(&s1);
  ASSERT(ustr_io_getfile(&s1, fp));
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_getfile(&s1, fp));

  rewind(fp);

  ASSERT(ustr_set(&s1, line1));
  ASSERT(ustr_add(&s1, line2));
  ASSERT(ustr_add(&s1, line3));
  ASSERT(ustr_add(&s1, line4));
  ASSERT(ustr_add(&s1, line5));
  ASSERT(ustr_add(&s1, line999));
  ASSERT(ustr_add_cstr(&s1, "\n"));

  ASSERT(ustr_io_putfilename(&s1, ustr_cstr(s2), "wb"));
  
  rewind(fp); tst_getline(fp, USTR_FALSE);
  rewind(fp); tst_getline(fp, USTR_TRUE);

  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT(ustr_del(&s1, 1)); /* remove \n */
  ASSERT_EQ(s1, line999);
  ASSERT(!ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line999);
  ustr_sc_del(&s1);
  ASSERT(!ustr_io_get(&s1, fp, 1, NULL));
  ASSERT(ustr_io_getfile(&s1, fp));
  ASSERT(!ustr_len(s1));

  rewind(fp);
  
  ASSERT(ustrp_io_getline(pool, &sp1, fp));
  ASSERT_EQ(&sp1->s, line1);
  ustrp_sc_del(pool, &sp1);
  ASSERT(ustrp_io_getdelim(pool, &sp1, fp, '\n'));
  ASSERT_EQ(&sp1->s, line2);
  ustrp_sc_del(pool, &sp1);
  ASSERT(ustrp_io_getline(pool, &sp1, fp));
  ASSERT_EQ(&sp1->s, line3);
  ustrp_sc_del(pool, &sp1);
  ASSERT(ustrp_io_getline(pool, &sp1, fp));
  ASSERT_EQ(&sp1->s, line4);
  ustrp_sc_del(pool, &sp1);
  ASSERT(ustrp_io_getline(pool, &sp1, fp));
  ASSERT_EQ(&sp1->s, line5);
  ustrp_sc_del(pool, &sp1);
  ASSERT(ustrp_io_getfile(pool, &sp1, fp));
  ASSERT(ustrp_del(pool, &sp1, 1)); /* remove \n */
  ASSERT_EQ(&sp1->s, line999);
  ustrp_sc_del(pool, &sp1);
  ASSERT(!ustrp_io_get(pool, &sp1, fp, 1, NULL));
  ASSERT(!ustrp_len(sp1));
  ASSERT(ustrp_io_getfile(pool, &sp1, fp));
  ASSERT(!ustrp_len(sp1));
  
  fclose(fp);
  
  remove(ustr_cstr(s2));
  ASSERT(!ustrp_io_getfilename(pool, &sp1, ustr_cstr(s2)));

  ustrp_sc_free2(pool, &sp1, USTRP(""));
  ASSERT(!ustrp_len(sp1));
  ASSERT(ustrp_io_putfilename(pool, &sp1, ustr_cstr(s2), "wb"));
  ASSERT(!ustrp_len(sp1));
  ASSERT(ustrp_io_getfilename(pool, &sp1, ustr_cstr(s2)));
  ASSERT(!ustrp_len(sp1));

  remove(ustr_cstr(s2));

  ASSERT(ustr_add_rep_chr(&s1, '-', 80 * 1000));
  ASSERT(ustr_io_putfilename(&s1, ustr_cstr(s2), "wb"));
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_getfilename(&s1, ustr_cstr(s2)));
  ASSERT(ustr_len(s1) == (80 * 1000));
  ASSERT(ustr_spn_cstr_fwd(s1, 0, "-") == (80 * 1000));
  
  remove(ustr_cstr(s2));

  ASSERT((sp1 = ustrp_dup_buf(pool, ustr_cstr(s2), ustr_len(s2))));
  
  ASSERT(ustr_cmp_buf_eq(s2, ustrp_cstr(sp1), ustrp_len(sp1)));

  errno = 0;
  ASSERT(!ustr_io_putfilename(&s2, "doesn't exist.txt", "rb"));
#ifdef __linux__
  ASSERT(errno == ENOENT);
#endif
  errno = 0;
  ASSERT(!ustr_io_putfilename(&s2, "./doesn't exist/foo.txt", "rb"));
#ifdef __linux__
  ASSERT(errno == ENOENT);
#endif
  ASSERT(ustr_cmp_buf_eq(s2, ustrp_cstr(sp1), ustrp_len(sp1)));

#ifdef ENOSPC
  do
  {
    FILE *filechk = fopen("/dev/full", "wb");

    if (!filechk) /* some versions of mock didn't have it */
    {
      ret = EXIT_FAILED_OK;
      break;
    }
    fclose(filechk);
    
    ASSERT(!ustr_io_putfilename(&s2, "/dev/full", "wb"));
    ASSERT(errno == ENOSPC);
    ASSERT(ustr_cmp_buf_eq(s2, ustrp_cstr(sp1), ustrp_len(sp1)) ||
           !ustr_len(s2));

    ASSERT(ustrp_add(pool, &sp1, sp1));
    ASSERT(ustrp_add(pool, &sp1, sp1));
    ASSERT(ustrp_add(pool, &sp1, sp1));
    ASSERT(ustrp_add(pool, &sp1, sp1));
    ASSERT(ustrp_add(pool, &sp1, sp1));
    ASSERT(ustrp_add(pool, &sp1, sp1));
    ASSERT(ustrp_add(pool, &sp1, sp1));
    ASSERT(ustrp_add(pool, &sp1, sp1));
    ASSERT(!ustrp_io_putfilename(pool, &sp1, "/dev/full", "wb"));
    ASSERT(errno == ENOSPC);
  } while (FALSE);
#else
  ret = EXIT_FAILED_OK;
#endif
  
  ustr_pool_free(pool);

  return (ret);
}
