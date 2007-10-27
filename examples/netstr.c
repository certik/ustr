
/* #define USTR_CONF_COMPILE_USE_INLINE 0 */
#include "ustr.h"

#include <getopt.h>
#include <errno.h>
#include <ctype.h>

static int combine = USTR_FALSE;
static int fnames  = USTR_FALSE;

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
           -C = Interpret arguments as CSV lists.\n\
           -f = Interpret arguments as filenames.\n\
           -h = Print help.\n\
           -V = Print version.\n\
", prog_name ? prog_name : "netstr");
  
  exit (xcode);
}

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

static Ustr *gen_netstr_buf(const char *buf, size_t len, int interp)
{
  Ustr *tmp = USTR_NULL;
  Ustr *ret = USTR_NULL;
  
  if (interp)
  {
    if (!(tmp = ustr_dup_buf(buf, len)))
      goto fail_mem;
    if (!cescape_decode(&tmp))
      goto fail_mem;
    if (!(ret = ustr_dup_fmt("%zu:", ustr_len(tmp))))
      goto fail_mem;
    ustr_add(&ret, tmp);
  }
  else
  {
    if (!(ret = ustr_dup_fmt("%zu:", len)))
      goto fail_mem;
    ustr_add_buf(&ret, buf, len);
  }
  
  ustr_add_rep_chr(&ret, ',', 1);

  if (ustr_enomem(ret))
    goto fail_mem;
  
  ustr_free(tmp);
  return (ret);

 fail_mem:
  ustr_free(tmp);
  ustr_free(ret);

  return (USTR_NULL);
}

typedef struct Nstrs
{
 Ustr **ptr;
 size_t sz;
 size_t num;
 size_t total;
} Nstrs;

static int nstrs_add_ustr(Nstrs *nstrs, Ustr *arg)
{
  if (nstrs->num >= nstrs->sz)
  {
    Ustr **tmp = NULL;
    
    nstrs->sz <<= 1;
    if (!(tmp = realloc(nstrs->ptr, nstrs->sz * sizeof(Ustr *))))
      return (USTR_FALSE);
    nstrs->ptr = tmp;
  }
  
  nstrs->ptr[nstrs->num] = arg;
  nstrs->total += ustr_len(nstrs->ptr[nstrs->num++]);

  return (USTR_TRUE);
}

static void nstrs_reset(Nstrs *nstrs, size_t num, size_t total)
{
  if (!nstrs)
    return;

  while (nstrs->num > num)
    ustr_sc_free(&nstrs->ptr[--nstrs->num]);
    
  nstrs->total = total;
}

static Nstrs *nstrs_init(void)
{
  Nstrs *nstrs = NULL;
  
  if (!(nstrs = malloc(sizeof(Nstrs))))
    goto fail_nstrs;

  nstrs->sz = 32;
  
  if (!(nstrs->ptr = calloc(nstrs->sz, sizeof(Ustr *))))
    goto fail_nstrs_ptr;

  nstrs->num   = 0;
  nstrs->total = 0;
  
  return (nstrs);

 fail_nstrs_ptr:
  free(nstrs);
 fail_nstrs:
  return (NULL);
}

static void nstrs_free(Nstrs *nstrs)
{
  if (!nstrs)
    return;

  nstrs_reset(nstrs, 0, 0);
  free(nstrs->ptr);
  free(nstrs);
}

