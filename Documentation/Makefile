ALL = \
  functions.html \
  constants.html \
  ustr.3 \
  ustr_const.3 \

all: $(ALL)

#  This _shouldn't_ be part of teh normal build ... so we need to make sure it
# happens before we distribute.

functions.html: functions.txt txt2html.pl
		./txt2html.pl

constants.html: constants.txt txt2html.pl
		./txt2html.pl

ustr.3: functions.txt txt2man.pl
		./txt2man.pl

ustr_const.3: constants.txt txt2man.pl
		./txt2man.pl
