/*
 * Butlp
 *
 * This is an implementation of a 2nd-order butterworth
 * lowpass filter, discretized using the bilinear transform.
 *
 * For more information on using the BLT on 2nd-order
 * butterworth filters, see:
 *
 * https://ccrma.stanford.edu/~jos/filters/Example_Second_Order_Butterworth_Lowpass.html
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#define ROOT2 (1.4142135623730950488)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "butlp.h"

static SPFLOAT filter(SPFLOAT in, SPFLOAT *a)
{
    SPFLOAT t, y;
    /* a5 = t(n - 1); a6 = t(n - 2) */
    t = in - a[3]*a[5] - a[4]*a[6];
    y = t*a[0] + a[1]*a[5] + a[2]*a[6];
    a[6] = a[5];
    a[5] = t;
    return y;
}

void sp_butlp_init(sp_butlp *p, int sr)
{
    p->freq = 1000;
    p->pidsr = M_PI / sr * 1.0;
    p->a[5] = p->a[6] = 0.0;
    p->lfreq = 0.0;
}

SPFLOAT sp_butlp_compute(sp_butlp *p, SPFLOAT in)
{
    if (p->freq <= 0.0) {
        return 0;
    }

    if (p->freq != p->lfreq) {
        SPFLOAT *a, c;
        a = p->a;
        p->lfreq = p->freq;
        /* derive C constant used in BLT */
        c = 1.0 / tan((SPFLOAT)(p->pidsr * p->lfreq));

        /* perform BLT, store components */
        a[0] = 1.0 / (1.0 + c*ROOT2 + c*c);
        a[1] = 2*a[0];
        a[2] = a[0];
        a[3] = 2.0 * (1.0 - c*c) * a[0];
        a[4] = (1.0 - c*ROOT2 + c*c) * a[0];
    }

    return filter(in, p->a);
}
