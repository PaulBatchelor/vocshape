#line 143 "fmpair.org"
#ifndef SK_FMPAIR_H
#define SK_FMPAIR_H
#ifndef SKFLT
#define SKFLT float
#endif
#line 160 "fmpair.org"
typedef struct sk_fmpair sk_fmpair;
#line 527 "fmpair.org"
typedef struct sk_fmpair_fdbk sk_fmpair_fdbk;
#line 143 "fmpair.org"
#line 296 "fmpair.org"
void sk_fmpair_init(sk_fmpair *fmp, int sr,
                    SKFLT *ctab, int csz, SKFLT ciphs,
                    SKFLT *mtab, int msz, SKFLT miphs);
#line 316 "fmpair.org"
void sk_fmpair_freq(sk_fmpair *fmp, SKFLT freq);
#line 343 "fmpair.org"
void sk_fmpair_modulator(sk_fmpair *fmp, SKFLT mod);
void sk_fmpair_carrier(sk_fmpair *fmp, SKFLT car);
#line 378 "fmpair.org"
void sk_fmpair_modindex(sk_fmpair *fmp, SKFLT index);
#line 406 "fmpair.org"
SKFLT sk_fmpair_tick(sk_fmpair *fmp);
#line 549 "fmpair.org"
void sk_fmpair_fdbk_init(sk_fmpair_fdbk *fmp, int sr,
                         SKFLT *ctab, int csz, SKFLT ciphs,
                         SKFLT *mtab, int msz, SKFLT miphs);
#line 572 "fmpair.org"
void sk_fmpair_fdbk_amt(sk_fmpair_fdbk *f, SKFLT amt);
#line 588 "fmpair.org"
SKFLT sk_fmpair_fdbk_tick(sk_fmpair_fdbk *fmp);
#line 150 "fmpair.org"
#ifdef SK_FMPAIR_PRIV
#line 165 "fmpair.org"
struct sk_fmpair {
#line 191 "fmpair.org"
SKFLT *ctab;
int csz;
int clphs;
#line 207 "fmpair.org"
SKFLT *mtab;
int msz;
int mlphs;
#line 238 "fmpair.org"
/* carrier constants */
int cnlb;
SKFLT cinlb;
unsigned long cmask;

/* modulator constants */
int mnlb;
SKFLT minlb;
unsigned long mmask;

SKFLT maxlens;
#line 329 "fmpair.org"
SKFLT freq;
#line 362 "fmpair.org"
SKFLT car;
SKFLT mod;
#line 391 "fmpair.org"
SKFLT index;
#line 167 "fmpair.org"
};
#line 537 "fmpair.org"
struct sk_fmpair_fdbk {
    sk_fmpair fmpair;
    SKFLT prev;
    SKFLT feedback;
};
#line 152 "fmpair.org"
#endif
#endif