static Ustr *gen_csv_netstr(Nstrs *nstrs, Ustr **pin, FILE *fp,
                            int csv, int interp)
{
  Ustr *in = *pin;
  size_t onum;
  size_t ototal;
  Ustr *ret = USTR_NULL;
  size_t off = 0;
  size_t len = ustr_len(in);
  size_t scan = 0;
  int saved_errno = -1;
  
  if (!csv)
    return (gen_netstr_buf(ustr_cstr(in), len, interp));

  onum   = nstrs->num;
  ototal = nstrs->total;

  while (off < len)
  {
    size_t tmp = 0;
    char end = '"';
    
    tmp = ustr_spn_cstr_fwd(in, off, " \t\v\r");
    if (tmp)
    {
      off += tmp;
      continue;
    }

    switch (*(ustr_cstr(in) + off))
    {
      case ',': /* empty entry */
        if (!nstrs_add_ustr(nstrs, USTR1(\3, "0:,")))
          goto fail_mem_csv;
        ++off;
        continue;

      default:
      { /* normal single entry */
        size_t spaces = 0;
        size_t data   = 0;

        do
        {
          tmp    += spaces + data;
          spaces  = ustr_spn_cstr_fwd(in, off + tmp, " \t\v\r");
          data    = ustr_cspn_cstr_fwd(in, off + tmp + spaces, " \t\v\r,");
        } while (data);

        if (!(ret = gen_netstr_buf(ustr_cstr(in) + off, tmp, USTR_FALSE)))
          goto fail_mem_csv;
        if (!nstrs_add_ustr(nstrs, ret))
          goto fail_mem_csv;
        ret = USTR_NULL;
        off += tmp + spaces;
      }
      break;
        
      case '\'': end = '\'';
      case '"':
      {
        char cur = 0;

        ++off;
        tmp = 0;
        while (((off + tmp) < len) || fp)
        {
          if ((off + tmp) >= len)
          { /* have a multi-line CSV entry, from a file */
            if (!ustr_add_rep_chr(pin, '\n', 1))
              goto fail_mem_csv;
            if (!ustr_io_getline(pin, fp))
              goto fail_bad_csv;
            ustr_del(pin, 1);
            in = *pin;
            len = ustr_len(in);
          }
          
          tmp += ustr_cspn_cstr_fwd(in, off + tmp, "\\\"'");

          cur = *(ustr_cstr(in) + off + tmp);
          if (cur == '\\')
            tmp += 2;
          else if (cur != end)
            tmp += 1;
          else
            break;
        }

        if (cur != end)
          goto fail_bad_csv;
        
        if (!(ret = gen_netstr_buf(ustr_cstr(in) + off, tmp, USTR_TRUE)))
          goto fail_mem_csv;
        if (!nstrs_add_ustr(nstrs, ret))
          goto fail_mem_csv;
        ret = USTR_NULL;
        off += tmp + 1;
        off += ustr_spn_cstr_fwd(in, off, " \t\v\r");
      }
      break;
    }

    if (off >= len)
      continue;
    
    if (*(ustr_cstr(in) + off) != ',')
      goto fail_bad_csv;
    
    if (off != (len - 1)) /* hack for ending in a blank */
      ++off;
  }

  if (!(ret = ustr_dup_fmt("%zu:", nstrs->total - ototal)))
    goto fail_mem_csv;

  scan = onum;
  while (scan < nstrs->num)
    ustr_add(&ret, nstrs->ptr[scan++]);
  ustr_add_rep_chr(&ret, ',', 1);

  if (ustr_enomem(ret))
    goto fail_mem_csv;
  
  nstrs_reset(nstrs, onum, ototal);
  return (ret);

 fail_mem_csv:
  saved_errno = ENOMEM;
  goto fail_csv;
  
 fail_bad_csv:
  saved_errno = EINVAL;
  goto fail_csv;
  
 fail_csv:
  ustr_free(ret);
  nstrs_reset(nstrs, onum, ototal);
  errno = saved_errno;
  return (USTR_NULL);
}

static Ustr *gen_csv_netstr_cstr(Nstrs *nstrs, const char *cstr,
                                 int csv, int interp)
{
  Ustr *in = ustr_dup_cstr(cstr);
  Ustr *ret = USTR_NULL;
  int saved_errno = -1;
  
  if (!in)
    return (USTR_NULL);
  
  ret = gen_csv_netstr(nstrs, &in, NULL, csv, interp);
  saved_errno = errno;
  ustr_free(in);
  errno = saved_errno;

  return (ret);
}

