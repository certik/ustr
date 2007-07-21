#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  /* single char sep; do not keep sep; do keep blank strings */
  Ustr *a = ustr_dup_cstr("blue,yellow,red,green,,orange");
  const char *ans[] = {"blue","yellow","red","green","","orange"};
  size_t off = 0;
  unsigned int flags = (USTR_FLAG_SPLIT_RET_NON);
  Ustr *sep = ustr_dup_cstr(",");
  Ustr *tok;
  Ustr *ret;
  size_t i=0;
  const char *ans2[] = {"a==","b,c==","d,c==","==","==",",12==","xzy==","=="};
  const char *ans3[] = {"a","bx","cx","dx","xx"};
  const char *ans4[] = {"a,bx","cx","dx",",xx"};
  
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
  flags = (USTR_FLAG_SPLIT_RET_SEP);
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
  ASSERT((ret = ustr_split(a,&off,sep,flags)) == USTR_FALSE);
  ASSERT(off==0);
  ustr_sc_free(&ret);
  /* blank target */
  sep = ustr_dup_cstr(",");
  ustr_sc_free(&a);
  a = USTR("");
  ASSERT((ret = ustr_split(a,&off,sep,flags)) == USTR_FALSE);
  ASSERT(off==0);

  ustr_sc_free(&sep);
  ustr_sc_free(&ret);

  a = USTR1(\x15, "a||bx||||cx||dx||||xx");
  i   = 0;
  off = 0;
  while ((tok = ustr_split_cstr(a, &off, "||", 0)))
  {
    ASSERT(!ustr_sized(a));
    ASSERT(!ustr_sized(tok));
    ASSERT(ustr_cmp_cstr_eq(tok, ans3[i++]));
    ustr_sc_free(&tok);
  }

  ASSERT((a = ustr_dupx(1, 0, 0, 0, a)));
  
  off = 1;
  ASSERT((tok = ustr_split_cstr(a, &off, "||", USTR_FLAG_SPLIT_RET_NON)));
  ASSERT( ustr_sized(a));
  ASSERT(!ustr_sized(tok));
  ASSERT( ustr_ro(tok));
  ASSERT(!ustr_len(tok));
  ustr_sc_free(&tok);
  off = 1;
  ASSERT((tok = ustr_split_cstr(a, &off, "||", USTR_FLAG_SPLIT_RET_SEP)));
  ASSERT( ustr_sized(a));
  ASSERT(!ustr_sized(tok));
  ASSERT(!ustr_ro(tok));
  ASSERT(ustr_cmp_cstr_eq(tok, "||"));
  ustr_sc_free(&tok);
  ASSERT((tok = ustr_split_cstr(a, &off, "||", USTR_FLAG_SPLIT_RET_SEP)));
  ASSERT( ustr_sized(a));
  ASSERT(!ustr_sized(tok));
  ASSERT(!ustr_ro(tok));
  ASSERT(ustr_cmp_cstr_eq(tok, "bx||"));
  ustr_sc_free(&tok);
    
  i   = 2;
  while ((tok = ustr_split_cstr(a, &off, "||", USTR_FLAG_SPLIT_KEEP_CONF)))
  {
    ASSERT( ustr_sized(a));
    ASSERT( ustr_sized(tok));
    ASSERT(ustr_cmp_cstr_eq(tok, ans3[i++]));
    ustr_sc_free(&tok);
  }

  off = 0;
  ASSERT(!ustr_split(a, &off, a, 0));
  ASSERT((off == ustr_len(a)));
  off = 0;
  ASSERT((tok = ustr_split(a, &off, a, (USTR_FLAG_SPLIT_RET_NON |
                                        USTR_FLAG_SPLIT_KEEP_CONF))));
  ASSERT((off == ustr_len(a)));
  ASSERT( ustr_sized(tok));
  ASSERT( ustr_cstr(tok));
  ASSERT_EQ(tok, USTR(""));
  ustr_sc_free(&tok);
  off = 0;
  ASSERT((tok = ustr_split(a, &off, a, (USTR_FLAG_SPLIT_RET_SEP |
                                        USTR_FLAG_SPLIT_KEEP_CONF))));
  ASSERT((off == ustr_len(a)));
  ASSERT_EQ(tok, a);
  ASSERT( ustr_sized(tok));
  ASSERT( ustr_cstr(tok));
  ustr_sc_free(&tok);
  
  ustr_sc_free(&a);
  
  return (EXIT_SUCCESS);
}
