#ifndef SPFLOAT
#define SPFLOAT float
#endif

typedef struct  {
    SPFLOAT freq;
    SPFLOAT lfreq;
    SPFLOAT a[7];
    SPFLOAT pidsr;
} sp_butlp;

void sp_butlp_init(sp_butlp *p, int sr);
SPFLOAT sp_butlp_compute(sp_butlp *p, SPFLOAT in);
