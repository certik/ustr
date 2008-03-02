#! /usr/bin/perl -w

use strict;
use FileHandle;

my $inc = undef;
   $inc ="./";

my @files = qw(main set cmp srch spn fmt b io utf8 parse cntl sc pool ins sub replace split);

for my $name (@files) {
if (!open(IN, "< $inc/ustr-$name.h"))
  {
    die "open: $!\n";
  }

$/ = undef;

$_ = <IN>;

while (/^USTR_CONF_EI?_PROTO\s+([\s0-9a-zA-Z_*]*\s+\**)(ustrp?(_)[0-9a-zA-Z][0-9a-zA-Z_]*)\((?:void|\s*([^,\)]+)|\s*([^,\)]+),\s*([^,\)]+)|\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+)|\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+)|\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+)|\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+)|\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+)|\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+)|\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+),\s*([^,\)]+))\);?\s*$/gm)
  {
    print "Function: $2()\n";
    print " Returns: \n";
    print " Type: $1\n";
    print "\n";
    if (defined ($4)) {
      print " Parameter[1]: \n";
      print " Type[1]: $4\n";
    }
    if (defined ($5)) {
      print " Parameter[1]: \n";
      print " Type[1]: $5\n";
      print "\n";
      print " Parameter[2]: \n";
      print " Type[2]: $6\n";
    }
    if (defined ($7)) {
      print " Parameter[1]: \n";
      print " Type[1]: $7\n";
      print "\n";
      print " Parameter[2]: \n";
      print " Type[2]: $8\n";
      print "\n";
      print " Parameter[3]: \n";
      print " Type[3]: $9\n";
    }
    if (defined ($10)) {
      print " Parameter[1]: \n";
      print " Type[1]: $10\n";
      print "\n";
      print " Parameter[2]: \n";
      print " Type[2]: $11\n";
      print "\n";
      print " Parameter[3]: \n";
      print " Type[3]: $12\n";
      print "\n";
      print " Parameter[4]: \n";
      print " Type[4]: $13\n";
    }
    if (defined ($14)) {
      print " Parameter[1]: \n";
      print " Type[1]: $14\n";
      print "\n";
      print " Parameter[2]: \n";
      print " Type[2]: $15\n";
      print "\n";
      print " Parameter[3]: \n";
      print " Type[3]: $16\n";
      print "\n";
      print " Parameter[4]: \n";
      print " Type[4]: $17\n";
      print "\n";
      print " Parameter[5]: \n";
      print " Type[5]: $18\n";
    }
    if (defined ($19)) {
      print " Parameter[1]: \n";
      print " Type[1]: $19\n";
      print "\n";
      print " Parameter[2]: \n";
      print " Type[2]: $20\n";
      print "\n";
      print " Parameter[3]: \n";
      print " Type[3]: $21\n";
      print "\n";
      print " Parameter[4]: \n";
      print " Type[4]: $22\n";
      print "\n";
      print " Parameter[5]: \n";
      print " Type[5]: $23\n";
      print "\n";
      print " Parameter[6]: \n";
      print " Type[6]: $24\n";
    }
    if (defined ($25)) {
      print " Parameter[1]: \n";
      print " Type[1]: $25\n";
      print "\n";
      print " Parameter[2]: \n";
      print " Type[2]: $26\n";
      print "\n";
      print " Parameter[3]: \n";
      print " Type[3]: $27\n";
      print "\n";
      print " Parameter[4]: \n";
      print " Type[4]: $28\n";
      print "\n";
      print " Parameter[5]: \n";
      print " Type[5]: $29\n";
      print "\n";
      print " Parameter[6]: \n";
      print " Type[6]: $30\n";
      print "\n";
      print " Parameter[7]: \n";
      print " Type[7]: $31\n";
    }
    if (defined ($32)) {
      print " Parameter[1]: \n";
      print " Type[1]: $32\n";
      print "\n";
      print " Parameter[2]: \n";
      print " Type[2]: $33\n";
      print "\n";
      print " Parameter[3]: \n";
      print " Type[3]: $34\n";
      print "\n";
      print " Parameter[4]: \n";
      print " Type[4]: $35\n";
      print "\n";
      print " Parameter[5]: \n";
      print " Type[5]: $36\n";
      print "\n";
      print " Parameter[6]: \n";
      print " Type[6]: $37\n";
      print "\n";
      print " Parameter[7]: \n";
      print " Type[7]: $38\n";
      print "\n";
      print " Parameter[8]: \n";
      print " Type[8]: $39\n";
    }
    if (defined ($40)) {
      print " Parameter[1]: \n";
      print " Type[1]: $41\n";
      print "\n";
      print " Parameter[2]: \n";
      print " Type[2]: $42\n";
      print "\n";
      print " Parameter[3]: \n";
      print " Type[3]: $43\n";
      print "\n";
      print " Parameter[4]: \n";
      print " Type[4]: $44\n";
      print "\n";
      print " Parameter[5]: \n";
      print " Type[5]: $45\n";
      print "\n";
      print " Parameter[6]: \n";
      print " Type[6]: $46\n";
      print "\n";
      print " Parameter[7]: \n";
      print " Type[7]: $47\n";
      print "\n";
      print " Parameter[8]: \n";
      print " Type[8]: $48\n";
      print "\n";
      print " Parameter[9]: \n";
      print " Type[9]: $49\n";
    }
    print "\n";
    print " Explanation:\n";
    print "\n";
    print "  This function is NOT DOCUMENTED YET!\n";
    print "\n";
    print "\n";
  }
}

