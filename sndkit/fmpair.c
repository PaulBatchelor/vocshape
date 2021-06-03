#line 130 "fmpair.org"
#include <math.h>
#define SK_FMPAIR_PRIV
#include "fmpair.h"
#line 178 "fmpair.org"
#define SK_FMPAIR_MAXLEN 0x1000000L
#define SK_FMPAIR_PHASEMASK 0x0FFFFFFL
#line 130 "fmpair.org"
#line 303 "fmpair.org"
void sk_fmpair_init(sk_fmpair *fmp, int sr,
                    SKFLT *ctab, int csz, SKFLT ciphs,
                    SKFLT *mtab, int msz, SKFLT miphs)
{
#line 198 "fmpair.org"
fmp->ctab = ctab;
fmp->csz = msz;
#line 214 "fmpair.org"
fmp->mtab = mtab;
fmp->msz = msz;
#line 226 "fmpair.org"
fmp->clphs = floor(ciphs * SK_FMPAIR_MAXLEN);
fmp->mlphs = floor(miphs * SK_FMPAIR_MAXLEN);
#line 253 "fmpair.org"
{
    int tmp;

    /* carrier */
    tmp = SK_FMPAIR_MAXLEN / csz;
    fmp->cnlb = 0;
    while (tmp >>= 1) fmp->cnlb++;

    /* modulator */
    tmp = SK_FMPAIR_MAXLEN / msz;
    fmp->mnlb = 0;
    while (tmp >>= 1) fmp->mnlb++;
}

/* phase mask for dividing lower/upper bits */

fmp->cmask = (1<<fmp->cnlb) - 1;
fmp->mmask = (1<<fmp->mnlb) - 1;

/* constant used to convert to floating point */

fmp->cinlb = 1.0 / (1<<fmp->cnlb);
fmp->minlb = 1.0 / (1<<fmp->mnlb);

/* max table length in seconds */
/* used to convert cycles-per-second units to cycles */

fmp->maxlens = 1.0 * SK_FMPAIR_MAXLEN / sr;
#line 336 "fmpair.org"
sk_fmpair_freq(fmp, 440);
#line 370 "fmpair.org"
sk_fmpair_carrier(fmp, 1);
sk_fmpair_modulator(fmp, 1);
#line 399 "fmpair.org"
sk_fmpair_modindex(fmp, 1);
#line 308 "fmpair.org"
}
#line 321 "fmpair.org"
void sk_fmpair_freq(sk_fmpair *fmp, SKFLT freq)
{
    fmp->freq = freq;
}
#line 349 "fmpair.org"
void sk_fmpair_modulator(sk_fmpair *fmp, SKFLT mod)
{
    fmp->mod = mod;
}

