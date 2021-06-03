#line 34 "expmap.org"
#include <math.h>
#define SK_EXPMAP_PRIV
#include "expmap.h"
#line 138 "expmap.org"
static SKFLT compute_scale(SKFLT slope);
#line 34 "expmap.org"
#line 73 "expmap.org"
SKFLT sk_expmap_stateless(SKFLT in, SKFLT slope)
{
    return (1 - exp(in*slope)) / (1 - exp(slope));
}
#line 143 "expmap.org"
static SKFLT compute_scale(SKFLT slope)
{
    return 1 / (1 - exp(slope));
}
#line 166 "expmap.org"
void sk_expmap_init(sk_expmap *em)
{
#line 107 "expmap.org"
em->pin = -1;
#line 121 "expmap.org"
em->pout = -1;
#line 199 "expmap.org"
sk_expmap_slope(em, 1);
em->pslope = 1;
#line 154 "expmap.org"
em->scale = compute_scale(em->slope);
#line 199 "expmap.org"
#line 169 "expmap.org"
}
#line 181 "expmap.org"
void sk_expmap_slope(sk_expmap *em, SKFLT slope)
{
    em->slope = slope;
}
#line 213 "expmap.org"
SKFLT sk_expmap_tick(sk_expmap *em, SKFLT in)
{
    SKFLT out = 0;
#line 228 "expmap.org"
if (em->slope != em->pslope) {
    em->pslope = em->slope;
    em->scale = compute_scale(em->slope);
}
#line 213 "expmap.org"
#line 244 "expmap.org"
if (in != em->pin) {
    em->pin = in;
    em->pout = (1 - exp(in * em->slope)) * em->scale;
}
out = em->pout;
#line 218 "expmap.org"
    return out;
}
#line 34 "expmap.org"
