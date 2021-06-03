#line 46 "mtof.org"
#ifndef SK_MTOF_H
#define SK_MTOF_H
#ifndef SKFLT
#define SKFLT float
#endif

#line 85 "mtof.org"
typedef struct sk_mtof sk_mtof;
#line 46 "mtof.org"
#line 100 "mtof.org"
void sk_mtof_init(sk_mtof *mf);
#line 116 "mtof.org"
SKFLT sk_mtof_tick(sk_mtof *mf, SKFLT nn);
#line 46 "mtof.org"
#line 75 "mtof.org"
#define SK_MTOF(f) (pow(2, (nn - 69.0) / 12.0) * 440.0);
#line 55 "mtof.org"
#ifdef SK_MTOF_PRIV
#line 90 "mtof.org"
struct sk_mtof {
   SKFLT freq;
   SKFLT nn;
};
#line 57 "mtof.org"
#endif

#endif
