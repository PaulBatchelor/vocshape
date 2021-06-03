#ifndef SK_GLOTTIS_H
#define SK_GLOTTIS_H

#ifndef SKFLT
#define SKFLT float
#endif

typedef struct sk_glottis sk_glottis;

#ifdef SK_GLOTTIS_PRIV
struct sk_glottis {
    SKFLT freq;
    SKFLT tenseness;
    SKFLT Rd;
    SKFLT waveform_length;
    SKFLT time_in_waveform;

    SKFLT alpha;
    SKFLT E0;
    SKFLT epsilon;
    SKFLT shift;
    SKFLT delta;
    SKFLT Te;
    SKFLT omega;

    SKFLT T;
};
#endif

void sk_glottis_init(sk_glottis *glot, SKFLT sr);
SKFLT sk_glottis_tick(sk_glottis *glot);
void sk_glottis_freq(sk_glottis *glot, SKFLT freq);
void sk_glottis_tenseness(sk_glottis *glot, SKFLT tenseness);

#endif
