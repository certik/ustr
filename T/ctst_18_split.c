#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{

  /* single char sep; do not keep sep; do keep blank strings */
  Ustr *a = ustr_dup_cstr("blue,yellow,red,green,,orange");
  const char *ans[] = {"blue","yellow","red","green","","orange"};
  size_t off = 0;
  unsigned int flags = (USTR_SPLIT_RET_NON);
  Ustr *sep = ustr_dup_cstr(",");
  size_t i=0;
  if (ans) {};
  Ustr *tok;
  while ((tok = ustr_split(a,&off,sep,flags)))
  {
       ASSERT(ustr_cmp_cstr_eq(tok,ans[i]));
       ++i;
       ustr_sc_free(&tok);
  }
  ASSERT(i == 6);
  ustr_sc_free(&a);
  ustr_sc_free(&sep);
  /* mult-char sep; keep sep; do not keep blank strings */
  a = ustr_dup_cstr("a==b,c==d,c======,12==xzy====");
  sep = ustr_dup_cstr("==");
  flags = (USTR_SPLIT_RET_SEP);
  const char *ans2[] = {"a==","b,c==","d,c==","==","==",",12==","xzy==","=="};
  i=0;
  off=0;
  while ((tok = ustr_split(a,&off,sep,flags)))
  {
     ASSERT(ustr_cmp_cstr_eq(tok,ans2[i]));
     ++i;
     ustr_sc_free(&tok);
  }
  ASSERT(i == 8);
  ustr_sc_free(&a);
  ustr_sc_free(&sep);
  /* single-char sep; skip sep, skip blank */
  a = ustr_dup_cstr("a,bx,,,,cx,,dx,,,xx,,,,");
  sep = ustr_dup_cstr(",");
  flags = 0;
  const char *ans3[] = {"a","bx","cx","dx","xx"};
  i=0;
  off=0;
  while ((tok = ustr_split(a,&off,sep,flags)))
  {
     ASSERT(ustr_cmp_cstr_eq(tok,ans3[i]));
     ++i;
     ustr_sc_free(&tok);
  }
  ustr_sc_free(&sep);
  /* multi-char sep; skip sep; skip blank */
  sep = ustr_dup_cstr(",,");
  const char *ans4[] = {"a,bx","cx","dx",",xx"};
   i=0;
  off=0;
  while ((tok = ustr_split(a,&off,sep,flags)))
  {
     ASSERT(ustr_cmp_cstr_eq(tok,ans4[i]));
     ++i;
     ustr_sc_free(&tok);
  }
  ustr_sc_free(&sep);
  ASSERT(i==4);
  /* blank sep */
  off = 0;
  sep = USTR("");
  Ustr *ret;
  ASSERT((ret = ustr_split(a,&off,sep,flags)) == USTR_FALSE);
  ASSERT(off==0);
  ustr_sc_free(&ret);
  /* blank target */
  sep = ustr_dup_cstr(",");
  ustr_sc_free(&a);
  a = USTR("");
  ASSERT((ret = ustr_split(a,&off,sep,flags)) == USTR_FALSE);
  ASSERT(off==0);

  /* I'm not exactly sure how to test for KEEP_CONF */

  ustr_sc_free(&sep);
  ustr_sc_free(&ret);
  return EXIT_SUCCESS;
}
