#line 43 "expmap.org"
#ifndef SK_EXPMAP_H
#define SK_EXPMAP_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 83 "expmap.org"
typedef struct sk_expmap sk_expmap;
#line 43 "expmap.org"
#line 68 "expmap.org"
SKFLT sk_expmap_stateless(SKFLT in, SKFLT slope);
#line 161 "expmap.org"
void sk_expmap_init(sk_expmap *em);
#line 176 "expmap.org"
void sk_expmap_slope(sk_expmap *em, SKFLT slope);
#line 208 "expmap.org"
SKFLT sk_expmap_tick(sk_expmap *em, SKFLT in);
#line 52 "expmap.org"

#ifdef SK_EXPMAP_PRIV
#line 88 "expmap.org"
struct sk_expmap {
#line 102 "expmap.org"
SKFLT pin;
#line 116 "expmap.org"
SKFLT pout;
#line 130 "expmap.org"
SKFLT scale;
#line 191 "expmap.org"
SKFLT slope;
SKFLT pslope;
#line 90 "expmap.org"
};
#line 55 "expmap.org"
#endif

#endif
