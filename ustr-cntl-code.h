/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_MAIN_H
#error " You should have already included ustr-main.h, or just include ustr.h."
#endif

/* not sure of the version */
#if defined(__GNUC__) && !defined(__STRICT_ANSI__) && (__GNUC__ > 3) && \
    USTR_CONF_COMPILE_USE_ATTRIBUTES
# define USTR__COMPILE_ATTR_H() __attribute__((visibility("hidden")))
#else
# define USTR__COMPILE_ATTR_H()
#endif

#include <stdarg.h> /* va_list for va_arg() like functionality */

/* second set of defaults... *sigh* */
struct Ustr_opts USTR__COMPILE_ATTR_H() ustr__opts[1] = {
 {2, /* ref bytes */
  {malloc, realloc, free}, /* ustr */
  USTR_FALSE,   /* has_size */
  USTR_FALSE}}; /* exact_bytes */

USTR_CONF_I_PROTO int ustr_cntl_opt(int option, ...)
{
  int ret = USTR_FALSE;
  va_list ap;

  va_start(ap, option);

  switch (option)
  {
    case USTR_CNTL_OPT_GET_REF_BYTES:
    {
      size_t *val = va_arg(ap, size_t *);

      *val = ustr__opts->ref_bytes;
      
      ret = USTR_TRUE;
    }
    break;
    
    case USTR_CNTL_OPT_SET_REF_BYTES:
    {
      size_t rbytes = va_arg(ap, size_t);

      USTR_ASSERT_RET((rbytes == 0) ||
                      (rbytes == 1) || (rbytes == 2) || (rbytes == 4) ||
                      (USTR_CONF_HAVE_64bit_SIZE_MAX && (rbytes == 8)),
                      USTR_FALSE);

      ustr__opts->ref_bytes = rbytes;
      
      ret = USTR_TRUE;
    }
    break;

    case USTR_CNTL_OPT_GET_HAS_SIZE:
    {
      int *val = va_arg(ap, int *);

      *val = ustr__opts->has_size;
      
      ret = USTR_TRUE;
    }
    break;
    
    case USTR_CNTL_OPT_SET_HAS_SIZE:
    {
      int val = va_arg(ap, int);

      USTR_ASSERT_RET((val == !!val), USTR_FALSE);

      ustr__opts->has_size = val;
      
      ret = USTR_TRUE;
    }
    break;

    case USTR_CNTL_OPT_GET_EXACT_BYTES:
    {
      int *val = va_arg(ap, int *);

      *val = ustr__opts->exact_bytes;
      
      ret = USTR_TRUE;
    }
    break;
    
    case USTR_CNTL_OPT_SET_EXACT_BYTES:
    {
      int val = va_arg(ap, int);

      USTR_ASSERT_RET((val == !!val), USTR_FALSE);

      ustr__opts->exact_bytes = val;
      
      ret = USTR_TRUE;
    }
    break;

    /* call backs */
    
    case USTR_CNTL_OPT_GET_MEM:
    {
      struct Ustr_cntl_mem *val = va_arg(ap, struct Ustr_cntl_mem *);

      val->sys_malloc  = ustr__opts->ustr.sys_malloc;
      val->sys_realloc = ustr__opts->ustr.sys_realloc;
      val->sys_free    = ustr__opts->ustr.sys_free;
      
      ret = USTR_TRUE;
    }
    break;
    
    case USTR_CNTL_OPT_SET_MEM:
    {
      const struct Ustr_cntl_mem *val = va_arg(ap, struct Ustr_cntl_mem *);

      ustr__opts->ustr.sys_malloc  = val->sys_malloc;
      ustr__opts->ustr.sys_realloc = val->sys_realloc;
      ustr__opts->ustr.sys_free    = val->sys_free;
      
      ret = USTR_TRUE;
    }
    break;

#if USTR_CONF_USE_MALLOC_CHECK
    case 666:
    {
      unsigned long valT = va_arg(ap, unsigned long);
      unsigned long valV = va_arg(ap, unsigned long);

      ASSERT(valT == 0xF0F0);
      
      if (0) {}
      else if (USTR_CONF_USE_MALLOC_CHECK   && (valT == 0xF0F0))
      { MALLOC_CHECK_FAIL_IN(valV); ret = TRUE; }
    }
    break;    
#endif

    default:
      USTR_ASSERT(!" Bad cntl option.");
      break;
  }

  va_end(ap);

  return (ret);
}
