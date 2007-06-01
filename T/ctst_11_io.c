#include "tst.h"

static const char *rf = __FILE__;

/* 1234567 */

static const Ustr *line1 = USTR1(\x11, "#include \"tst.h\"\n");
static const Ustr *line2 = USTR1(\x1, "\n");
static const Ustr *line3 = USTR1(\x22, "static const char *rf = __FILE__;\n");
static const Ustr *line4 = USTR1(\x1, "\n");
static const Ustr *line5 = USTR1(\xE, "/* 1234567 */\n");
static const Ustr *line999 = USTR1(\xa1, /* 161 */
"123456789 " "123456789 " "123456789 " "123456789 " "123456789 "
"123456789 " "123456789 " "123456789 " "123456789 " "123456789 "
"123456789 " "123456789 " "123456789 " "123456789 " "123456789 "
                                   "123456789 " "\n");

static void tst_getline(FILE *fp)
{
  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line1);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line2);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line3);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line4);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line5);

  ustr_sc_del(&s1);
  ASSERT(!ustr_len(s1));
}

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make();
  Ustrp *sp1 = USTRP("");
  FILE *fp = fopen("T/ctst_11_io.c", "rb");

  ASSERT(pool);
  ASSERT(fp);

  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_get(&s1, fp, 0));
  ASSERT(!ustr_len(s1));
  ASSERT(ustr_io_get(&s1, fp, 11));
  ASSERT(ustr_len(s1) >= 11);
  ASSERT(ustr_io_get(&s1, fp, 20));
  ASSERT(ustr_len(s1) >= 31);
  ASSERT(ustr_io_get(&s1, fp, 40));
  ASSERT(ustr_len(s1) >= 71);
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
  
  rewind(fp);

  tst_getline(fp);
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
  ASSERT( ustr_len(s1));  
  ASSERT(ustr_io_putfile(&s1, fp));
  ASSERT(!ustr_len(s1));  
  ASSERT((sp1 = ustrp_dup_empty(pool)));
  ASSERT(!ustrp_len(sp1));
  ASSERT(ustrp_add(pool, &sp1, USTRP(line999)));
  ASSERT_PEQ(sp1, USTRP(line999));
  ASSERT( ustrp_len(sp1));
  ASSERT(ustrp_io_putfile(pool, &sp1, fp));
  ASSERT(!ustrp_len(sp1));
  ASSERT(ustrp_io_putfile(pool, &sp1, fp));
  
  rewind(fp);

  tst_getline(fp);
  ASSERT(ustr_io_getline(&s1, fp));
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

  ASSERT(ustr_io_putfilename(&s1, ustr_cstr(s2), "wb"));
  
  tst_getline(fp);
  ASSERT(ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line999);
  ASSERT(!ustr_io_getline(&s1, fp));
  ASSERT_EQ(s1, line999);
  ustr_sc_del(&s1);
  ASSERT(!ustr_io_get(&s1, fp, 1));
  ASSERT(ustr_io_getfile(&s1, fp));
  ASSERT(!ustr_len(s1));

  remove(ustr_cstr(s2));
  ASSERT(!ustrp_io_getfilename(pool, &sp1, ustr_cstr(s2)));

  rewind(fp);
  
  ustrp_sc_free2(pool, &sp1, USTRP(""));
  ASSERT(!ustrp_len(sp1));
  ASSERT(ustrp_io_putfilename(pool, &sp1, ustr_cstr(s2), "wb"));
  ASSERT(!ustrp_len(sp1));
  ASSERT(ustrp_io_getfilename(pool, &sp1, ustr_cstr(s2)));
  ASSERT(!ustrp_len(sp1));
  
  remove(ustr_cstr(s2));

  ustr_pool_free(pool);

  return (EXIT_SUCCESS);
}
