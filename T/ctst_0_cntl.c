#include "ustr-debug.h"

#define ASSERT(x) do {                                                  \
      if (x) {} else {                                                  \
        fprintf(stderr, " -=> ASSERT (%s) failed in (%s) from %d %s.\n", \
                #x , __func__, __LINE__, __FILE__);                     \
        abort(); }                                                      \
    } while (0)

static void my_mem_free(void *x)
{
  free(x);
}

int main(void)
{
  size_t rbytes = 0;
  int has_sz = -1;
  int exact_bytes = -1;
  int scrub = -1;
  Ustr_cntl_mem  mymem_tst = {0,0,0};
  Ustr_cntl_mem  mymem = {0,0,0};
  Ustr *s1 = USTR("");
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_REF_BYTES, &rbytes));
  ASSERT(rbytes == 1);
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_HAS_SIZE, &has_sz));
  ASSERT(has_sz == 0);
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_HAS_SIZE, 1));
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_HAS_SIZE, &has_sz));
  ASSERT(has_sz == 1);
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_EXACT_BYTES, &exact_bytes));
  ASSERT(exact_bytes == 0);
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_EXACT_BYTES, 1));
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_EXACT_BYTES, &exact_bytes));
  ASSERT(exact_bytes == 1);

  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_M_SCRUB, &scrub));
  ASSERT(scrub == 0);
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_MC_M_SCRUB, 1));
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_M_SCRUB, &scrub));
  ASSERT(scrub == 1);
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_F_SCRUB, &scrub));
  ASSERT(scrub == 0);
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_MC_F_SCRUB, 1));
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_F_SCRUB, &scrub));
  ASSERT(scrub == 1);
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_R_SCRUB, &scrub));
  ASSERT(scrub == 0);
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_MC_R_SCRUB, 1));
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_R_SCRUB, &scrub));
  ASSERT(scrub == 1);
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MEM, &mymem_tst));
  ASSERT(mymem_tst.sys_malloc  != malloc);
  ASSERT(mymem_tst.sys_realloc == realloc);
  ASSERT(mymem_tst.sys_free    == free);

  if (USTR_CONF_USE_ASSERT)
    ASSERT(!setenv("USTR_CNTL_MC", "no",     USTR_TRUE));
  else
    ASSERT(!setenv("USTR_CNTL_MC", "foobar", USTR_TRUE));
  
  ASSERT(ustr_sc_ensure_owner(&s1)); /* env testing done */
  ustr_sc_free2(&s1, USTR(""));
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_M_SCRUB, &scrub));
  ASSERT(scrub == 1);
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_F_SCRUB, &scrub));
  ASSERT(scrub == 1);
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_R_SCRUB, &scrub));
  ASSERT(scrub == 1);
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MEM, &mymem));
  ASSERT(mymem.sys_malloc  == malloc);
  ASSERT(mymem.sys_realloc == realloc);
  ASSERT(mymem.sys_free    == free);
  mymem.sys_free = my_mem_free;
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_MEM, &mymem));  
  mymem.sys_malloc  = 0;
  mymem.sys_realloc = 0;
  mymem.sys_free    = 0;
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MEM, &mymem));
  ASSERT(mymem.sys_malloc  == malloc);
  ASSERT(mymem.sys_realloc == realloc);
  ASSERT(mymem.sys_free    == my_mem_free);
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_MEM, &mymem_tst));

  ASSERT(!setenv("USTR_CNTL_MC", "yes", USTR_TRUE));
  ASSERT(!setenv("USTR_CNTL_MC_M_SCRUB", "no", USTR_TRUE));
  ASSERT(!setenv("USTR_CNTL_MC_F_SCRUB", "no", USTR_TRUE));
  ASSERT(!setenv("USTR_CNTL_MC_R_SCRUB", "no", USTR_TRUE));
  
  ASSERT(ustr_sc_ensure_owner(&s1)); /* env testing done, again */
  ustr_sc_free2(&s1, USTR(""));
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_M_SCRUB, &scrub));
  ASSERT(scrub == 0);
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_F_SCRUB, &scrub));
  ASSERT(scrub == 0);
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MC_R_SCRUB, &scrub));
  ASSERT(scrub == 0);
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MEM, &mymem));
  ASSERT(mymem.sys_malloc  != malloc);
  ASSERT(mymem.sys_realloc != realloc);
  ASSERT(mymem.sys_free    != free);
  
  if (!USTR_DEBUG)
    ASSERT(!ustr_cntl_opt(-1, &mymem));

  ustr_free(s1);

  USTR_CNTL_MALLOC_CHECK_END();
  
  return 0;
}
