#! /usr/bin/perl -w

use strict;
use FileHandle;

my $inc = undef;
   $inc ="./";

my @files = qw(main set cmp srch spn fmt b io utf8 parse);

for my $name (@files) {
if (!open(IN, "< $inc/ustr-$name.h"))
  {
    die "open: $!\n";
  }

$/ = undef;

$_ = <IN>;

while (/^USTR_CONF_EI?_PROTO\s+([\s0-9a-zA-Z_*]*\s+\**)(ustr(_)[0-9a-zA-Z][0-9a-zA-Z_]*)\((?:void|([^,\)]+)|([^,\)]+),([^,\)]+)|([^,\)]+),([^,\)]+),([^,\)]+)|([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+)|([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+)|([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+)|([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+),([^,\)]+))\);?\s*$/gm)
  {
    print "Function: $2()\n";
    print " Returns: $1\n";
    print " Type: $1\n";
    print "\n";
    if (defined ($4)) {
      print " Parameter[1]: $4\n";
      print " Type[1]: $4\n";
    }
    if (defined ($5)) {
      print " Parameter[1]: $5\n";
      print " Type[1]: $5\n";
      print "\n";
      print " Parameter[2]: $6\n";
      print " Type[2]: $6\n";
    }
    if (defined ($7)) {
      print " Parameter[1]: $7\n";
      print " Type[1]: $7\n";
      print "\n";
      print " Parameter[2]: $8\n";
      print " Type[2]: $8\n";
      print "\n";
      print " Parameter[3]: $9\n";
      print " Type[3]: $9\n";
    }
    if (defined ($10)) {
      print " Parameter[1]: $10\n";
      print " Type[1]: $10\n";
      print "\n";
      print " Parameter[2]: $11\n";
      print " Type[2]: $11\n";
      print "\n";
      print " Parameter[3]: $12\n";
      print " Type[3]: $12\n";
      print "\n";
      print " Parameter[4]: $13\n";
      print " Type[4]: $13\n";
    }
    if (defined ($14)) {
      print " Parameter[1]: $14\n";
      print " Type[1]: $14\n";
      print "\n";
      print " Parameter[2]: $15\n";
      print " Type[2]: $15\n";
      print "\n";
      print " Parameter[3]: $16\n";
      print " Type[3]: $16\n";
      print "\n";
      print " Parameter[4]: $17\n";
      print " Type[4]: $17\n";
      print "\n";
      print " Parameter[5]: $18\n";
      print " Type[5]: $18\n";
    }
    if (defined ($19)) {
      print " Parameter[1]: $19\n";
      print " Type[1]: $19\n";
      print "\n";
      print " Parameter[2]: $20\n";
      print " Type[2]: $20\n";
      print "\n";
      print " Parameter[3]: $21\n";
      print " Type[3]: $21\n";
      print "\n";
      print " Parameter[4]: $22\n";
      print " Type[4]: $22\n";
      print "\n";
      print " Parameter[5]: $23\n";
      print " Type[5]: $23\n";
      print "\n";
      print " Parameter[6]: $24\n";
      print " Type[6]: $24\n";
    }
    if (defined ($25)) {
      print " Parameter[1]: $25\n";
      print " Type[1]: $25\n";
      print "\n";
      print " Parameter[2]: $26\n";
      print " Type[2]: $26\n";
      print "\n";
      print " Parameter[3]: $27\n";
      print " Type[3]: $27\n";
      print "\n";
      print " Parameter[4]: $28\n";
      print " Type[4]: $28\n";
      print "\n";
      print " Parameter[5]: $29\n";
      print " Type[5]: $29\n";
      print "\n";
      print " Parameter[6]: $30\n";
      print " Type[6]: $30\n";
      print "\n";
      print " Parameter[7]: $31\n";
      print " Type[7]: $31\n";
    }
    if (defined ($32)) {
      print " Parameter[1]: $32\n";
      print " Type[1]: $32\n";
      print "\n";
      print " Parameter[2]: $33\n";
      print " Type[2]: $33\n";
      print "\n";
      print " Parameter[3]: $34\n";
      print " Type[3]: $34\n";
      print "\n";
      print " Parameter[4]: $35\n";
      print " Type[4]: $35\n";
      print "\n";
      print " Parameter[5]: $36\n";
      print " Type[5]: $36\n";
      print "\n";
      print " Parameter[6]: $37\n";
      print " Type[6]: $37\n";
      print "\n";
      print " Parameter[7]: $38\n";
      print " Type[7]: $38\n";
      print "\n";
      print " Parameter[8]: $39\n";
      print " Type[8]: $39\n";
    }
    print "\n";
    print "Explanation:\n";
    print "\n";
    print "  This function is NOT DOCUMENTED YET!\n";
    print "\n";
    print "\n";
  }
}

