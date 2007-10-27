/* #define USTR_CONF_COMPILE_USE_INLINE 0 */
#include "ustr.h"

#include <getopt.h>
#include <errno.h>
#include <ctype.h>

static int combine  = USTR_FALSE;
static int interp   = USTR_FALSE;
static int fnames   = USTR_FALSE;

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit (EXIT_FAILURE);
}

static void usage(const char *prog_name, int xcode)
{
  fprintf((xcode == EXIT_SUCCESS) ? stdout : stderr, "\
 Format: %s [-cfhiV] <string> [string]...\n\
           -c = Combine arguments into single line.\n\
           -f = Interpret arguments as filenames.\n\
           -i = Interpret arguments from C-escaping.\n\
           -h = Print help.\n\
           -V = Print version.\n\
", prog_name ? prog_name : "ustr");
  
  exit (xcode);
}

/* NOTE: copied from netstr ... */
/* This function is implemented showing how you can simply use the Ustr as
   a container for storing a growable byte array+length, and then easily do
   the more complex operations the same way you would if you managed the
   entire thing by hand using just a malloc() and storing the length
   separately. */
static int cescape_decode(Ustr **ps1)
{ /* Convert \n \t etc. into their correct bytes. */
  size_t fpos = 0;
  size_t len = ustr_len(*ps1);
  char  *src = NULL;
  char  *dst = NULL;

  if (!(fpos = ustr_srch_chr_fwd(*ps1, 0, '\\')) || (fpos == len))
    return (USTR_TRUE);

  if (!(dst = src = ustr_sc_wstr(ps1)))
    return (USTR_FALSE);

  --fpos;
  src += fpos;
  dst += fpos;
  len -= fpos;
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

        case '\n': ++src; break;
        case '"':  ++src; break;
        case '\'': ++src; break;
          
        case 'b':  ++src; *dst++ = '\b'; break;
        case 'f':  ++src; *dst++ = '\f'; break;
        case 'n':  ++src; *dst++ = '\n'; break;
        case 'r':  ++src; *dst++ = '\r'; break;
        case 't':  ++src; *dst++ = '\t'; break;
        case 'v':  ++src; *dst++ = '\v'; break;
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
              !ustr_srch_chr_fwd(bconv, 0, src[1]) ||
              !ustr_srch_chr_fwd(bconv, 0, src[2]))
          {
            *dst++ = '\\';
            goto normal;
          }

          if (!(pos = ustr_srch_chr_fwd(uconv, 0, src[1])))
            pos = ustr_srch_chr_fwd(lconv, 0, src[1]) + 10;
          val = pos - 1;
          val *= 16;
          if (!(pos = ustr_srch_chr_fwd(uconv, 0, src[2])))
            pos = ustr_srch_chr_fwd(lconv, 0, src[2]) + 10;
          val += pos - 1;
          *dst++ = val;
          
          len -= 2;
          src += 3;
        }
        break;
      }
    }
  }

  ustr_del(ps1, src - dst); /* chop the end, if we converted */
  
  return (USTR_TRUE);
}

static int cescape_encode(Ustr **ret, const char *src, size_t len)
{ /* Convert to a C-style string ... lowest common denominator,
   * mostly use: \xXX. */
  const char *last = NULL;

  if (!ustr_sc_ensure_owner(ret))
    return (USTR_FALSE);
  
  ustr_add_rep_chr(ret, '"', 1);

  last = src;
  while (len--)
  {
    if ((*src == 0x20) || (*src == 0x21) || /* not 0x22 == " */
        ((*src >= 0x23) && (*src <= 0x7E)))
      ++src;
    else
    {
      if (last != src)
        ustr_add_buf(ret, last, src - last);

      switch (*src)
      {
        case '\b': ustr_add_cstr(ret, "\\b"); break;
        case '\f': ustr_add_cstr(ret, "\\f"); break;
        case '\n': ustr_add_cstr(ret, "\\n"); break;
        case '\r': ustr_add_cstr(ret, "\\r"); break;
        case '\t': ustr_add_cstr(ret, "\\t"); break;
        case '\v': ustr_add_cstr(ret, "\\v"); break;
        default:
          if (!len || /* no problems with merging */
              !(((src[1] >= '0') && (src[1] <= '9')) ||
                ((src[1] >= 'A') && (src[1] <= 'F')) ||
                ((src[1] >= 'a') && (src[1] <= 'f'))))
          {
            if (!*src)
              ustr_add_cstr(ret, "\\0");
            else
              ustr_add_fmt(ret, "\\x%02hhX", *src);
          }
          else if (!*src)
            ustr_add_cstr(ret, "\" \"\\0\" \"");
          else
            ustr_add_fmt(ret, "\\x%02hhX\" \"", *src);
      }

      ++src;

      last = src;
    }
  }

  if (last != src)
    ustr_add_buf(ret, last, src - last);
  
  ustr_add_rep_chr(ret, '"', 1);
  
  return (!ustr_enomem(*ret));
}

