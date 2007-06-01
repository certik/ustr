#! /usr/bin/perl -w

use strict;

open IN, '<', shift || die " Format: $0 <filename>";

my %vals = ();
my $cur_len = 0;

while (<IN>)
  {
    if (/^String: (\d+)/)
      {
	$cur_len = $1;

	$vals{$cur_len}->[8] = 0; # 32bit doesn't have them
	$vals{$cur_len}->[9] = 0;

	next;
      }

    if (/^\s+ Ss(\d) .* = \s (\d+.\d\d)%/x)
      { $vals{$cur_len}->[$1] = $2; next; }

    if (/^\s+ strdup .* = \s (\d+.\d\d)%/x)
      { $vals{$cur_len}->[10] = $1; next; }

    if (/^\s+ NM_Ss \s+ = .* = \s (\d+.\d\d)%/x)
      { $vals{$cur_len}->[11] = $1; next; }

    if (/^\s+ NM_Ss \s x2 \s+ = .* = \s (\d+.\d\d)%/x)
      { $vals{$cur_len}->[12] = $1; next; }

    if (/^\s+ NM_Ss \s xSs \s+ = .* = \s (\d+.\d\d)%/x)
      { $vals{$cur_len}->[13] = $1; next; }
  }

print <<EOL;
# set style data lines
# set xrange [0:259]
# set yrange [100:500]
#
# set xlabel "Length"
# set ylabel "Percent bigger"
#
# plot "sizes_out_64bit.tsv" using 1:2 title 'Ss0',
#      "sizes_out_64bit.tsv" using 1:3 title 'Ss1',
#      "sizes_out_64bit.tsv" using 1:4 title 'Ss2',
#      "sizes_out_64bit.tsv" using 1:5 title 'Ss3',
#      "sizes_out_64bit.tsv" using 1:6 title 'Ss4',
#      "sizes_out_64bit.tsv" using 1:7 title 'Ss5',
#      "sizes_out_64bit.tsv" using 1:8 title 'Ss6',
#      "sizes_out_64bit.tsv" using 1:9 title 'Ss7',
#      "sizes_out_64bit.tsv" using 1:10 title 'Ss8',
#      "sizes_out_64bit.tsv" using 1:11 title 'Ss9',
#      "sizes_out_64bit.tsv" using 1:12 title 'strdup',
#      "sizes_out_64bit.tsv" using 1:13 title 'NM_Ss',
#      "sizes_out_64bit.tsv" using 1:14 title 'NM_Ss x2',
#      "sizes_out_64bit.tsv" using 1:15 title 'NM_Ss xSs'
#
# plot "sizes_out_32bit.tsv" using 1:2 title 'Ss0',
#      "sizes_out_32bit.tsv" using 1:3 title 'Ss1',
#      "sizes_out_32bit.tsv" using 1:4 title 'Ss2',
#      "sizes_out_32bit.tsv" using 1:5 title 'Ss3',
#      "sizes_out_32bit.tsv" using 1:6 title 'Ss4',
#      "sizes_out_32bit.tsv" using 1:7 title 'Ss5',
#      "sizes_out_32bit.tsv" using 1:8 title 'Ss6',
#      "sizes_out_32bit.tsv" using 1:9 title 'Ss7',
#      "sizes_out_32bit.tsv" using 1:12 title 'strdup',
#      "sizes_out_32bit.tsv" using 1:13 title 'NM_Ss',
#      "sizes_out_32bit.tsv" using 1:14 title 'NM_Ss x2',
#      "sizes_out_32bit.tsv" using 1:15 title 'NM_Ss xSs'
#
# set xrange [1:59]
# set yrange [100:800]
# replot
#
EOL


for my $num (sort { $a <=> $b } keys %vals)
  {
    my $i = $vals{$num};
    printf("%u" . "\t%s" x 14 . "\n", $num,
	   $i->[0], $i->[1], $i->[2], $i->[3],
	   $i->[4], $i->[5], $i->[6], $i->[7],
	   $i->[8], $i->[9],
	   $i->[10], $i->[11], $i->[12], $i->[13]);
  }
