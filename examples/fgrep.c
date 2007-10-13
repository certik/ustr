#include "ustr.h"
/* this is a simple program showing how you can make fgrep --color, with Ustr */

#include <getopt.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h> /* -r */

#ifndef _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(d) strlen((d)->d_name)
#endif

static const Ustr *colour_beg = USTR1(\x8, "\x1B[01;31m");
static const Ustr *colour_end = USTR1(\x8, "\x1B[00m\x1B[K");

static Ustr *fgrep_srch = USTR_NULL;
static Ustr *fgrep_repl = USTR_NULL;

static Ustr *printable_fname = USTR("");

static enum {
 COLOUR_AUTO,
 COLOUR_ON,
 COLOUR_SUB_AUTO,
 COLOUR_OFF
} colour_when = COLOUR_AUTO;

static int first_only = USTR_FALSE;

static int ignore_case = USTR_FALSE;

static enum {
 PRNT_FNAME_AUTO,
 PRNT_FNAME_ON,
 PRNT_FNAME_OFF
} prnt_fname = PRNT_FNAME_AUTO;

static int prnt_line_num = USTR_FALSE;

static int recurse = USTR_FALSE;

static const char *prog_name = NULL;

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit (EXIT_FAILURE);
}

static void usage(int xcode)
{
  fprintf((xcode == EXIT_SUCCESS) ? stdout : stderr, "\
 Format: %s [-hV] <string> [filename]...\n\
           -h           = Print help.\n\
           -C --colour  = When to do colour.\n\
           -R --replace = Replacement string.\n\
           -V           = Print version.\n\
", prog_name);
  
  exit (xcode);
}

static int grep_srch_only = USTR_FALSE;
static int fgrep(Ustr **ps1)
{
  size_t num = 0;

  if (!ignore_case)
  {
    if (grep_srch_only)
      num = ustr_srch_fwd(*ps1, 0, fgrep_srch);
    else if (!(num = ustr_replace(ps1, fgrep_srch, fgrep_repl,
                                  !first_only)) && errno)
      die(prog_name, strerror(ENOMEM));
  }
  else
  {
    if (grep_srch_only)
      num = ustr_srch_case_fwd(*ps1, 0, fgrep_srch);
    else
    { /* so the output is in the correct case, we need to do it "each" hit
         separately, so we can't have a ustr_replace function */
      size_t pos = 0;

      while ((pos = ustr_srch_case_fwd(*ps1, pos, fgrep_srch)))
      {
        ++num;

        if (!ustr_ins(ps1, pos - 1 + ustr_len(fgrep_srch), colour_end) ||
            !ustr_ins(ps1, pos - 1,                        colour_beg))
          die(prog_name, strerror(ENOMEM));

        pos += ustr_len(fgrep_srch) - 1;
        pos += ustr_len(colour_beg);
        pos += ustr_len(colour_end);
      }
    }
  }
  
  if (!num)
    ustr_sc_del(ps1);

  return (!!num);
}

/* "Simple" Boyer-Moore-Horspool
 * http://en.wikipedia.org/wiki/Boyer-Moore-Horspool_algorithm
 */
static unsigned char fast__off_bad[256];
static void fast_analyze(void)
{
  const unsigned char *ptr = (unsigned char *)ustr_cstr(fgrep_srch);
  size_t olen = ustr_len(fgrep_srch);
  size_t scan = 0;
  
  if (olen > 255) /* it'll be fast "enough", probably */
  {
    ptr += (olen - 255);
    olen = 255;
  }

  /* init. "bad" offset table to max offset */
  memset(fast__off_bad, olen, sizeof(fast__off_bad));

  /* init. "bad" offset table from data */
  --olen;
  while (scan < olen)
  {
    USTR_ASSERT(*ptr <= 255);

    fast__off_bad[ptr[scan]] = olen - scan;

    if (ignore_case &&
        (((ptr[scan] >= 0x41) && (ptr[scan] <= 0x5A)) ||
         ((ptr[scan] >= 0x61) && (ptr[scan] <= 0x7A))))
      fast__off_bad[ptr[scan] ^ 0x20] = olen - scan;

    ++scan;
  }
}

