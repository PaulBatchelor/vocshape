#ifndef SK_TRACT_H
#define SK_TRACT_H

#ifndef SKFLT
#define SKFLT float
#endif

typedef struct sk_tract sk_tract;

void sk_tract_init(sk_tract *tract, int sr);
SKFLT sk_tract_tick(sk_tract *tract, SKFLT in);
int sk_tract_tract_compute(sk_tract *tract, SKFLT *in, SKFLT *out);

SKFLT* sk_tract_get_tract_diameters(sk_tract *tract);
int sk_tract_get_tract_size(sk_tract *tract);
void sk_tract_set_tongue_shape(sk_tract *tract,
                               SKFLT tongue_index,
                               SKFLT tongue_diameter);

void sk_tract_set_diameters(sk_tract *tract,
                            int blade_start,
                            int lip_start,
                            int tip_start,
                            SKFLT tongue_index,
                            SKFLT tongue_diameter,
                            SKFLT *diameters);

void sk_tract_shaper(sk_tract *tract,
                     void (*shape)(sk_tract *, SKFLT *, void *),
                     void *ud);

#ifdef SK_TRACT_PRIV
struct sk_tract {
    int n;

    SKFLT diameter[44];
    SKFLT R[44];
    SKFLT L[44];
    SKFLT A[44];
    SKFLT reflection[44];
    SKFLT junction_outL[44];
    SKFLT junction_outR[44];

    SKFLT glottal_reflection;
    SKFLT lip_reflection;

    void *ud;
    void (*shape)(sk_tract *, SKFLT *, void *);
};
#endif

#endif
