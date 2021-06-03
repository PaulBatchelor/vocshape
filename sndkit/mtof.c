#line 64 "mtof.org"
#include <math.h>
#define SK_MTOF_PRIV
#include "mtof.h"
#line 105 "mtof.org"
void sk_mtof_init(sk_mtof *mf)
{
    mf->freq = -1;
    mf->nn = -1;
}
#line 124 "mtof.org"
SKFLT sk_mtof_tick(sk_mtof *mf, SKFLT nn)
{
    if (nn != mf->nn) {
        mf->nn = nn;
        mf->freq = SK_MTOF(nn);
    }
    return mf->freq;
}
#line 64 "mtof.org"
