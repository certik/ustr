#include "ustr.h"
/* this is a simple program showing how you can make mkdir_p() with Ustr */

#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>


static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit (EXIT_FAILURE);
}

static int fu__mkdir_p(const Ustr *s1, int mode, size_t off, int ret)
{
  Ustr *allocd = USTR_NULL;
  char *ptr = NULL;

  if (mkdir(ustr_cstr(s1), mode) != -1)
    return (ret + 1);

  switch (errno)
  {
    case EEXIST: return (ret);
      
    case ENOENT: break;
    
    default:     return (-1);
  }

  if ((off = ustr_srch_chr_rev(s1, off, '/')) <= 1)
  {
    errno = EINVAL;
    return (-1);
  }
  --off; /* NOTE: offset moves from beg. to end */
  
  if (!ustr_owner(s1))
  { /* do it this way, so we can pass constant Ustr's to this function
     * and don't use ustr_sc_ensure_owner() so that we don't release a
     * reference */
    if (!(allocd = ustr_dup_buf(ustr_cstr(s1), ustr_len(s1))))
      return (-1); /* errno == ENOMEM, done by ustr */
    s1 = allocd;
  }
  
  ptr = ustr_wstr((Ustr *)s1);
  ptr[off] = 0;
  if ((ret = fu__mkdir_p(s1, mode, ustr_len(s1) - off, ret + 1)) != -1)
  {
    ptr[off] = '/';
    if (mkdir(ustr_cstr(s1), mode) == -1)
      ret = -1;
  }
  ustr_free(allocd);

  return (ret);
}

/* This returns -1, on error, or the number of directories created. */
static int mkdir_p(const Ustr *s1, int mode)
{
  return (fu__mkdir_p(s1, mode, 0, 0));
}

int main(int argc, char *argv[])
{
  const char *prog_name = NULL;
  int scan = 1;
  Ustr *s1 = USTR("");
  
  USTR_CNTL_MALLOC_CHECK_BEG(USTR_DEBUG);
  
  if (!argc)
    exit (EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];

#ifdef TST
  mkdir_p(USTR1(\x24, "/tmp/abcd/1/2/3/4/5/6/7/8/9/10/11/12"), 0700, prog_name);
#endif

  if (argc == 1)
    die(prog_name, "No arguments given");
  
  while (scan < argc)
  {
    int num = 0;
    
    if (!ustr_set_cstr(&s1, argv[scan++]))
      die(prog_name, strerror(ENOMEM));
    if ((num = mkdir_p(s1, 0700)) == -1)
    {
      int sve_err = errno;
      
      /* leaking memory, but we don't care */
      die(ustr_cstr(ustr_dup_fmt("%s: mkdir(\"%s\")", prog_name,
                                 ustr_cstr(s1))), strerror(sve_err));
    }


    if (num)
    {
      Ustr *msg = ustr_dup_fmt(" Created %d leaves of: ", num);

      if (!msg)
        die(prog_name, strerror(ENOMEM));
        
      if (!ustr_io_putfile(&msg, stdout))
        die(prog_name, strerror(errno));
      
      if (!ustr_io_putfileline(&s1, stdout))
        die(prog_name, strerror(errno));
      
      ustr_free(msg);
    }
  }

  ustr_free(s1);
  
  USTR_CNTL_MALLOC_CHECK_END();
  
  return (EXIT_SUCCESS);
}
