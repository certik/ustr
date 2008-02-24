
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

#define ASSERT_EQ(x, y) do {                                            \
      if (ustr_cmp_eq(x, y)) {} else {                                  \
        fprintf(stderr, " -=> ASSERT_EQ (%zu:\"%s\", %zu:\"%s\") failed " \
                "in (%s) from %d %s.\n",                                \
                ustr_len(x), ustr_cstr(x), ustr_len(y), ustr_cstr(y),   \
                __func__, __LINE__, __FILE__);                          \
        abort(); }                                                      \
    } while (FALSE)
#define assert_eq(x, y) ASSERT_EQ(x, y)
#define ASSERT_PEQ(x, y) do {                                           \
      if (ustrp_cmp_eq(x, y)) {} else {                                 \
        fprintf(stderr, " -=> ASSERT_PEQ (%zu:\"%s\", %zu:\"%s\") failed " \
                "in (%s) from %d %s.\n",                                \
                ustrp_len(x), ustrp_cstr(x), ustrp_len(y), ustrp_cstr(y), \
                __func__, __LINE__, __FILE__);                          \
        abort(); }                                                      \
    } while (FALSE)
#define assert_peq(x, y) ASSERT_PEQ(x, y)

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

#define MALLOC_CHECK__ATTR_H() /* do nothing */

#define USE_MALLOC_CHECK 1

#include "malloc-check.h"

/* doesn't work on ctst_*.c cotst_*.c -- as they alloc. from the lib. */
#define USTR_CONF_MALLOC(x)     MC_MALLOC(x)
#define USTR_CONF_REALLOC(x, y) MC_REALLOC(x, y)
#define USTR_CONF_FREE(x)       MC_FREE(x)


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FALSE 0
#define TRUE  1


#include "ustr-debug.h"

static int tst(void); /* fwd */

#define EXIT_FAILED_OK 77

static struct Ustr *s1 = USTR("");
static struct Ustr *s2 = USTR1(\x02, "s2");

static const char *rf;

MALLOC_CHECK_DECL();

#if USTR_CONF_USE_DYNAMIC_CONF
static void *mc_malloc(size_t x)
{ (void)x; return (NULL); }
static void *mc_realloc(void *p, size_t x)
{ (void)p; (void)x; return (NULL); }
static void mc_free(void *x)
{ (void) x; }

static unsigned long mc_mem_fail_num = 0;
#define TST_MC_SET_NUM(x) assert(ustr_cntl_opt(666, 0xF0F0, x))
#define TST_MC_GET_NUM() (ustr_cntl_opt(666, 0xF0F1, &mc_mem_fail_num) ? mc_mem_fail_num : 0xFFFFFFFFU)
#else

#define TST_MC_SET_NUM(x) MALLOC_CHECK_STORE.mem_fail_num = (x)
#define TST_MC_GET_NUM()  MALLOC_CHECK_STORE.mem_fail_num
#endif

int main(void)
{
  int ret = -1;
  struct Ustr *s2_chk1 = USTR1_CHK(\2, "s2");
  struct Ustr *s2_chk2 = USTR1_CHK(\x2, "s2");
  struct Ustr *s2_chk3 = USTR1_CHK(\x02, "s2");
  struct Ustr *s2_chk4 = USTR2_CHK(\0, \x02, "s2");
  struct Ustr *s2_chk8 = USTR4_CHK(\0, \0, \0, \x02, "s2");

  ASSERT(ustr_len(s2_chk1));
  ASSERT(ustr_len(s2_chk2));
  ASSERT(ustr_len(s2_chk3));
  ASSERT(ustr_len(s2_chk4));
  ASSERT(ustr_len(s2_chk8));
  
  assert(USTR_CNTL_MALLOC_CHECK_MEM("")); /* not enabled yet */
  
#if USTR_CONF_USE_DYNAMIC_CONF
  {
    static const struct Ustr_cntl_mem mc_mem = {mc_malloc, mc_realloc, mc_free};

    ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_SET_MEM, &mc_mem));
    
    assert(USTR_CNTL_MALLOC_CHECK_BEG(USTR_TRUE));

    assert(!TST_MC_GET_NUM());
  }
#else
  assert(!USTR_CNTL_MALLOC_CHECK_BEG(USTR_TRUE));
#endif
  
  ASSERT(ustr_size(s1) == 0);
  ASSERT(ustr_size(s2) == 2);
  ASSERT(ustr_ro(s1));
  ASSERT(ustr_ro(s2));
  ASSERT(ustr_dup(s1) == s1);
  ASSERT(ustr_dup(s2) == s2);
  
  assert(USTR_CNTL_MALLOC_CHECK_MEM_USTR(s2));
  ASSERT(ustr_sc_ensure_owner(&s2));
  ASSERT(s2);
  assert(USTR_CNTL_MALLOC_CHECK_MEM(s2));
  assert(USTR_CNTL_MALLOC_CHECK_MEM_USTR(s2));
  ASSERT(!ustr_ro(s2));
  
  {
    size_t esz;
    size_t ref;
    int exact;
    size_t lenn;
    size_t refc;

#if USTR_CONF_USE_DYNAMIC_CONF
    int conf_esz;
    size_t conf_ref;
    int conf_exact;
    ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_HAS_SIZE,    &conf_esz));
    ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_REF_BYTES,   &conf_ref));
    ASSERT(ustr_cntl_opt(USTR_CNTL_OPT_GET_EXACT_BYTES, &conf_exact));
#endif

    ustr_conf(s1, NULL,NULL,NULL, NULL,NULL);
    ustr_conf(s1, &esz,&ref,&exact, &lenn,&refc);
#if USTR_CONF_USE_DYNAMIC_CONF
    ASSERT(!conf_esz   == !esz);
    ASSERT( conf_ref   ==  ref);
    ASSERT( conf_exact ==  exact);
#else
    ASSERT(!USTR_CONF_HAS_SIZE    == !esz);
    ASSERT( USTR_CONF_REF_BYTES   ==  ref);
    ASSERT( USTR_CONF_EXACT_BYTES ==  exact);
#endif
    ustr_conf(s2, NULL,NULL,NULL, NULL,NULL);
    ustr_conf(s2, &esz,&ref,&exact, &lenn,&refc);
#if USTR_CONF_USE_DYNAMIC_CONF
    ASSERT(!conf_esz   == !esz);
    ASSERT( conf_ref   ==  ref);
    ASSERT( conf_exact ==  exact);
#else
    ASSERT(!USTR_CONF_HAS_SIZE    == !esz);
    ASSERT( USTR_CONF_REF_BYTES   ==  ref);
    ASSERT( USTR_CONF_EXACT_BYTES ==  exact);
#endif
  }
  
  if ((ret = tst()) && (ret != EXIT_FAILED_OK))
    fprintf(stderr, "Error(%s) value = %x\n", rf, ret);
  else
  {
    ustr_free(s1);
    ustr_free(s2);
    if (!USTR_CONF_USE_DYNAMIC_CONF)
      MALLOC_CHECK_EMPTY();
    assert(USTR_CNTL_MALLOC_CHECK_END());
  }
  
  switch (ret)
  {
    case EXIT_FAILED_OK: exit (EXIT_FAILED_OK);
    case EXIT_SUCCESS:   exit (EXIT_SUCCESS);
    default:             exit (EXIT_FAILURE);
  }
}
