#line 44 "osc.org"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#define SK_OSC_PRIV
#include "osc.h"
#line 105 "osc.org"
#define SK_OSC_MAXLEN 0x1000000L
#define SK_OSC_PHASEMASK 0x0FFFFFFL
#line 44 "osc.org"
#line 92 "osc.org"
void sk_osc_freq(sk_osc *osc, SKFLT freq)
{
    osc->freq = freq;
}

void sk_osc_amp(sk_osc *osc, SKFLT amp)
{
    osc->amp = amp;
}
#line 253 "osc.org"
void sk_osc_init(sk_osc *osc, int sr, SKFLT *wt, int sz, SKFLT iphs)
{
#line 140 "osc.org"
osc->freq = 440;
osc->amp = 0.2;
#line 149 "osc.org"
osc->tab = wt;
osc->sz = sz;
#line 160 "osc.org"
osc->inc = 0;
#line 170 "osc.org"
osc->lphs = ((int32_t)(iphs * SK_OSC_MAXLEN)) & SK_OSC_PHASEMASK;
#line 204 "osc.org"
{
    uint32_t tmp;
    tmp = SK_OSC_MAXLEN / sz;
    osc->nlb = 0;
    while (tmp >>= 1) osc->nlb++;
}
#line 227 "osc.org"
osc->mask = (1<<osc->nlb) - 1;
#line 204 "osc.org"
#line 237 "osc.org"
osc->inlb = 1.0 / (1<<osc->nlb);
#line 204 "osc.org"
#line 246 "osc.org"
osc->maxlens = 1.0 * SK_OSC_MAXLEN / sr;
#line 204 "osc.org"
#line 256 "osc.org"
}
#line 265 "osc.org"
SKFLT sk_osc_tick(sk_osc *osc)
{
    SKFLT out;
    SKFLT fract;
    SKFLT x1, x2;
    int32_t phs;
    int pos;

    out = 0;
#line 330 "osc.org"
osc->inc = floor(lrintf(osc->freq * osc->maxlens));
#line 265 "osc.org"
#line 341 "osc.org"
phs = osc->lphs;
pos = phs >> osc->nlb;
x1 = osc->tab[pos];
x2 = osc->tab[(pos + 1) % osc->sz];
#line 265 "osc.org"
#line 368 "osc.org"
fract = (phs & osc->mask) * osc->inlb;
#line 265 "osc.org"
#line 395 "osc.org"
out = (x1 + (x2 - x1) * fract) * osc->amp;
#line 265 "osc.org"
#line 404 "osc.org"
phs += osc->inc;
phs &= SK_OSC_PHASEMASK;
osc->lphs = phs;
#line 279 "osc.org"
    return out;
}
#line 44 "osc.org"
