/* This code is placed in the public domain. */

#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SK_TRACT_PRIV

#include "tract.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void tract_calculate_reflections(sk_tract *tr)
{
    int i;
    SKFLT *diam;

    diam = tr->diameter;

    if (tr->shape != NULL) {
        tr->shape(tr, tr->diameter, tr->ud);
    }

    for(i = 0; i < tr->n; i++) {
        tr->A[i] = diam[i] * diam[i];
    }

    for(i = 1; i < tr->n; i++) {
        if(tr->A[i] == 0) {
            tr->reflection[i] = 0.999; /* to prevent bad behavior if 0 */
        } else {
            tr->reflection[i] =
                (tr->A[i - 1] - tr->A[i]) / (tr->A[i - 1] + tr->A[i]);
        }
    }

}

void sk_tract_init(sk_tract *tr, int sr)
{
    int i;
    SKFLT diameter;

    tr->n = 44;
    tr->glottal_reflection = 0.75;
    tr->lip_reflection = -0.85;

    memset(tr->diameter, 0, tr->n * sizeof(SKFLT));
    memset(tr->L, 0, tr->n * sizeof(SKFLT));
    memset(tr->R, 0, tr->n * sizeof(SKFLT));
    memset(tr->reflection, 0, tr->n * sizeof(SKFLT));
    memset(tr->junction_outL, 0, tr->n * sizeof(SKFLT));
    memset(tr->junction_outR, 0, tr->n * sizeof(SKFLT));
    memset(tr->A, 0, tr->n * sizeof(SKFLT));

    for(i = 0; i < tr->n; i++) {
        diameter = 0;
        if(i < 7 * (SKFLT)tr->n / 44 - 0.5) {
            diameter = 0.6;
        } else if( i < 12 * (SKFLT)tr->n / 44) {
            diameter = 1.1;
        } else {
            diameter = 1.5;
        }

        tr->diameter[i] = diameter;
    }

    tr->shape = NULL;
    tract_calculate_reflections(tr);
}

static SKFLT tract_compute(sk_tract *tr, SKFLT in)
{
    SKFLT  r, w;
    int i;

    tr->junction_outR[0] = tr->L[0] * tr->glottal_reflection + in;
    tr->junction_outL[tr->n - 1] = tr->R[tr->n - 1] * tr->lip_reflection;

    for(i = 1; i < tr->n; i++) {
        r = tr->reflection[i];
        w = r * (tr->R[i - 1] + tr->L[i]);
        tr->junction_outR[i] = tr->R[i - 1] - w;
        tr->junction_outL[i - 1] = tr->L[i] + w;
    }

    for(i = 0; i < tr->n; i++) {
        tr->R[i] = tr->junction_outR[i]*0.999;
        tr->L[i] = tr->junction_outL[i]*0.999;
    }

    return tr->R[tr->n - 1];
}

SKFLT sk_tract_tick(sk_tract *tract, SKFLT in)
{
    SKFLT tmp;
    SKFLT out;

    out = 0;

    tract_calculate_reflections(tract);
    tmp = 0;

    /* do it twice, for some reason */
    tmp += tract_compute(tract, in);
    tmp += tract_compute(tract, in);

    out = tmp * 0.125;

    return out;
}

SKFLT* sk_tract_get_tract_diameters(sk_tract *tract)
{
    return tract->diameter;
}

void sk_tract_set_diameters(sk_tract *tract,
                          int blade_start,
                          int lip_start,
                          int tip_start,
                          SKFLT tongue_index,
                          SKFLT tongue_diameter,
                          SKFLT *diameters)
{
    int i;
    SKFLT t;
    SKFLT fixed_tongue_diameter;
    SKFLT curve;
    int grid_offset = 0;

    for(i = blade_start; i < lip_start; i++) {
        t = 1.1 * M_PI *
            (SKFLT)(tongue_index - i)/(tip_start - blade_start);
        fixed_tongue_diameter = 2+(tongue_diameter-2)/1.5;
        curve = (1.5 - fixed_tongue_diameter + grid_offset) * cos(t);
        if(i == blade_start - 2 || i == lip_start - 1) curve *= 0.8;
        if(i == blade_start || i == lip_start - 2) curve *= 0.94;
        diameters[i] = 1.5 - curve;
    }
}

void sk_tract_set_tongue_shape(sk_tract *tract,
                             SKFLT tongue_index,
                             SKFLT tongue_diameter)
{
    SKFLT *diameters;
    diameters = sk_tract_get_tract_diameters(tract);

    tongue_index = 12 + 16.0 * tongue_index;
    tongue_diameter = 3.5 * tongue_diameter;
    sk_tract_set_diameters(tract, 10, 39, 32,
                         tongue_index, tongue_diameter, diameters);
}

void sk_tract_shaper(sk_tract *tract,
                     void (*shape)(sk_tract *, SKFLT *, void *),
                     void *ud)
{
    tract->shape = shape;
    tract->ud = ud;
}
