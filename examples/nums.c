/* note this only works upto9 uintmax_t size... unlike the Vstr version */

#include "ustr.h"

#include <getopt.h>
#include <errno.h>
#include <locale.h>

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit (EXIT_FAILURE);
}

static void usage(const char *prog_name, int xcode)
{
  fprintf((xcode == EXIT_SUCCESS) ? stdout : stderr, "\
 Format: %s <number>\n\
           -a = All localization, include hex, octal and binary output.\n\
           -l = Call system localization to get seperator and groupings.\n\
           -h = Print help.\n\
           -V = Print version.\n\
", prog_name ? prog_name : "nums");
  
  exit (xcode);
}

static void num2ustr(Ustr **buf, uintmax_t num, unsigned int base,
                     const char *steps, const char *sep)
{
  static const char digits[] = "0123456789ABCDEF";
  int st_num = 0;
  
  ustr_set_empty(buf);
  while (num)
  {
    if ((*steps > 0) && (*steps < CHAR_MAX) &&
        (st_num >= *steps))
    {
      ustr_add_cstr(buf, sep);
      st_num = 0;
      if (steps[1]) ++steps;
    }
    ustr_add_rep_chr(buf, digits[num % base], 1);
    ++st_num;
    num /= base;
  }
  if (!ustr_sc_reverse(buf))
    abort();
}

int main(int argc, char *argv[])
{
  struct option long_options[] =
  {
   {"localization", no_argument, NULL, 'l'},
   {"localisation", no_argument, NULL, 'l'},
   {"l10n",         no_argument, NULL, 'l'},
   
   {"all-localization", no_argument, NULL, 'a'},
   {"all-localisation", no_argument, NULL, 'a'},
   {"all-l10n",         no_argument, NULL, 'a'},
   
   {"help", no_argument, NULL, 'h'},
   {"version", no_argument, NULL, 'V'},
   
   {NULL, 0, NULL, 0}
  };
  const char *prog_name = NULL;
  int optchar = -1;
  int loc = USTR_FALSE;
  int all_loc = USTR_FALSE;
  Ustr *s1 = USTR_NULL;
  Ustr *out = USTR_NULL;
  char tout_buf[1024];
  Ustr *tout = USTR_SC_INIT_AUTO(tout_buf, USTR_FALSE, 0);
  const char *sep   = "_";
  const char *xsteps = "\4";
  const char *usteps = "\3";
  const char *osteps = "\3";
  const char *bsteps = "\4";
  uintmax_t num = 0;
  unsigned int flags = (USTR_FLAG_PARSE_NUM_SEP | USTR_FLAG_PARSE_NUM_OVERFLOW |
                        USTR_FLAG_PARSE_NUM_SPACE | USTR_FLAG_PARSE_NUM_EXACT);
  unsigned int ern = 0;
  
  if (!argc)
    usage(NULL, EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  while ((optchar = getopt_long(argc, argv, "alhV", long_options, NULL)) != -1)
    switch (optchar)
    {
      case '?': usage(prog_name, EXIT_FAILURE);
      case 'h': usage(prog_name, EXIT_SUCCESS);
      case 'V':
        printf("%s version %s\n", prog_name, "1.0.0");
        exit (EXIT_SUCCESS);

      case 'a': all_loc = !all_loc; break;
      case 'l': loc     = !loc;     break;
    }

  argc -= optind;
  argv += optind;
  
  if (argc != 1)
    usage(prog_name, EXIT_FAILURE);

  if (!(s1 = ustr_dup_cstr(argv[0])))
    die(prog_name, strerror(ENOMEM));
  if (!(out = ustr_dup_empty()))
    die(prog_name, strerror(ENOMEM));

  if (loc)
  {
    struct lconv *sys_loc = NULL;

    setlocale(LC_ALL, "");
    if ((sys_loc = localeconv()))
    {
      if (!(usteps = sys_loc->grouping))
        usteps = "";
      if (!(sep   = sys_loc->thousands_sep))
        usteps = "";
      if (all_loc)
      {
        xsteps = usteps;
        osteps = usteps;
        bsteps = usteps;
      }
    }
  }
  
  {
    uintmax_t num_min = INTMAX_MIN;
    num = ustr_parse_uintmaxx(s1, 0, flags, -num_min, UINTMAX_MAX, sep,
                              NULL, &ern);
  }
  
  ustr_add_fmt(&out, " Input: %s\n", argv[0]);
  if (ern)
    switch (ern)
    {
      case USTR_TYPE_PARSE_NUM_ERR_ONLY_S:
      case USTR_TYPE_PARSE_NUM_ERR_ONLY_SPM:
      case USTR_TYPE_PARSE_NUM_ERR_ONLY_SPMX:
      case USTR_TYPE_PARSE_NUM_ERR_OOB:
        ustr_add_fmt(&out, "  Error: %u: %s\n", ern, strerror(EINVAL));
        break;
        
      case USTR_TYPE_PARSE_NUM_ERR_OVERFLOW:
        ustr_add_fmt(&out, "  Error: %u: %s\n", ern, strerror(E2BIG));
        break;
        
        /* USTR_TYPE_PARSE_NUM_ERR_NEGATIVE 
           USTR_TYPE_PARSE_NUM_ERR_BEG_ZERO */
        
      default:
        ustr_add_fmt(&out, "  Error: %u\n", ern);
    }    

  ustr_add_cstr(&out, "    %#'x = 0x");
  num2ustr(&tout, num, 16, xsteps, sep);
  if (ustr_enomem(tout))
    die(prog_name, strerror(ENOMEM));
  ustr_add(&out, tout); ustr_add_rep_chr(&out, '\n', 1);
  
  ustr_add_cstr(&out, "    %#'u =   ");
  num2ustr(&tout, num, 10, usteps, sep);
  if (ustr_enomem(tout))
    die(prog_name, strerror(ENOMEM));
  ustr_add(&out, tout); ustr_add_rep_chr(&out, '\n', 1);
    
  ustr_add_cstr(&out, "    %#'o = 0o");
  num2ustr(&tout, num,  8, osteps, sep);
  if (ustr_enomem(tout))
    die(prog_name, strerror(ENOMEM));
  ustr_add(&out, tout); ustr_add_rep_chr(&out, '\n', 1);

  ustr_add_cstr(&out, "    %#'b = 0b");
  num2ustr(&tout, num,  2, bsteps, sep);
  if (ustr_enomem(tout))
    die(prog_name, strerror(ENOMEM));
  ustr_add(&out, tout); ustr_add_rep_chr(&out, '\n', 1);

  if (ustr_enomem(out))
    die(prog_name, strerror(ENOMEM));

  if (!ustr_io_putfile(&out, stdout))
    die(prog_name, strerror(errno));

  /* cleanup */
  ustr_free(s1);
  ustr_free(out);
  ustr_free(tout);
  
  USTR_CNTL_MALLOC_CHECK_END();
  
  exit (EXIT_SUCCESS);
}
