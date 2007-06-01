#! /bin/sh

if false; then
 echo "Not reached."
elif [ -f ./Makefile ]; then
	doln=false
else
  echo "Not in right place, goto to the root."
  exit 1;
fi

# Remove arch stuff
make distclean

rm -rf ,,*

if [ "x$1" = "xfull" ]; then
  rm -rf lcov-output
fi

rm -f  gmon.out