static void *fast_memcasemem(const void *passed_hsptr, size_t hslen)
{
  const unsigned char *hsptr = passed_hsptr;
  const unsigned char *ndptr = (unsigned char *)ustr_cstr(fgrep_srch);
  size_t ndlen = ustr_len(fgrep_srch);
  
  while (hslen >= ndlen)
  {
    size_t last = ndlen - 1;
    size_t scan = last;
    size_t used = 0;
    size_t off  = 0;

    while (USTR_TRUE)
    {
      unsigned char c1 = hsptr[scan];
      unsigned char c2 = ndptr[scan];
      
      if ((c1 >= 0x61) && (c1 <= 0x7a))
        c1 ^= 0x20;
      if ((c2 >= 0x61) && (c2 <= 0x7a))
        c2 ^= 0x20;

      if (c1 != c2)
        break;
      
      if (!scan--)
        return ((void *)hsptr);
    }

    used = last - scan;
    off  = fast__off_bad[hsptr[scan]];
    if (off <= used)
      off = 1; /* make sure we move at least 1 byte :) */
    else
      off -= used;

    hsptr += off;
    hslen -= off;
  }

  return (NULL);
}

static int fast_check(int fd, const char *prog_name)
{
  char tbuf[32 * 1024];
  size_t off = 0;
  ssize_t len = 0;
  
  while ((len = read(fd, tbuf + off, sizeof(tbuf) - off)) > 0)
  {
    char *ptr = NULL;
    
    len += off;

    if (fast_memcasemem(tbuf, len))
      return (USTR_TRUE);

    if (!(ptr = memrchr(tbuf, '\n', len)))
    {
      if (len < (ssize_t)sizeof(tbuf))
        continue;
      
      die(prog_name, strerror(ENOMEM));
    }
      
    off = (ptr - tbuf) + 1;
    memmove(tbuf, tbuf + off, len - off);
    off = len - off;
  }

  if (len == -1)
    return (USTR_TRUE);
  
  return (USTR_FALSE);
}

static void fp_loop(FILE *in, const char *prog_name)
{
  char buf_line[128]; /* can grow */
  Ustr *line = USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0);
  size_t line_num = 0;
  
  if (USTR_TRUE)
  {
    int fd = fileno(in);
    off_t off = 0;

    if ((off = lseek(fd, 0, SEEK_CUR)) != -1)
    {
      if (!fast_check(fd, prog_name))
        return;

      if (lseek(fileno(in), off, SEEK_SET) == -1)
        die(prog_name, strerror(errno));
    }
  }
  
  while (ustr_io_getline(&line, in))
  {
    ++line_num;
    
    if (fgrep(&line))
    {
      if (prnt_fname == PRNT_FNAME_ON)
      {
        Ustr *tmp = ustr_dup(printable_fname);
        if (!ustr_io_putfile(&tmp, stdout))
          die(prog_name, strerror(errno));
      }

      if (prnt_line_num)
        if (fprintf(stdout, "%ju:", line_num) == -1)
          die(prog_name, strerror(errno));
      
      if (!ustr_io_putfile(&line, stdout))
        die(prog_name, strerror(errno));
    }
    
    if (line != USTR(buf_line)) /* re-init */
      ustr_sc_free2(&line, USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0));
  }
  if (errno)
    die(prog_name, strerror(errno));
}

static void file_loop(Ustr *fname, const char *prog_name, int sure_file)
{
  FILE *fp = NULL;
  char buf[32 * 1024];
  struct stat st[1];
  
  if (!sure_file &&
      ((stat(ustr_cstr(fname), st) == -1) || !S_ISREG(st->st_mode)))
    return;
  
  if (!(fp = fopen(ustr_cstr(fname), "rb")))
    die(prog_name, strerror(errno));
  
  setbuffer(fp, buf, sizeof(buf));
  
  if (!ustr_add_cstr(&fname, ":"))
    die(prog_name, strerror(ENOMEM));
  
  printable_fname = fname;
  fp_loop(fp, prog_name);
  
  if (fclose(fp) == EOF)
    die(prog_name, strerror(errno));
}

