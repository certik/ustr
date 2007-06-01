#include "ustr.h"

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

static void my_memp_free(void *x, void *y)
{
  (void)x;
  (void)y;
}

int main(void)
{
  size_t rbytes = 0;
  int has_sz = -1;
  int exact_bytes = -1;
  Ustr_cntl_mem  mymem;
  Ustr_cntl_memp mymemp;
  
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_REF_BYTES, &rbytes));
  ASSERT(rbytes == 2);
  
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

  mymem.sys_malloc  = 0;
  mymem.sys_realloc = 0;
  mymem.sys_free    = 0;
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
  
  mymemp.pool_sys_malloc  = 0;
  mymemp.pool_sys_realloc = 0;
  mymemp.pool_sys_free    = 0;
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MEMP, &mymemp));
  ASSERT(mymemp.pool_sys_malloc  == ustr_pool_sys_malloc);
  ASSERT(mymemp.pool_sys_realloc == ustr_pool_sys_realloc);
  ASSERT(mymemp.pool_sys_free    == ustr_pool_sys_free);
  mymemp.pool_sys_free = my_memp_free;
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_MEMP, &mymemp));  
  mymemp.pool_sys_malloc  = 0;
  mymemp.pool_sys_realloc = 0;
  mymemp.pool_sys_free    = 0;
  ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_MEMP, &mymemp));
  ASSERT(mymemp.pool_sys_malloc  == ustr_pool_sys_malloc);
  ASSERT(mymemp.pool_sys_realloc == ustr_pool_sys_realloc);
  ASSERT(mymemp.pool_sys_free    == my_memp_free);
  
  return 0;
}
