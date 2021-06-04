#include <stdlib.h>
#include "sndkit/patchwerk.h"
#include "butlp.h"

typedef struct {
    sp_butlp butlp;
    pw_cable *in;
    pw_cable *p_freq;
    pw_cable *out;
} butlp_d;

static void setup(pw_node *node)
{
    butlp_d *butlp;
    pw_patch *patch;

    butlp = malloc(sizeof(butlp_d));

    pw_node_get_patch(node, &patch);

    sp_butlp_init(&butlp->butlp, pw_patch_srate_get(patch));

    pw_node_cables_alloc(node, 3);
    pw_node_get_cable(node, 0, &butlp->in);

    pw_node_get_cable(node, 1, &butlp->p_freq);
    pw_cable_set_constant(butlp->p_freq, 1000);

    pw_node_get_cable(node, 2, &butlp->out);
    pw_node_set_block(node, 2);

    pw_node_set_data(node, butlp);
}

static void destroy(pw_node *node)
{
    butlp_d *butlp;

    butlp = (butlp_d *) pw_node_get_data(node);

    pw_node_cables_free(node);

    free(butlp);
}

static void compute(pw_node *node)
{
    int i;
    int blksize;
    butlp_d *butlp;
    SPFLOAT in;
    SPFLOAT out;

    butlp = pw_node_get_data(node);
    blksize = pw_node_blksize(node);

    for(i = 0; i < blksize; i++) {
        in = pw_cable_get(butlp->in, i);
        butlp->butlp.freq = pw_cable_get(butlp->p_freq, i);
        out = sp_butlp_compute(&butlp->butlp, in);
        pw_cable_set(butlp->out, i, out);
    }
}

void node_butlp(pw_node *node, void *ud)
{
    pw_node_set_setup(node, setup);
    pw_node_set_compute(node, compute);
    pw_node_set_destroy(node, destroy);
    pw_node_set_data(node, ud);
}