static void loop(Ustr *, const char *);
static void dir_loop(const Ustr *fname, DIR *dir, const char *prog_name)
{
  struct dirent *ent = NULL;
  
  while ((ent = readdir(dir)))
  {
    Ustr *dname = NULL;
    size_t len = _D_EXACT_NAMLEN(ent);
    
    if (ustr_cmp_buf_eq(USTR1(\1, "."),  ent->d_name, len) ||
        ustr_cmp_buf_eq(USTR1(\2, ".."), ent->d_name, len))
      continue;
    
    if (!(dname = ustr_dup(fname)) ||
        !ustr_add_cstr(&dname, "/") ||
        !ustr_add_buf(&dname, ent->d_name, len))
      die(prog_name, strerror(ENOMEM));

#ifndef _DIRENT_HAVE_D_TYPE
    loop(dname, prog_name);
#else
    switch (ent->d_type)
    {
      case DT_UNKNOWN: /* FALL THROUGH */
      case DT_LNK:     /* FALL THROUGH */
      case DT_DIR:     loop(dname, prog_name);                 break;
      case DT_REG:     file_loop(dname, prog_name, USTR_TRUE); break;
        
      default:
        continue;
    }
#endif
  }

  if (closedir(dir) == -1)
    die(prog_name, strerror(errno));
}

static void loop(Ustr *fname, const char *prog_name)
{ /* NOTE: takes the reference to fname */
  DIR *dir = NULL;
  
  if ((dir = opendir(ustr_cstr(fname))))
    dir_loop(fname, dir, prog_name);
  else
    file_loop(fname, prog_name, USTR_FALSE);

  ustr_free(fname);
}


