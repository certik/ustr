#include "ustr.h"
/*  Note that this version of hexdump might well not scale as well, for large
 * inputs, as things like the example hexdump that comes with Vstr. */


#include <getopt.h>
#include <errno.h>

static int prnt_spac_bytes = USTR_TRUE;
static int prnt_high_bytes = USTR_FALSE;

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit (EXIT_FAILURE);
}

static void usage(const char *prog_name, int xcode)
{
  fprintf((xcode == EXIT_SUCCESS) ? stdout : stderr, "\
 Format: %s [-hHSV] [filename]...\n\
           -S, --space = Print spaces in output.\n\
           -H, --high  = Print high bytes in output.\n\
           -h          = Print help.\n\
           -V          = Print version.\n\
", prog_name);
  
  exit (xcode);
}

static void conv_unprintable_chr(Ustr *s1, int prnt_h, int prnt_s)
{
  size_t len = ustr_len(s1);
  char  *ptr = ustr_wstr(s1);

  while (len--)
  {
    unsigned char tst = *ptr;
    
    if ((tst >= 0x21) && (tst <= 0x7E))
      ++ptr;
    else if (prnt_h && prnt_s && (tst == 0xA0)) /* high space */
      ++ptr;
    else if (prnt_h && (tst >= 0xA1)) /* "high" chars */
      ++ptr;
    else if (prnt_s && (tst == 0x20)) /* space */
      ++ptr;
    else
      *ptr++ = '.';
  }
}

#define CONF_READ_SZ 16
static unsigned int addr = 0;
static void hexdump(Ustr **ps1)
{
  Ustr *s1 = *ps1;
  char buf[CONF_READ_SZ * 5]; /* each 16 bytes turns into 70 bytes */
  Ustr *out = USTR_SC_INIT_AUTO(buf, USTR_FALSE, 0);
  char buf_line[128]; /* 16 max */
  Ustr *line = USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0);
  size_t len = ustr_len(s1);
  const unsigned char *ptr = (const unsigned char *)ustr_cstr(s1);

  while (len >= 16)
  {
    ustr_set_buf(&line, ptr, 16);
    
    ustr_add_fmt(&out, "0x%08X:"
                 " %02X%02X %02X%02X %02X%02X %02X%02X"
                 " %02X%02X %02X%02X %02X%02X %02X%02X  ", addr,
                 ptr[ 0], ptr[ 1], ptr[ 2], ptr[ 3],
                 ptr[ 4], ptr[ 5], ptr[ 6], ptr[ 7],
                 ptr[ 8], ptr[ 9], ptr[10], ptr[11],
                 ptr[12], ptr[13], ptr[14], ptr[15]);
    conv_unprintable_chr(line, prnt_high_bytes, prnt_spac_bytes);
    ustr_add(&out, line);
    ustr_add_cstr(&out, "\n");

    addr += 16;
    len  -= 16;
    ptr  += 16;
  }
  if (len)
  {
    size_t missing = 16 - len;

    missing -= (missing % 2);
    
    ustr_set_buf(&line, ptr, len);
    
    ustr_add_fmt(&out, "0x%08X:", addr);

    while (len >= 2)
    {
      ustr_add_fmt(&out,  " %02X%02X", (unsigned)ptr[ 0], ptr[ 1]);
      len  -= 2;
      ptr  += 2;
    }
    if (len)
    {
      ustr_add_fmt(&out,  " %02X  ", ptr[ 0]);
      len  -= 1;
      ptr  += 1;
    }
    ustr_add_rep_chr(&out, ' ', (missing * 2) + (missing / 2) + 2);

    conv_unprintable_chr(line, prnt_high_bytes, prnt_spac_bytes);
    ustr_add(&out, line);
    ustr_add_cstr(&out, "\n");

    addr += ustr_len(line);
  }

  ustr_set(ps1, out);
  ustr_free(out);    
  ustr_free(line);
}

static void loop(Ustr **io, FILE *in, const char *prog_name)
{
  int last = USTR_FALSE;
  size_t got = 0;
  
  while (!last && ustr_io_get(io, in, CONF_READ_SZ, &got))
  {
    last = got != CONF_READ_SZ;
    
    hexdump(io);
    if (!ustr_io_putfile(io, stdout))
      die(prog_name, strerror(errno));
  }
}

int main(int argc, char *argv[])
{
  struct option long_options[] =
  {
   {"space", no_argument, NULL, 'S'},
   {"high", no_argument, NULL, 'H'},
   
   {"help", no_argument, NULL, 'h'},
   {"version", no_argument, NULL, 'V'},
   
   {NULL, 0, NULL, 0}
  };
  const char *prog_name = NULL;
  Ustr *io = NULL;
  int scan = 0;
  int optchar = -1;
  
  if (!argc)
    exit (EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  while ((optchar = getopt_long(argc, argv, "hHSV", long_options, NULL)) != -1)
    switch (optchar)
    {
      case '?': usage(prog_name, EXIT_FAILURE);
      case 'h': usage(prog_name, EXIT_SUCCESS);
      case 'V':
        printf("%s version %s\n", prog_name, "1.0.0");
        exit (EXIT_SUCCESS);

      case 'H': prnt_high_bytes = !prnt_high_bytes;  break;
      case 'S': prnt_spac_bytes = !prnt_spac_bytes;  break;
    }

  argc -= optind;
  argv += optind;

  if (!(io = ustr_dupx_empty(4096, 0, USTR_FALSE, USTR_FALSE)))
    die(prog_name, strerror(ENOMEM));
  
  if (!argc)
    loop(&io, stdin, prog_name);
  
  scan = 0;
  while (scan < argc)
  {
    if (ustr_cmp_cstr_eq(USTR1(\1, "-"), argv[scan]))
      loop(&io, stdin, prog_name);
    else
    {
      FILE *fp = fopen(argv[scan], "rb");
      
      if (!fp)
        die(prog_name, strerror(errno));
      
      loop(&io, fp, prog_name);
      
      if (ferror(fp))
        die(prog_name, strerror(errno));
      
      fclose(fp);
    }
    
    ++scan;
  }

  ustr_free(io); /* to make it allocation "clean" */
  
  USTR_CNTL_MALLOC_CHECK_END();
  
  return (EXIT_SUCCESS);
}

