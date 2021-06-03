#line 29 "swell.org"
#ifndef SK_SWELL_H
#define SK_SWELL_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 60 "swell.org"
typedef struct sk_swell sk_swell;
#line 37 "swell.org"

#ifdef SK_SWELL_PRIV
#line 65 "swell.org"
struct sk_swell {
#line 102 "swell.org"
SKFLT smooth_y;
SKFLT smooth_a1;
SKFLT smooth_b0;
SKFLT smooth_pamt;
#line 120 "swell.org"
SKFLT inertia_y;
SKFLT inertia_a1;
SKFLT inertia_b0;
SKFLT inertia_pamt;
#line 139 "swell.org"
SKFLT onedsr;
#line 164 "swell.org"
SKFLT prev;
#line 191 "swell.org"
SKFLT sm_a;
#line 216 "swell.org"
SKFLT sm_b;
#line 241 "swell.org"
SKFLT inertia_amt;
#line 255 "swell.org"
SKFLT *dir;
#line 67 "swell.org"
};
#line 40 "swell.org"
#endif

#line 72 "swell.org"
void sk_swell_init(sk_swell *sw, int sr);
#line 178 "swell.org"
void sk_swell_smootha(sk_swell *sw, SKFLT smooth);
#line 203 "swell.org"
void sk_swell_smoothb(sk_swell *sw, SKFLT smooth);
#line 228 "swell.org"
void sk_swell_inertia(sk_swell *sw, SKFLT inertia);
#line 268 "swell.org"
SKFLT sk_swell_tick(sk_swell *sw, SKFLT in);
#line 43 "swell.org"
#endif
