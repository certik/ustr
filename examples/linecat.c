#include "ustr.h"
/* This is a simple program showing "cat", works a line at a time.
   Mainly an example of simplicity, Ie. Python's "for line in file():" */

#include <errno.h>

#define CAT_BUF   1
#define CAT_SIZED 2

#ifndef CAT_TYPE
#define CAT_TYPE CAT_SIZED
#endif

#if 0
#elif CAT_TYPE == CAT_BUF
# define fp_loop fp_buf_loop
#elif CAT_TYPE == CAT_SIZED
# define fp_loop fp_sized_loop
#endif

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  fflush(stderr);
  exit (EXIT_FAILURE);
}

static void udie(const char *prog_name, const char *fmtmsg, ...)
{
  va_list ap;
  Ustr *msg = USTR_NULL;
  
  va_start(ap, fmtmsg);
  msg = ustr_dup_vfmt(fmtmsg, ap);
  va_end(ap);

  if (!msg)
    errno = ENOMEM, die(prog_name, strerror(errno));
    
  fprintf(stderr, "%s: ", prog_name);
  if (!ustr_io_putfileline(&msg, stderr))
    die(prog_name, strerror(errno));
  
  fflush(stderr);
  exit (EXIT_FAILURE);
}


static void fp_buf_loop(FILE *in, FILE *out, const char *pname)
{
  char buf_line[USTR_SIZE_FIXED(160)]; /* enough for two "normal" lines,
                                          after that we alloc. */
  Ustr *line = USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0);

  while (ustr_io_getline(&line, in))
  {
    /* do something with line, this version doesn't touch malloc() at all which
     * is kind of nice ... although probably not worth it.
     * It also limits memory used, if there are any "abnormally large" lines. */
    
    if (!ustr_io_putfile(&line, out))
      die(pname, strerror(errno));
    
    if (line != USTR(buf_line)) /* re-init */
      ustr_sc_free2(&line, USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0));
  }
  if (errno)
    die(pname, strerror(errno));

  ustr_free(line);
}

static void fp_sized_loop(FILE *in, FILE *out, const char *pname)
{
  Ustr *line = ustr_dupx_empty(128, 0, USTR_TRUE, USTR_FALSE);

  if (!line)
    die(pname, strerror(errno));

  while (ustr_io_getline(&line, in))
  {
    /* do something with line, note that we can ustr_dup() for O(1) here.
     * This version keeps memory it allocates, which can be good or bad. */
    
    if (!ustr_io_putfile(&line, out))
      die(pname, strerror(errno));
  }
  if (errno)
    die(pname, strerror(errno));

  ustr_free(line);
}

int main(int argc, char *argv[])
{
  int scan = 1;
  const char *prog_name = NULL;
  
  if (!argc)
    exit (EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  if (scan == argc)
    fp_loop(stdin, stdout, prog_name);
  
  while (scan < argc)
  {
    if (ustr_cmp_cstr_eq(USTR1(\1, "-"), argv[scan]))
      fp_loop(stdin, stdout, prog_name);
    else
    {
      FILE *fp = NULL;
      
      if (!(fp = fopen(argv[scan], "rb")))
        udie(prog_name, "open(%s): %s", argv[scan], strerror(errno));
  
      fp_loop(fp, stdout, prog_name);

      if (fclose(fp) == EOF)
        die(prog_name, strerror(errno));
    }
    
    ++scan;
  }

  USTR_CNTL_MALLOC_CHECK_END();
  
  return (EXIT_SUCCESS);
}
