#include "ustr.h"

#include <getopt.h>
#include <errno.h>

static int combine = USTR_FALSE;
static int interp  = USTR_FALSE;

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit (EXIT_FAILURE);
}

static void usage(const char *prog_name, int xcode)
{
  fprintf((xcode == EXIT_SUCCESS) ? stdout : stderr, "\
 Format: %s [-chiV] <string> [string]...\n\
           -c = Combine arguments into a single netstr.\n\
           -i = Interpret arguments from C-escaping.\n\
           -h = Print help.\n\
           -V = Print version.\n\
", prog_name);
  
  exit (xcode);
}

int cescape_decode(Ustr **ps1)
{ /* convert \n \t etc. into their correct bytes */
  size_t len = 0;
  char  *src = NULL;
  char  *dst = NULL;

  if (!ustr_srch_chr_fwd(*ps1, '\\'))
    return (USTR_TRUE);

  USTR_ASSERT(ustr_owner(*ps1));
  /* -- added 0.99.2
   *  if (!ustr_sc_ensure_owner(ps1))
   *    return (USTR_FALSE);
   */
  
  src = ustr_wstr(*ps1);
  len = ustr_len(*ps1);
  dst = src;
  while (len--)
  {
    if ((*src != '\\') || !len)
    {
     normal:
      *dst++ = *src++;
    }
    else
    {
      --len; ++src;
      switch (*src)
      {
        default:
          goto normal;
        case 'b':  ++src; *dst++ = '\b'; break;
        case 'f':  ++src; *dst++ = '\f'; break;
        case 'n':  ++src; *dst++ = '\n'; break;
        case 'r':  ++src; *dst++ = '\r'; break;
        case 't':  ++src; *dst++ = '\t'; break;
        case '\\': ++src; *dst++ = '\\'; break;

        case 'o':
        case 'O':
          if ((len < 1) || !isdigit((unsigned char)src[1]))
          {
            *dst++ = '\\';
            goto normal;
          }
          --len; ++src;

          /* FALL THROUGH */

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        {
          unsigned char val = 0;

          if (USTR_FALSE)
          { }
          else if ((len >= 2) && isdigit((unsigned char)src[1]) &&
                   isdigit((unsigned char)src[2]))
          {
            val =  (*src++ - '0');
            val *= 8;
            val += (*src++ - '0');
            val *= 8;
            val += (*src++ - '0');
            len -= 2;
          }
          else if ((len >= 1) && isdigit((unsigned char)src[1]))
          {
            val =  (*src++ - '0');
            val *= 8;
            val += (*src++ - '0');
            len -= 1;
          }
          else
            val = (*src++ - '0');

          *dst++ = val;
          break;
        }
        
        case 'x':
        case 'X':
        {
          unsigned char val = 0;
          Ustr *bconv = USTR1(\x16, "0123456789ABCDEFabcdef");
          Ustr *uconv = USTR1(\x10, "0123456789ABCDEF");
          Ustr *lconv = USTR1(\x6, "abcdef");
          size_t pos = 0;
          
          if ((len < 2) || /* only does \xDD and not \xDDDD like C */
              !ustr_srch_chr_fwd(bconv, src[1]) ||
              !ustr_srch_chr_fwd(bconv, src[2]))
          {
            *dst++ = '\\';
            goto normal;
          }

          if (!(pos = ustr_srch_chr_fwd(uconv, src[1])))
            pos = ustr_srch_chr_fwd(lconv, src[1]) + 10;
          val = pos - 1;
          val *= 16;
          if (!(pos = ustr_srch_chr_fwd(uconv, src[2])))
            pos = ustr_srch_chr_fwd(lconv, src[2]) + 10;
          val += pos - 1;
          *dst++ = val;
          
          len -= 2;
          src += 3;
        }
        break;
      }
    }
  }

  USTR_ASSERT(src - dst);
  ustr_del(ps1, src - dst); /* chop the end, if we converted */
  
  return (USTR_TRUE);
}

int main(int argc, char *argv[])
{
  struct option long_options[] =
  {
   {"combine", no_argument, NULL, 'c'},
   {"interpret", no_argument, NULL, 'i'},
   
   {"help", no_argument, NULL, 'h'},
   {"version", no_argument, NULL, 'V'},
   
   {NULL, 0, NULL, 0}
  };
  const char *prog_name = NULL;
  Ustr **nstrs = NULL;
  Ustr *out = NULL;
  int scan = 0;
  size_t total = 0;
  int optchar = -1;
  
  if (!argc)
    exit (EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  while ((optchar = getopt_long(argc, argv, "chiV", long_options, NULL)) != -1)
    switch (optchar)
    {
      case '?': usage(prog_name, EXIT_FAILURE);
      case 'h': usage(prog_name, EXIT_SUCCESS);
      case 'V':
        printf("%s version %s\n", prog_name, "1.0.0");
        exit (EXIT_SUCCESS);

      case 'c': combine = !combine; break;
      case 'i': interp  = !interp;  break;
    }

  argc -= optind;
  argv += optind;

  if (!argc)
    usage(prog_name, EXIT_FAILURE);
  
  if (!(nstrs = calloc(argc, sizeof(Ustr *))))
    die(prog_name, strerror(ENOMEM));

  scan = 0;
  while (scan < argc)
  {
    size_t len = strlen(argv[scan]);

    if (!interp)
    {
      if (!(nstrs[scan] = ustr_dup_fmt("%zu:%s,", len, argv[scan])))
        die(prog_name, strerror(ENOMEM));
    }
    else
    {
      Ustr *tmp = NULL;
      
      if (!(tmp = ustr_dup_cstr(argv[scan])))
        die(prog_name, strerror(ENOMEM));
      if (!(cescape_decode(&tmp)))
        die(prog_name, strerror(ENOMEM));      
      
      if (!(nstrs[scan] = ustr_dup_fmt("%zu:", ustr_len(tmp))))
        die(prog_name, strerror(ENOMEM));
      if (!(ustr_add(&nstrs[scan], tmp)))
        die(prog_name, strerror(ENOMEM));
      if (!(ustr_add_rep_chr(&nstrs[scan], ',', 1)))
        die(prog_name, strerror(ENOMEM));
      
      ustr_free(tmp);
    }
    
    total += ustr_len(nstrs[scan++]);
  }

  if (!combine)
  { /* Output one per. line */
    scan = 0;
    while (scan < argc)
    {
      if (!ustr_io_putfileline(&nstrs[scan], stdout))
        die(prog_name, strerror(errno));
      ustr_free(nstrs[scan++]);
    }
  }
  else
  {
    size_t sz = 0;
    size_t rbytes = 0;
  
    sz = total + (sizeof(size_t) * 4) + 6; /* close to total size needed */
    if (!(out = ustr_dupx_fmt(sz, rbytes, USTR_TRUE, USTR_FALSE, "%zu:",total)))
      die(prog_name, strerror(ENOMEM));

    scan = 0;
    while (scan < argc)
    {
      ustr_add(&out, nstrs[scan]);
      ustr_free(nstrs[scan++]); /* to make it allocation "clean" */
    }
    
    ustr_add_cstr(&out, ",\n");
  
    if (ustr_enomem(out)) /*  Not really needed for the multi arg. case unless,
                           * we screwed up sz. */
      die(prog_name, strerror(ENOMEM));
    
    if (!ustr_io_putfile(&out, stdout))
      die(prog_name, strerror(errno));
    
    ustr_free(out); /* to make it allocation "clean" */
  }
  
  return (EXIT_SUCCESS);
}

