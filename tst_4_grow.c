#undef  USTR_CONF_REF_BYTES
#define USTR_CONF_REF_BYTES    1
#define USTR_CONF_USE_EOS_MARK 0

#  define ustr_assert_ret(x, y) \
    do { if (x){} else return (y); } while (USTR_FALSE)
#  define USTR_ASSERT_RET(x, y) \
    do { if (x){} else return (y); } while (USTR_FALSE)

#include "ctst_4_grow.c"
