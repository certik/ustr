#! /bin/sh -e

if false; then
 echo "Not reached."
elif [ -f ./ustr.pc ]; then
        doln=false
else
  echo "Not in right place, goto to the root."
  exit 1;
fi

sort libustr.ver > /tmp/libustr.ver.sort
tr -d '; ' < /tmp/libustr.ver.sort  > /tmp/ustr.libver.sort.tr

./scripts/list_functions_src.pl x | sort > /tmp/ustr.gen.sort

tr -d '()' < /tmp/ustr.gen.sort > /tmp/ustr.gen.sort.tr

diff -bBu /tmp/ustr.libver.sort.tr /tmp/ustr.gen.sort.tr