void sk_fmpair_carrier(sk_fmpair *fmp, SKFLT car)
{
    fmp->car = car;
}
#line 383 "fmpair.org"
void sk_fmpair_modindex(sk_fmpair *fmp, SKFLT index)
{
    fmp->index = index;
}
#line 411 "fmpair.org"
SKFLT sk_fmpair_tick(sk_fmpair *fmp)
{
    SKFLT out;
    SKFLT cfreq, mfreq;
    SKFLT modout;
    int ipos;
    SKFLT frac;
    SKFLT x[2];
    out = 0;
#line 436 "fmpair.org"
cfreq = fmp->freq * fmp->car;
mfreq = fmp->freq * fmp->mod;
#line 411 "fmpair.org"
#line 446 "fmpair.org"
fmp->mlphs &= SK_FMPAIR_PHASEMASK;
ipos = fmp->mlphs >> fmp->mnlb;
x[0] = fmp->mtab[ipos];

if (ipos == fmp->msz - 1) {
    x[1] = fmp->mtab[0];
} else {
    x[1] = fmp->mtab[ipos + 1];
}

frac = (fmp->mlphs & fmp->mmask) * fmp->minlb;
modout = (x[0] + (x[1] - x[0]) * frac);
#line 411 "fmpair.org"
#line 467 "fmpair.org"
modout *= mfreq * fmp->index;
#line 411 "fmpair.org"
#line 476 "fmpair.org"
cfreq += modout;
#line 411 "fmpair.org"
#line 485 "fmpair.org"
fmp->clphs &= SK_FMPAIR_PHASEMASK;
ipos = (fmp->clphs) >> fmp->cnlb;
x[0] = fmp->ctab[ipos];

if (ipos == fmp->csz - 1) {
    x[1] = fmp->ctab[0];
} else {
    x[1] = fmp->ctab[ipos + 1];
}

frac = (fmp->clphs & fmp->cmask) * fmp->cinlb;
out = (x[0] + (x[1] - x[0]) * frac);
#line 411 "fmpair.org"
#line 512 "fmpair.org"
fmp->clphs += floor(cfreq * fmp->maxlens);
fmp->mlphs += floor(mfreq * fmp->maxlens);
#line 426 "fmpair.org"
    return out;
}
#line 556 "fmpair.org"
void sk_fmpair_fdbk_init(sk_fmpair_fdbk *fmp, int sr,
                         SKFLT *ctab, int csz, SKFLT ciphs,
                         SKFLT *mtab, int msz, SKFLT miphs)
{
    sk_fmpair_init(&fmp->fmpair, sr,
                   ctab, csz, ciphs,
                   mtab, msz, miphs);
    fmp->prev = 0;
    fmp->feedback = 0;
}
#line 577 "fmpair.org"
void sk_fmpair_fdbk_amt(sk_fmpair_fdbk *f, SKFLT amt)
{
    f->feedback = amt;
}
#line 600 "fmpair.org"
SKFLT sk_fmpair_fdbk_tick(sk_fmpair_fdbk *f)
{
    SKFLT out;
    SKFLT cfreq, mfreq;
    SKFLT modout;
    int ipos;
    SKFLT frac;
    SKFLT x[2];
    sk_fmpair *fmp;
    out = 0;
    fmp = &f->fmpair;

#line 436 "fmpair.org"
cfreq = fmp->freq * fmp->car;
mfreq = fmp->freq * fmp->mod;
#line 600 "fmpair.org"
#line 446 "fmpair.org"
fmp->mlphs &= SK_FMPAIR_PHASEMASK;
ipos = fmp->mlphs >> fmp->mnlb;
x[0] = fmp->mtab[ipos];

if (ipos == fmp->msz - 1) {
    x[1] = fmp->mtab[0];
} else {
    x[1] = fmp->mtab[ipos + 1];
}

frac = (fmp->mlphs & fmp->mmask) * fmp->minlb;
modout = (x[0] + (x[1] - x[0]) * frac);
#line 614 "fmpair.org"

    /* feedback-oscillator specific */
#line 634 "fmpair.org"
modout += f->prev * f->feedback;
f->prev = modout;
#line 617 "fmpair.org"

#line 467 "fmpair.org"
modout *= mfreq * fmp->index;
#line 619 "fmpair.org"

#line 476 "fmpair.org"
cfreq += modout;
#line 619 "fmpair.org"
#line 485 "fmpair.org"
fmp->clphs &= SK_FMPAIR_PHASEMASK;
ipos = (fmp->clphs) >> fmp->cnlb;
x[0] = fmp->ctab[ipos];

if (ipos == fmp->csz - 1) {
    x[1] = fmp->ctab[0];
} else {
    x[1] = fmp->ctab[ipos + 1];
}

frac = (fmp->clphs & fmp->cmask) * fmp->cinlb;
out = (x[0] + (x[1] - x[0]) * frac);
#line 619 "fmpair.org"
#line 512 "fmpair.org"
fmp->clphs += floor(cfreq * fmp->maxlens);
fmp->mlphs += floor(mfreq * fmp->maxlens);
#line 623 "fmpair.org"
    return out;
}
#line 130 "fmpair.org"
