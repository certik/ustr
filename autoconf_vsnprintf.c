#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#define USE_FMT_1_3 0

static int my_autoconf(const char *fmt, ...)
{
  va_list ap;
  char buf[1] = "";
  int ret = -1;

  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  return (ret);
}

#if 0
static char *my_fmt0(const char *fmt, ...)
{
  char *tmp = NULL;
  va_list ap;
  int ret = -1;
  size_t lim = 32;
  char buf[1024] = "";

  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

printf("fmt0\n");
printf("%d\n", ret);
  if (ret == -1)
    return (NULL);
  if ((size_t)ret > lim)
    return (NULL);

  if (!(tmp = malloc(ret + 1))) exit(1);
  
  va_start(ap, fmt);
  ret = vsnprintf(tmp, ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);
printf("%d\n", ret);
printf("\n");

  return (tmp);
}

static char *my_fmt1(const char *fmt, ...)
{
  char *tmp = NULL;
  va_list ap;
  int ret = -1;
  size_t sz = 1;

printf("fmt1\n");
  if (!(tmp = malloc(sz))) exit(1);
  tmp[0] = 0;
 
  va_start(ap, fmt);
  while ((ret = vsnprintf(tmp, sz, fmt, ap)) == -1)
  {
    printf("%d - %lu\n", ret, (unsigned long)strlen(tmp));
    sz <<= 1;

    if (!(tmp = realloc(tmp, sz))) exit(1);
    if (sz > 1024) exit(2);

    va_end(ap);
    va_start(ap, fmt);
  } 
    va_end(ap);

    if (sz <= (unsigned)ret)
    {
      sz = ret + 1;
      if (!(tmp = realloc(tmp, sz))) exit(1);

      va_start(ap, fmt);
      ret = vsnprintf(tmp, sz, fmt, ap);
      va_end(ap);
    }

printf("%d\n", ret);
printf("\n");

  return (tmp);
}
#endif

#if USE_FMT_1_3
static char *myx_vfmt(size_t lim, const char *fmt, va_list ap)
{
  char *tmp = NULL;
  va_list nap;
  int ret = -1;

  va_copy(nap, ap);
  ret = vsnprintf(NULL, 0, fmt, nap);
  va_end(nap);
  
printf("%d\n", ret);
  if (ret == -1)
    return (NULL);
  if ((size_t)ret > lim)
    return (NULL);

  if (!(tmp = malloc(ret + 1)))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (NULL);
  }
  
  ret = vsnprintf(tmp, ret + 1, fmt, ap); /* assuming it works now */
printf("%d\n", ret);
printf("\n");

  return (tmp);
}

static char *my_fmt2(const char *fmt, ...)
{
  va_list ap;
  char *ret = NULL;
  
  va_start(ap, fmt);
  ret = myx_vfmt(32, fmt, ap);
  va_end(ap);

  return (ret);
}

static char *my_vfmt(const char *fmt, va_list ap)
{ return (myx_vfmt(32, fmt, ap)); }

static char *my_fmt3(const char *fmt, ...)
{
  va_list ap;
  char *ret = NULL;
  
  va_start(ap, fmt);
  ret = my_vfmt(fmt, ap);
  va_end(ap);

  return (ret);
}
#endif

int main(void)
{
#if 0
  char *tmp0 = my_fmt0("a b %d c d", 42);
  char *tmp1 = my_fmt1("a b %d c d", 42);
#if USE_FMT_1_3
  char *tmp2 = my_fmt2("a b %d c d", 42);
  char *tmp3 = my_fmt3("a b %d c d", 42);
#endif

  if (tmp0) puts(tmp0); free(tmp0);
  if (tmp1) puts(tmp1); free(tmp1);
#if USE_FMT_1_3
  if (tmp2) puts(tmp2); free(tmp2);
  if (tmp3) puts(tmp3); free(tmp3);
#endif
#endif

  if (my_autoconf("12") == -1)
    puts("1");
  else
    puts("0");
  
  return 0;
}
