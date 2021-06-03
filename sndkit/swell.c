#line 48 "swell.org"
#include <math.h>
#define SK_SWELL_PRIV
#include "swell.h"
#line 77 "swell.org"
void sk_swell_init(sk_swell *sw, int sr)
{
#line 110 "swell.org"
sw->smooth_y = 0;
sw->smooth_b0 = 0;
sw->smooth_a1 = 0;
sw->smooth_pamt = -100;
#line 128 "swell.org"
sw->inertia_y = 0;
sw->inertia_b0 = 0;
sw->inertia_a1 = 0;
sw->inertia_pamt = -100;
#line 144 "swell.org"
sw->onedsr = 1.0 / (SKFLT) sr;
#line 196 "swell.org"
sk_swell_smootha(sw, 10);
#line 221 "swell.org"
sk_swell_smoothb(sw, 0.1);
#line 246 "swell.org"
sk_swell_inertia(sw, 0.001);
#line 260 "swell.org"
sw->dir = &sw->sm_a;
#line 80 "swell.org"
}
#line 183 "swell.org"
void sk_swell_smootha(sk_swell *sw, SKFLT smooth)
{
    sw->sm_a = smooth;
}
#line 208 "swell.org"
void sk_swell_smoothb(sk_swell *sw, SKFLT smooth)
{
    sw->sm_b = smooth;
}
#line 233 "swell.org"
void sk_swell_inertia(sk_swell *sw, SKFLT inertia)
{
    sw->inertia_amt = inertia;
}
#line 273 "swell.org"
SKFLT sk_swell_tick(sk_swell *sw, SKFLT in)
{
    SKFLT out;
    SKFLT switching;
    SKFLT dir;
    const SKFLT eps = 0.0001;
    out = 0;
#line 297 "swell.org"
if (in > sw->prev) {
    sw->dir = &sw->sm_a;
} else if (in < sw->prev) {
    sw->dir = &sw->sm_b;
}
sw->prev = in;
dir = *sw->dir;
switching = dir;
#line 273 "swell.org"
#line 315 "swell.org"
if (sw->inertia_amt != sw->inertia_pamt) {
    sw->inertia_pamt = sw->inertia_amt;
    sw->inertia_a1 = pow(0.5, sw->onedsr/sw->inertia_amt);
    sw->inertia_b0 = 1.0 - sw->inertia_a1;
}

switching =
    sw->inertia_b0*switching +
    sw->inertia_a1*sw->inertia_y;
sw->inertia_y = switching;
#line 273 "swell.org"
#line 336 "swell.org"
if (fabs(switching - dir) < eps) switching = dir;
#line 273 "swell.org"
#line 346 "swell.org"
if (switching != sw->smooth_pamt) {
    sw->smooth_pamt = switching;
    sw->smooth_a1 = pow(0.5, sw->onedsr/switching);
    sw->smooth_b0 = 1.0 - sw->smooth_a1;
}

out =
    sw->smooth_b0*in +
    sw->smooth_a1*sw->smooth_y;
sw->smooth_y = out;
#line 284 "swell.org"
    return out;
}
#line 48 "swell.org"