static Ustr *gen_custr_beg(size_t len)
{
  if (len > 65535)
    return (ustr_dup_fmt("USTR1(\\x%zX, \\x%02zX, \\x%02zX, \\x%02zX, ",
                         ((len >> 24) & 0xFF), ((len >> 16) & 0xFF),
                         ((len >>  8) & 0xFF), ((len >>  0) & 0xFF)));
  if (len >   255)
    return (ustr_dup_fmt("USTR2(\\x%zX, \\x%02zX, ",
                         ((len >>  8) & 0xFF), ((len >>  0) & 0xFF)));

  if (len >     7)
    return (ustr_dup_fmt("USTR1(\\x%zX, ", len));
  
  return (ustr_dup_fmt("USTR1(\\%zo, ", len));
}

static Ustr *gen_custr_buf(const char *buf, size_t len)
{
  Ustr *tmp = USTR_NULL;
  Ustr *ret = USTR_NULL;
  
  if (interp)
  {
    if (!(tmp = ustr_dup_buf(buf, len)))
      goto fail_mem;
    if (!cescape_decode(&tmp))
      goto fail_mem;
    if (!(ret = gen_custr_beg(ustr_len(tmp))))
      goto fail_mem;
    if (!cescape_encode(&ret, ustr_cstr(tmp), ustr_len(tmp)))
      goto fail_mem;
  }
  else
  {
    if (!(ret = gen_custr_beg(len)))
      goto fail_mem;
    if (!cescape_encode(&ret, buf, len))
      goto fail_mem;
  }

  if (!ustr_add_cstr(&ret, ");"))
    goto fail_mem;
  
  ustr_free(tmp);
  return (ret);

 fail_mem:
  ustr_free(tmp);
  ustr_free(ret);

  return (USTR_NULL);
}

static void custr(Ustr *data, const char *prog_name)
{
  Ustr *arg = USTR_NULL;
  
  if (!(arg = gen_custr_buf(ustr_cstr(data), ustr_len(data))))
    die(prog_name, strerror(errno));
  
  if (!ustr_io_putfileline(&arg, stdout))
    die(prog_name, strerror(errno));
  
  ustr_free(arg);      
}


int main(int argc, char *argv[])
{
  struct option long_options[] =
  {
   {"combine", no_argument, NULL, 'c'},
   {"interpret", no_argument, NULL, 'i'},
   {"filenames", no_argument, NULL, 'f'},
   
   {"help", no_argument, NULL, 'h'},
   {"version", no_argument, NULL, 'V'},
   
   {NULL, 0, NULL, 0}
  };
  int scan = 0;
  int optchar = -1;
  const char *prog_name = NULL;
  
  if (!argc)
    usage(NULL, EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  while ((optchar = getopt_long(argc, argv, "cfhiV", long_options,NULL)) != -1)
    switch (optchar)
    {
      case '?': usage(prog_name, EXIT_FAILURE);
      case 'h': usage(prog_name, EXIT_SUCCESS);
      case 'V':
        printf("%s version %s\n", prog_name, "1.0.0");
        exit (EXIT_SUCCESS);

      case 'c': combine = !combine; break;
      case 'f': fnames  = !fnames;  break;
      case 'i': interp  = !interp;  break;
    }

  argc -= optind;
  argv += optind;

  if (!argc)
    usage(prog_name, EXIT_FAILURE);

  if (fnames)
  {
    char buf_line[USTR_SIZE_FIXED(160)]; /* enough for two "normal" lines,
                                            after that we alloc. */
    Ustr *line = USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0);

    while (scan < argc)
    {
      FILE *fp = NULL;
      
      if (ustr_cmp_cstr_eq(USTR1(\1, "-"), argv[scan]))
        fp = stdin;
      else if (!(fp = fopen(argv[scan], "rb")))
        die(prog_name, strerror(errno));
      ++scan;

      if (combine)
      {
        if (!ustr_io_getfile(&line, fp))
          die(prog_name, strerror(errno));
        custr(line, prog_name);
      }
      else
      {
        while (ustr_io_getline(&line, fp))
        {
          ustr_del(&line, ustr_spn_chr_rev(line, 0, '\n'));
          
          custr(line, prog_name);

          ustr_sc_del(&line);
          if (line != USTR(buf_line)) /* re-init */
            ustr_sc_free2(&line, USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0));
        }
        if (errno)
          die(prog_name, strerror(errno));
      }

      if (line != USTR(buf_line)) /* re-init */
        ustr_sc_free2(&line, USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0));
      
      fclose(fp);
    }
  }
  else if (!combine)
    /* Output one per. line */
    while (scan < argc)
    {
      Ustr *arg = gen_custr_buf(argv[scan], strlen(argv[scan]));
      
      if (!arg || !ustr_io_putfileline(&arg, stdout))
        die(prog_name, strerror(errno));
      ustr_free(arg);
      
      ++scan;
    }
  else
  {
    char buf_args[USTR_SIZE_FIXED(160)]; /* enough for two "normal" lines,
                                            after that we alloc. */
    Ustr *args = USTR_SC_INIT_AUTO(buf_args, USTR_FALSE, 0);
    int done = USTR_FALSE;
    
    while (scan < argc)
    {
      if (done)
        ustr_add_cstr(&args, " ");
      done = USTR_TRUE;
      ustr_add_cstr(&args, argv[scan++]);
    }
    
    if (ustr_enomem(args))
      die(prog_name, strerror(ENOMEM));

    custr(args, prog_name);

    ustr_free(args);
  }
  
  USTR_CNTL_MALLOC_CHECK_END();
  
  return (EXIT_SUCCESS);
}

