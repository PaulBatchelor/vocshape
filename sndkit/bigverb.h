#line 106 "bigverb.org"
#ifndef SK_BIGVERB_H
#define SK_BIGVERB_H

#ifndef SKFLT
#define SKFLT float
#endif
#line 125 "bigverb.org"
typedef struct sk_bigverb sk_bigverb;
#line 499 "bigverb.org"
typedef struct sk_bigverb_delay sk_bigverb_delay;
#line 106 "bigverb.org"
#line 143 "bigverb.org"
sk_bigverb * sk_bigverb_new(int sr);
#line 167 "bigverb.org"
void sk_bigverb_del(sk_bigverb *bv);
#line 190 "bigverb.org"
void sk_bigverb_size(sk_bigverb *bv, SKFLT size);
#line 222 "bigverb.org"
void sk_bigverb_cutoff(sk_bigverb *bv, SKFLT cutoff);
#line 289 "bigverb.org"
void sk_bigverb_tick(sk_bigverb *bv,
                     SKFLT inL, SKFLT inR,
                     SKFLT *outL, SKFLT *outR);
#line 114 "bigverb.org"

#ifdef SK_BIGVERB_PRIV
#line 125 "bigverb.org"
#line 504 "bigverb.org"
struct sk_bigverb_delay {
#line 535 "bigverb.org"
SKFLT *buf;
size_t sz;
#line 549 "bigverb.org"
int wpos;
#line 564 "bigverb.org"
int irpos;
int frpos;
#line 573 "bigverb.org"
int rng;
#line 883 "bigverb.org"
int inc;
int counter;
#line 935 "bigverb.org"
int maxcount;
#line 973 "bigverb.org"
SKFLT dels;
#line 983 "bigverb.org"
SKFLT drift;
#line 1034 "bigverb.org"
SKFLT y;
#line 506 "bigverb.org"
};
#line 131 "bigverb.org"
struct sk_bigverb {
    int sr;
#line 202 "bigverb.org"
SKFLT size;
#line 246 "bigverb.org"
SKFLT cutoff;
SKFLT pcutoff;
#line 267 "bigverb.org"
SKFLT filt;
#line 397 "bigverb.org"
SKFLT *buf;
#line 471 "bigverb.org"
sk_bigverb_delay delay[8];
#line 134 "bigverb.org"
};
#line 117 "bigverb.org"
#endif
#endif
