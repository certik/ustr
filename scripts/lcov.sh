#! /bin/sh

if false; then
 echo "Not reached."
elif [ -f ./Makefile ]; then
	doln=false
else
  echo "Not in right place, goto to the root."
  exit 1;
fi

# if [ "x$1" = "x" ]; then
#   echo "Not got arg."
#   exit 1;
# fi

name="$1"
name="main"
# gendesc $d/coverage_descriptions.txt -o descriptions

lcov --capture --directory . --output-file lib-$name.info --test-name lib-$name

#cd examples
#lcov --capture --directory . --output-file ../examples-$1.info --test-name examples-$1
#cd ..

mkdir lcov-output || true
genhtml lib*.info --output-directory lcov-output/lib --title "Ustr coverage" --show-details # --description-file descriptions
# genhtml examples*.info --output-directory output/examples --title "Ustr EXAMPLES coverage" --show-details --description-file descriptions
genhtml *.info --output-directory lcov-output/all --title "Ustr ALL coverage" --show-details # --description-file descriptions
echo Point your browser at file:`pwd`/lcov-output/lib/index.html
