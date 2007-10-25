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

#include <stdarg.h> /* va_list for va_arg() functionality */

/* second set of defaults... *sigh* */
struct Ustr_opts USTR__COMPILE_ATTR_H() ustr__opts[1] = {
 {1, /* ref bytes */
  {malloc, realloc, free}, /* ustr */
  USTR_FALSE,   /* has_size */
  USTR_FALSE}}; /* exact_bytes */

/* not namespaced because this must be in a C-file. */
#ifndef USE_MALLOC_CHECK
#define USE_MALLOC_CHECK 1
#endif
#define MALLOC_CHECK_SCOPE_EXTERN 0
#include "malloc-check.h"

MALLOC_CHECK_DECL();

typedef struct Ustr__cntl_mc_ptrs
{
 const char *file;
 unsigned int line;
 const char *func;
} Ustr__cntl_mc_ptrs;
Ustr__cntl_mc_ptrs USTR__COMPILE_ATTR_H() *ustr__cntl_mc_ptr = 0;
size_t             USTR__COMPILE_ATTR_H()  ustr__cntl_mc_num = 0;
size_t             USTR__COMPILE_ATTR_H()  ustr__cntl_mc_sz  = 0;

USTR_CONF_i_PROTO void *ustr__cntl_mc_malloc(size_t x)
{
  struct Ustr__cntl_mc_ptrs *ptr = ustr__cntl_mc_ptr;
  size_t num = ustr__cntl_mc_num;
  
  return (malloc_check_malloc(x, ptr[num].file, ptr[num].line, ptr[num].func));
}
USTR_CONF_i_PROTO void *ustr__cntl_mc_realloc(void *p, size_t x)
{
  struct Ustr__cntl_mc_ptrs *ptr = ustr__cntl_mc_ptr;
  size_t num = ustr__cntl_mc_num;
  
  return (malloc_check_realloc(p,x, ptr[num].file,ptr[num].line,ptr[num].func));
}
USTR_CONF_i_PROTO void ustr__cntl_mc_free(void *x)
{
  struct Ustr__cntl_mc_ptrs *ptr = ustr__cntl_mc_ptr;
  size_t num = ustr__cntl_mc_num;
  
  malloc_check_free(x, ptr[num].file, ptr[num].line, ptr[num].func);
}

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

    case 666:
    {
      unsigned long valT = va_arg(ap, unsigned long);
      int enabled = ustr__cntl_mc_sz;

      USTR_ASSERT(ustr__cntl_mc_num <= ustr__cntl_mc_sz);
      
      switch (valT)
      {
        case 0xF0F0:
        case 0xF0F1:
        case 0x0FF0:
        case 0x0FF1:
        case 0x0FF2:
        case 0x0FF3:
        case 0x0FF4:
        case 0x0FFF:
          ret = USTR_TRUE;
      }
      USTR_ASSERT(ret);

      if (!enabled && (valT != 0x0FF0))
        break; /* pretend it worked */
      
      switch (valT)
      {
        case 0xF0F0:
        {
          unsigned long valV = va_arg(ap, unsigned long);
          MALLOC_CHECK_FAIL_IN(valV);
        }
        break;

        case 0xF0F1:
        {
          unsigned long *valV = va_arg(ap, unsigned long *);
          *valV = MALLOC_CHECK_STORE.mem_fail_num;
        }
        break;
        
        case 0x0FF0:
        {
          const char  *file = va_arg(ap, char *);
          unsigned int line = va_arg(ap, unsigned int);
          const char  *func = va_arg(ap, char *);

          if (!enabled)
          {
            ustr__cntl_mc_sz = 3;
            if (!(ustr__cntl_mc_ptr = MC_MALLOC(sizeof(Ustr__cntl_mc_ptrs) *
                                                ustr__cntl_mc_sz)))
              abort(); /* In theory we can fail here... */
          
            ustr__opts->ustr.sys_malloc  = ustr__cntl_mc_malloc;
            ustr__opts->ustr.sys_realloc = ustr__cntl_mc_realloc;
            ustr__opts->ustr.sys_free    = ustr__cntl_mc_free;
          }
          else if (++ustr__cntl_mc_num >= ustr__cntl_mc_sz)
          {
            ustr__cntl_mc_sz = (ustr__cntl_mc_sz * 2) + 1;
            if (!(ustr__cntl_mc_ptr = MC_REALLOC(ustr__cntl_mc_ptr,
                                                 sizeof(Ustr__cntl_mc_ptrs) *
                                                 ustr__cntl_mc_sz)))
              abort(); /* In theory we can fail here... */
          }
          
          ustr__cntl_mc_ptr[ustr__cntl_mc_num].file = file;
          ustr__cntl_mc_ptr[ustr__cntl_mc_num].line = line;
          ustr__cntl_mc_ptr[ustr__cntl_mc_num].func = func;
        }
        break;

        case 0x0FF1:
        {
          struct Ustr__cntl_mc_ptrs *ptr = ustr__cntl_mc_ptr;
          size_t num = ustr__cntl_mc_num;
          void *valP = va_arg(ap, void *);
          malloc_check_mem(valP, ptr[num].file, ptr[num].line, ptr[num].func);
        }
        break;

        case 0x0FF2:
        {
          struct Ustr__cntl_mc_ptrs *ptr = ustr__cntl_mc_ptr;
          size_t num = ustr__cntl_mc_num;
          void *valP  = va_arg(ap, void *);
          size_t valV = va_arg(ap, size_t);
          malloc_check_sz_mem(valP, valV,
                              ptr[num].file, ptr[num].line, ptr[num].func);
        }
        break;

        case 0x0FF3:
        {
          struct Ustr__cntl_mc_ptrs *ptr = ustr__cntl_mc_ptr;
          size_t num = ustr__cntl_mc_num;
          void *valP  = va_arg(ap, void *);
          size_t valV = va_arg(ap, size_t);
          malloc_check_minsz_mem(valP, valV,
                                 ptr[num].file, ptr[num].line, ptr[num].func);
        }
        break;

        case 0x0FF4:
        { /* needed for realloc() down doesn't fail */
          struct Ustr__cntl_mc_ptrs *ptr_mc = ustr__cntl_mc_ptr;
          size_t num_mc = ustr__cntl_mc_num;
          void *ptr = va_arg(ap, void *);
          size_t nsz = va_arg(ap, size_t);
          unsigned int scan = malloc_check_mem(ptr,
                                               ptr_mc[num_mc].file,
                                               ptr_mc[num_mc].line,
                                               ptr_mc[num_mc].func);

          MALLOC_CHECK_STORE.mem_vals[scan].sz = nsz;
        }
        break;

        case 0x0FFF:
        {
          const char  *file = va_arg(ap, char *);
          unsigned int line = va_arg(ap, unsigned int);
          const char  *func = va_arg(ap, char *);

          USTR_ASSERT(!strcmp(file, ustr__cntl_mc_ptr[ustr__cntl_mc_num].file));
          USTR_ASSERT(line); /* can't say much about this */
          USTR_ASSERT(!strcmp(func, ustr__cntl_mc_ptr[ustr__cntl_mc_num].func));
          
          if (ustr__cntl_mc_num)
            --ustr__cntl_mc_num;
          else
          {
            ustr__cntl_mc_sz = 0;
            MC_FREE(ustr__cntl_mc_ptr);

            /* it's bad otherwise */
            malloc_check_empty(file, line, func);
          }
          
          ustr__opts->ustr.sys_malloc  = malloc;
          ustr__opts->ustr.sys_realloc = realloc;
          ustr__opts->ustr.sys_free    = free;

          MALLOC_CHECK_STORE.mem_num      = 0;
          MALLOC_CHECK_STORE.mem_fail_num = 0;
        }
        break;
      }
    }
    break;
  }

  USTR_ASSERT(ret);
  
  va_end(ap);

  return (ret);
}
