#line 48 "smoother.org"
#include <math.h>
#define SK_SMOOTHER_PRIV
#include "smoother.h"
#line 87 "smoother.org"
void sk_smoother_init(sk_smoother *s, int sr)
{
#line 114 "smoother.org"
s->smooth = 0.01;
s->psmooth = -1;
#line 123 "smoother.org"
s->a1 = 0;
s->b0 = 0;
s->y0 = 0;
#line 134 "smoother.org"
s->onedsr = 1.0 / sr;
#line 90 "smoother.org"
}
#line 146 "smoother.org"
void sk_smoother_time(sk_smoother *s, SKFLT t)
{
    s->smooth = t;
}
#line 166 "smoother.org"
void sk_smoother_reset(sk_smoother *s, SKFLT in)
{
    s->y0 = in;
}
#line 187 "smoother.org"
SKFLT sk_smoother_tick(sk_smoother *s, SKFLT in)
{
    SKFLT out;

    if (s->psmooth != s->smooth) {
        s->a1 = pow(0.5, s->onedsr/s->smooth);
        s->b0 = 1.0 - s->a1;
        s->psmooth = s->smooth;
    }

    s->y0 = s->b0 * in + s->a1 * s->y0;
    out = s->y0;

    return out;
}
#line 48 "smoother.org"
