#ifndef USTR_H
#define USTR_H 1

#ifndef USTR_DEBUG
#define USTR_DEBUG 1
#endif

#if USTR_DEBUG
# include "ustr-conf-debug.h"
#else
# include "ustr-conf.h"
#endif

#include "ustr-main.h"
#include "ustr-set.h"
#include "ustr-fmt.h"
#include "ustr-srch.h"
#include "ustr-cmp.h"
#include "ustr-b.h"
#include "ustr-io.h"
#include "ustr-spn.h"
#include "ustr-utf8.h"

#endif
