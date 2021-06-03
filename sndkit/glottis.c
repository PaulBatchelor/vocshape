#include <math.h>
#include <stdlib.h>
#define SK_GLOTTIS_PRIV
#include "glottis.h"

static void setup_waveform(sk_glottis *glot)
{

    SKFLT Rd;
    SKFLT Ra;
    SKFLT Rk;
    SKFLT Rg;

    SKFLT Ta;
    SKFLT Tp;
    SKFLT Te;

    SKFLT epsilon;
    SKFLT shift;
    SKFLT delta;
    SKFLT rhs_integral;

    SKFLT lower_integral;
    SKFLT upper_integral;

    SKFLT omega;
    SKFLT s;
    SKFLT y;
    SKFLT z;

    SKFLT alpha;
    SKFLT E0;


    glot->Rd = 3 * (1 - glot->tenseness);
    glot->waveform_length = 1.0 / glot->freq;

    Rd = glot->Rd;
    if(Rd < 0.5) Rd = 0.5;
    if(Rd > 2.7) Rd = 2.7;

    Ra = -0.01 + 0.048*Rd;
    Rk = 0.224 + 0.118*Rd;
    Rg = (Rk/4)*(0.5 + 1.2*Rk)/(0.11*Rd-Ra*(0.5+1.2*Rk));

    Ta = Ra;
    Tp = (SKFLT)1.0 / (2*Rg);
    Te = Tp + Tp*Rk;

    epsilon = (SKFLT)1.0 / Ta;
    shift = exp(-epsilon * (1 - Te));
    delta = 1 - shift;

    rhs_integral = (SKFLT)(1.0/epsilon) * (shift-1) + (1-Te)*shift;
    rhs_integral = rhs_integral / delta;
    lower_integral = - (Te - Tp) / 2 + rhs_integral;
    upper_integral = -lower_integral;

    omega = M_PI / Tp;
    s = sin(omega * Te);

    y = -M_PI * s * upper_integral / (Tp*2);
    z = log(y);
    alpha = z / (Tp/2 - Te);
    E0 = -1 / (s * exp(alpha*Te));

    glot->alpha = alpha;
    glot->E0 = E0;
    glot->epsilon = epsilon;
    glot->shift = shift;
    glot->delta = delta;
    glot->Te = Te;
    glot->omega = omega;
}

void sk_glottis_init(sk_glottis *glot, SKFLT sr)
{
    glot->freq = 140; /* 140Hz frequency by default */
    glot->tenseness = 0.6; /* value between 0 and 1 */
    glot->T = 1.0/sr; /* big T */
    glot->time_in_waveform = 0;
    setup_waveform(glot);
}

SKFLT sk_glottis_tick(sk_glottis *glot)
{
    SKFLT out;
    SKFLT aspiration;
    SKFLT noise;
    SKFLT t;
    SKFLT intensity;

    out = 0;
    intensity = 1.0;
    glot->time_in_waveform += glot->T;

    if (glot->time_in_waveform > glot->waveform_length) {
        glot->time_in_waveform -= glot->waveform_length;
        setup_waveform(glot);

    }

    t = (glot->time_in_waveform / glot->waveform_length);

    if (t > glot->Te) {
        out = (-exp(-glot->epsilon * (t-glot->Te)) + glot->shift) / glot->delta;
    } else {
        out = glot->E0 * exp(glot->alpha * t) * sin(glot->omega * t);
    }

    /* TODO: build more portable RNG */
    noise = 2.0 * ((SKFLT) rand() / RAND_MAX) - 1;

    aspiration = intensity * (1 - sqrt(glot->tenseness)) * 0.3 * noise;

    aspiration *= 0.2;

    out += aspiration;

    return out;
}

void sk_glottis_freq(sk_glottis *glot, SKFLT freq)
{
    glot->freq = freq;
}

void sk_glottis_tenseness(sk_glottis *glot, SKFLT tenseness)
{
    glot->tenseness = tenseness;
}
