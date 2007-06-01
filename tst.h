
#undef NDEBUG /* always use assert */
#undef assert
#undef ASSERT
#define assert(x) do {                                                  \
      if (x) {} else {                                                  \
        fprintf(stderr, " -=> ASSERT (%s) failed in (%s) from %d %s.\n", \
                #x , __func__, __LINE__, __FILE__);                     \
        abort(); }                                                      \
    } while (FALSE)
#define ASSERT(x) do {                                                  \
      if (x) {} else {                                                  \
        fprintf(stderr, " -=> ASSERT (%s) failed in (%s) from %d %s.\n", \
                #x , __func__, __LINE__, __FILE__);                     \
        abort(); }                                                      \
    } while (FALSE)

#define ASSERT_EQ(x, y) ASSERT(ustr_cmp_eq(x, y))
#define assert_eq(x, y) ASSERT(ustr_cmp_eq(x, y))
#define ASSERT_PEQ(x, y) ASSERT(ustrp_cmp_eq(x, y))
#define assert_peq(x, y) ASSERT(ustrp_cmp_eq(x, y))

/* doesn't work on ctst_*.c cotst_*.c -- as they alloc. from the lib. */
#define USTR_CONF_MALLOC(x)     malloc_check_malloc(x,     __FILE__, __LINE__)
#define USTR_CONF_REALLOC(x, y) malloc_check_realloc(x, y, __FILE__, __LINE__)
#define USTR_CONF_FREE(x)       malloc_check_free(x)

#if !defined(USTR_DEBUG) || USTR_DEBUG
# define ustr_assert(x)        assert(x)
# define USTR_ASSERT(x)        assert(x)

# ifndef ustr_assert_ret /* ustr_assert_valid() == FALSE testing */
# define ustr_assert_ret(x, y) assert(x)
# endif
# ifndef USTR_ASSERT_RET
# define USTR_ASSERT_RET(x, y) assert(x)
# endif
#endif

#define _GNU_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FALSE 0
#define TRUE  1

#define MALLOC_CHECK__ATTR_H() /* do nothing */

#include "malloc-check.h"

MALLOC_CHECK_DECL();

#include "ustr-debug.h"

static int tst(void); /* fwd */

#define EXIT_FAILED_OK 77

static struct Ustr *s1 = USTR("");
static struct Ustr *s2 = USTR1(\x02, "s2");

static const char *rf;

int main(void)
{
  int ret = -1;

  /* puts((const char *)s2); */
  ASSERT(!ustr_size(s1));
  ASSERT(!ustr_size(s2));
  ASSERT( ustr_ro(s1));
  ASSERT( ustr_ro(s2));
  ASSERT( ustr_dup(s1) == s1);
  ASSERT( ustr_dup(s2) == s2);
  
  s2 = ustr_dup_buf(ustr_cstr(s2), ustr_len(s2));
  ASSERT(s2);
  
  if ((ret = tst()) && (ret != EXIT_FAILED_OK))
    fprintf(stderr, "Error(%s) value = %x\n", rf, ret);
  else
  {
    ustr_free(s1);
    ustr_free(s2);
    malloc_check_empty();
  }
  
  switch (ret)
  {
    case EXIT_FAILED_OK: exit (EXIT_FAILED_OK);
    case EXIT_SUCCESS:   exit (EXIT_SUCCESS);
    default:             exit (EXIT_FAILURE);
  }
}