int main(int argc, char *argv[])
{
  struct option long_options[] =
  {
   {"with-filename",  no_argument, NULL, 'H'},
   {"no-filename",  no_argument, NULL, 'h'},
   {"without-filename",  no_argument, NULL, 'h'},
   {"recursive",  no_argument, NULL, 'r'},
   {"replace",    required_argument, NULL, 'R'},
   {"colour",     optional_argument, NULL, 'C'},
   {"color",      optional_argument, NULL, 'C'},
   {"first-only", no_argument,       NULL, 'F'},
   {"ignore-case", no_argument,      NULL, 'i'},
   {"line-number", no_argument,      NULL, 'n'},
   
   {"help", no_argument, NULL, 1},
   {"version", no_argument, NULL, 'V'},
   
   {NULL, 0, NULL, 0}
  };
  int scan = 0;
  int optchar = -1;

  USTR_CNTL_MALLOC_CHECK_BEG();
  if (!argc)
    exit (EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  while ((optchar = getopt_long(argc, argv,
                                "hirCFHRV", long_options, NULL)) != -1)
    switch (optchar)
    {
      case '?': usage(EXIT_FAILURE);
      case   1: usage(EXIT_SUCCESS);
      case 'V':
        printf("%s version %s\n", prog_name, "1.0.0");
        exit (EXIT_SUCCESS);

      case 'R':
        if (!(fgrep_repl = ustr_dupx_cstr(0, 0, USTR_TRUE, USTR_FALSE, optarg)))
          die(prog_name, strerror(ENOMEM));
        break;
      case 'C':
        if (ustr_cmp_case_cstr_eq(USTR1(\1, "0"),      optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\5, "never"),  optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\2, "no"),     optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\3, "off"),    optarg))
          colour_when = COLOUR_OFF;
        if (ustr_cmp_case_cstr_eq(USTR1(\1, "1"),      optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\6, "always"), optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\3, "yes"),    optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\2, "on"),     optarg))
          colour_when = COLOUR_ON;
        if (ustr_cmp_case_cstr_eq(USTR1(\4, "auto"),   optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\5, "maybe"),  optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\3, "tty"),    optarg))
          colour_when = COLOUR_AUTO;
        if (ustr_cmp_case_cstr_eq(USTR1(\5,  "sub-1"),      optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\xa, "sub-always"), optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\7,  "sub-yes"),    optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\6,  "sub-on"),     optarg))
          colour_when = COLOUR_ON;
        if (ustr_cmp_case_cstr_eq(USTR1(\x8, "sub-auto"),   optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\x9, "sub-maybe"),  optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\7,  "sub-tty"),    optarg))
          colour_when = COLOUR_SUB_AUTO;
        if (ustr_cmp_case_cstr_eq(USTR1(\x9, "replace-1"),      optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\xe, "replace-always"), optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\xb, "replace-yes"),    optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\xa, "replace-on"),     optarg))
          colour_when = COLOUR_ON;
        if (ustr_cmp_case_cstr_eq(USTR1(\x8, "replace-auto"),   optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\x9, "replace-maybe"),  optarg) ||
            ustr_cmp_case_cstr_eq(USTR1(\xb, "replace-tty"),    optarg))
          colour_when = COLOUR_SUB_AUTO;
        break;
      case 'F': first_only    = USTR_TRUE; break;
      case 'r': recurse       = USTR_TRUE; break;
      case 'i': ignore_case   = USTR_TRUE; break;
      case 'n': prnt_line_num = USTR_TRUE; break;
        
      case 'H': prnt_fname  = PRNT_FNAME_ON;  break;
      case 'h': prnt_fname  = PRNT_FNAME_OFF; break;
    }
  
  argc -= optind;
  argv += optind;

  if (!argc)
    usage(EXIT_FAILURE);

  if (!(fgrep_srch = ustr_dupx_cstr(0, 1, USTR_TRUE, USTR_FALSE, argv[0])))
    die(prog_name, strerror(ENOMEM));

  fast_analyze();
  
  if (!fgrep_repl)
    fgrep_repl = ustr_dup(fgrep_srch);
  else if (colour_when == COLOUR_AUTO)
    colour_when = COLOUR_OFF;

  if (colour_when == COLOUR_SUB_AUTO) colour_when = COLOUR_AUTO;
  
  if (colour_when == COLOUR_OFF)
    /* do nothing */ ;
  else if ((colour_when == COLOUR_AUTO) && !isatty(STDOUT_FILENO))
    /* do nothing */ ;
  else if (!ustr_ins(&fgrep_repl, 0, colour_beg) ||
           !ustr_add(&fgrep_repl,    colour_end))
    die(prog_name, strerror(ENOMEM));

  grep_srch_only = ustr_cmp_eq(fgrep_srch, fgrep_repl);
  
  argc -= 1;
  argv += 1;
  
  if (!argc)
  {
    if (prnt_fname == PRNT_FNAME_AUTO)
      prnt_fname = PRNT_FNAME_OFF;
    fp_loop(stdin, prog_name);
  }
  
  if (recurse || (argc > 1))
    if (prnt_fname == PRNT_FNAME_AUTO)
      prnt_fname = PRNT_FNAME_ON;
  
  scan = 0;
  while (scan < argc)
  {
    if (ustr_cmp_cstr_eq(USTR1(\1, "-"), argv[scan]))
      fp_loop(stdin, prog_name);
    else
    {
      Ustr *arg = USTR_NULL;
      
      if (!(arg = ustr_dup_cstr(argv[scan])))
        die(prog_name, strerror(ENOMEM));

      loop(arg, prog_name);
    }
    
    ++scan;
  }

  ustr_free(fgrep_srch);
  ustr_free(fgrep_repl);
  
  USTR_CNTL_MALLOC_CHECK_END();
  
  return (EXIT_SUCCESS);
}
