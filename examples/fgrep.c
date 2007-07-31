#include "ustr.h"
/* this is a simple program showing how you can make fgrep --color, with Ustr */

#include <getopt.h>
#include <unistd.h>
#include <errno.h>

static const char colour_beg[] = "\x1B[01;31m";
static const char colour_end[] = "\x1B[00m\x1B[K";

static Ustr *fgrep_srch = USTR_NULL;
static Ustr *fgrep_repl = USTR_NULL;

static Ustr *fname = USTR("");

static enum {
 COLOUR_AUTO,
 COLOUR_ON,
 COLOUR_SUB_AUTO,
 COLOUR_OFF
} colour_when = COLOUR_AUTO;

static int first_only = USTR_FALSE;

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

static int fgrep(Ustr **ps1)
{
  size_t num = 0;

  if (ustr_cmp_eq(fgrep_srch, fgrep_repl))
    num = ustr_srch_fwd(*ps1, 0, fgrep_srch);
  else if (!(num = ustr_replace(ps1, fgrep_srch, fgrep_repl, !first_only)) &&
           errno)
    die(prog_name, strerror(ENOMEM));

  if (!num)
    ustr_sc_del(ps1);

  return (!!num);
}

static void loop(FILE *in, const char *prog_name)
{
  char buf_line[128]; /* can grow */
  Ustr *line = USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0);

  while (ustr_io_getline(&line, in))
  {
    if (fgrep(&line))
    {
      Ustr *tmp = ustr_dup(fname);
      if (!ustr_io_putfile(&tmp, stdout))
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

int main(int argc, char *argv[])
{
  struct option long_options[] =
  {
   {"replace",    required_argument, NULL, 'R'},
   {"colour",     optional_argument, NULL, 'C'},
   {"color",      optional_argument, NULL, 'C'},
   {"first-only", no_argument,       NULL, 'F'},
   
   {"help", no_argument, NULL, 'h'},
   {"version", no_argument, NULL, 'V'},
   
   {NULL, 0, NULL, 0}
  };
  int scan = 0;
  int optchar = -1;
  
  if (!argc)
    exit (EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  while ((optchar = getopt_long(argc, argv, "hCFRV", long_options, NULL)) != -1)
    switch (optchar)
    {
      case '?': usage(EXIT_FAILURE);
      case 'h': usage(EXIT_SUCCESS);
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
      case 'F': first_only = !first_only; break;
    }
  
  argc -= optind;
  argv += optind;

  if (!argc)
    usage(EXIT_FAILURE);

  if (!(fgrep_srch = ustr_dupx_cstr(0, 1, USTR_TRUE, USTR_FALSE, argv[0])))
    die(prog_name, strerror(ENOMEM));
  if (!fgrep_repl)
    fgrep_repl = ustr_dup(fgrep_srch);
  else if (colour_when == COLOUR_AUTO)
    colour_when = COLOUR_OFF;

  if (colour_when == COLOUR_SUB_AUTO) colour_when = COLOUR_AUTO;
  
  if (colour_when == COLOUR_OFF)
    /* do nothing */ ;
  else if ((colour_when == COLOUR_AUTO) && !isatty(STDOUT_FILENO))
    /* do nothing */ ;
  else if (!ustr_ins_cstr(&fgrep_repl, 0, colour_beg) ||
           !ustr_add_cstr(&fgrep_repl,    colour_end))
    die(prog_name, strerror(ENOMEM));
  
  argc -= 1;
  argv += 1;
  
  if (!argc)
    loop(stdin, prog_name);
  
  scan = 0;
  while (scan < argc)
  {
    if (ustr_cmp_cstr_eq(USTR1(\1, "-"), argv[scan]))
      loop(stdin, prog_name);
    else
    {
      FILE *fp = fopen(argv[scan], "rb");
      
      if (!fp)
        die(prog_name, strerror(errno));

      if (!(fname = ustr_dup_fmt("%s:", argv[scan])))
        die(prog_name, strerror(ENOMEM));
      
      loop(fp, prog_name);

      ustr_sc_free2(&fname, USTR(""));
      
      if (ferror(fp))
        die(prog_name, strerror(errno));
      
      fclose(fp);
    }
    
    ++scan;
  }

  ustr_free(fgrep_srch);
  ustr_free(fgrep_repl);
  
  return (EXIT_SUCCESS);
}