static void out_combine(const char *prog_name, Nstrs *nstrs)
{
  size_t scan = 0;
  Ustr *out = ustr_dup_fmt("%zu:", nstrs->total);

  if (!out)
    die(prog_name, strerror(ENOMEM));
  
  if (!ustr_io_putfile(&out, stdout))
    die(prog_name, strerror(errno));
  ustr_sc_free2(&out, USTR1(\2, ",\n"));

  scan = 0;
  while (scan < nstrs->num)
    if (!ustr_io_putfile(&nstrs->ptr[scan++], stdout))
      die(prog_name, strerror(errno));
  
  if (!ustr_io_putfile(&out, stdout))
    die(prog_name, strerror(errno));
}

int main(int argc, char *argv[])
{
  struct option long_options[] =
  {
   {"combine", no_argument, NULL, 'c'},
   {"interpret", no_argument, NULL, 'i'},
   {"csv", no_argument, NULL, 'C'},

   {"filenames", no_argument, NULL, 'f'},
   
   {"help", no_argument, NULL, 'h'},
   {"version", no_argument, NULL, 'V'},
   
   {NULL, 0, NULL, 0}
  };
  const char *prog_name = NULL;
  Nstrs *nstrs = NULL;
  int scan = 0;
  int optchar = -1;
  int interp  = USTR_FALSE;
  int csv     = USTR_FALSE;
  
  if (!argc)
    usage(NULL, EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  while ((optchar = getopt_long(argc, argv, "cfhiCV", long_options,NULL)) != -1)
    switch (optchar)
    {
      case '?': usage(prog_name, EXIT_FAILURE);
      case 'h': usage(prog_name, EXIT_SUCCESS);
      case 'V':
        printf("%s version %s\n", prog_name, "1.0.0");
        exit (EXIT_SUCCESS);

      case 'c': combine = !combine; break;
      case 'i': interp  = !interp;  break;
      case 'C': csv     = !csv;     break;

      case 'f': fnames  = !fnames;  break;
    }

  argc -= optind;
  argv += optind;
  
  if (!argc)
    usage(prog_name, EXIT_FAILURE);

  if (combine || csv)
    if (!(nstrs = nstrs_init()))
      die(prog_name, strerror(ENOMEM));

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
      
      nstrs_reset(nstrs, 0, 0);
      while (ustr_io_getline(&line, fp))
      {
        Ustr *arg = USTR_NULL;

        ustr_del(&line, ustr_spn_chr_rev(line, 0, '\n'));
        
        if (!(arg = gen_csv_netstr(nstrs, &line, fp, csv, interp)))
          die(prog_name, strerror(errno));
        
        if (combine)
        {
          if (!nstrs_add_ustr(nstrs, arg))
            die(prog_name, strerror(ENOMEM));
        }
        else
        {
          if (!ustr_io_putfileline(&arg, stdout))
            die(prog_name, strerror(errno));
          ustr_free(arg);
        }
        
        ustr_sc_del(&line);          
        if (line != USTR(buf_line)) /* re-init */
          ustr_sc_free2(&line, USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0));
      }
      if (errno)
        die(prog_name, strerror(errno));

      if (line != USTR(buf_line)) /* re-init */
        ustr_sc_free2(&line, USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0));
      
      fclose(fp);
      
      if (combine)
        out_combine(prog_name, nstrs);
    }
  }
  else if (!combine)  /* Output one per. line */
    while (scan < argc)
    {
      Ustr *arg = gen_csv_netstr_cstr(nstrs, argv[scan++], csv, interp);
      
      if (!arg || !ustr_io_putfileline(&arg, stdout))
        die(prog_name, strerror(errno));
      ustr_free(arg);
    }
  else
  {
    while (scan < argc)
    {
      Ustr *arg = gen_csv_netstr_cstr(nstrs, argv[scan++], csv, interp);
      
      if (!arg)
        die(prog_name, strerror(errno));
      if (!nstrs_add_ustr(nstrs, arg))
        die(prog_name, strerror(ENOMEM));
    }

    out_combine(prog_name, nstrs);
  }
  
  nstrs_free(nstrs);
    
  USTR_CNTL_MALLOC_CHECK_END();
  
  return (EXIT_SUCCESS);
}

