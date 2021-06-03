#line 28 "smoother.org"
#ifndef SK_SMOOTHER_H
#define SK_SMOOTHER_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 75 "smoother.org"
typedef struct sk_smoother sk_smoother;
#line 36 "smoother.org"

#ifdef SK_SMOOTHER_PRIV
#line 95 "smoother.org"
struct sk_smoother {
    SKFLT smooth;
    SKFLT a1, b0, y0, psmooth;
    SKFLT onedsr;
};
#line 39 "smoother.org"
#endif

#line 82 "smoother.org"
void sk_smoother_init(sk_smoother *s, int sr);
#line 141 "smoother.org"
void sk_smoother_time(sk_smoother *s, SKFLT t);
#line 161 "smoother.org"
void sk_smoother_reset(sk_smoother *s, SKFLT in);
#line 177 "smoother.org"
SKFLT sk_smoother_tick(sk_smoother *s, SKFLT in);
#line 42 "smoother.org"

#endif
