#include "ustr.h"
/* This is a simple program showing "basename" */

#include <errno.h>

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  fflush(stderr);
  exit (EXIT_FAILURE);
}

/* 1.0.3 is buggy */
size_t u_cspn_chr_rev(const struct Ustr *s1, size_t off, char chr)
{
  size_t f_pos = ustr_srch_chr_rev(s1, off, chr);

  if (!f_pos)
    return (ustr_len(s1) - off);
    
  return ((ustr_len(s1) - f_pos) - off);
}

static int u_basename(struct Ustr **ps1)
{
  size_t off  = 0;
  size_t len  = ustr_len(*ps1);
  size_t llen = 0;

  if (!len)
    return (ustr_set(ps1, USTR("")));

  llen = ustr_cspn_chr_rev(*ps1, 0, '/');
  if (!llen)
  {
    off  = ustr_spn_chr_rev(*ps1, off, '/');
    llen = u_cspn_chr_rev(*ps1, off, '/');
  }

  if (!llen)
    return (ustr_set(ps1, USTR1(\1, "/")));
  
  return (ustr_set_subustr(ps1, *ps1, (len - (off + llen)) + 1, llen));
}

int main(int argc, char *argv[])
{
  const char *prog_name = NULL;
  Ustr *s1 = USTR_NULL;
  
  if (!argc)
    exit (EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  if (argc != 2)
    die(prog_name, "missing operand");

  if (!(s1 = ustr_dup_cstr(argv[1])))
    die(prog_name, strerror(errno));

  if (!u_basename(&s1))
    die(prog_name, strerror(errno));
  
  if (!ustr_io_putfileline(&s1, stdout))
    die(prog_name, strerror(errno));

  USTR_CNTL_MALLOC_CHECK_END();
  
  exit (EXIT_SUCCESS);
}
