#! /bin/sh

if false; then
 echo "Not reached."
elif [ -f ./ustr.pc ]; then
	doln=false
else
  echo "Not in right place, goto to the root."
  exit 1;
fi

# Remove arch stuff
make distclean
( cd examples; make clean )

# Arch
rm -rf ,,*
# git
rm -rf .git

if [ "x$1" = "xfull" ]; then
  rm -rf lcov-output
fi

rm -f  gmon.out
rm -f  lib-all.info
rm -f  lib-all-ustr.info

