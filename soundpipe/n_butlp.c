#include <stdlib.h>
#include <sndkit/patchwerk.h>
#include <sndkit/core.h>
#include <stdlib.h>
#include "butlp.h"

struct butlp_n {
    sp_butlp butlp;
    pw_cable *in;
    pw_cable *freq;
    pw_cable *out;
};

/* static void setup(pw_node *node) */
/* { */
/*     struct butlp_n *butlp; */
/*     pw_patch *patch; */

/*     /\* butlp = malloc(sizeof(butlp_n)); *\/ */
/*     butlp = (struct butlp_n *) pw_node_get_data(node); */

/*     pw_node_get_patch(node, &patch); */

/*     sp_butlp_init(&butlp->butlp, pw_patch_srate_get(patch)); */

/*     pw_node_cables_alloc(node, 3); */
/*     pw_node_get_cable(node, 0, &butlp->in); */

/*     pw_node_get_cable(node, 1, &butlp->freq); */
/*     pw_cable_set_constant(butlp->freq, 1000); */

/*     pw_node_get_cable(node, 2, &butlp->out); */
/*     pw_node_set_block(node, 2); */

/*     /\* pw_node_set_data(node, butlp); *\/ */
/* } */

static void destroy(pw_node *node)
{
    struct butlp_n *butlp;

    butlp = (struct butlp_n *) pw_node_get_data(node);

    pw_node_cables_free(node);

    free(butlp);
}

static void compute(pw_node *node)
{
    int i;
    int blksize;
    struct butlp_n *butlp;
    SPFLOAT in;
    SPFLOAT out;

    butlp = pw_node_get_data(node);
    blksize = pw_node_blksize(node);

    for(i = 0; i < blksize; i++) {
        in = pw_cable_get(butlp->in, i);
        butlp->butlp.freq = pw_cable_get(butlp->freq, i);
        out = sp_butlp_compute(&butlp->butlp, in);
        pw_cable_set(butlp->out, i, out);
    }
}

/* void node_butlp(pw_node *node, void *ud) */
/* { */
/*     pw_node_set_setup(node, setup); */
/*     pw_node_set_compute(node, compute); */
/*     pw_node_set_destroy(node, destroy); */
/*     pw_node_set_data(node, ud); */
/* } */

int sk_node_butlp(sk_core *core)
{
    pw_patch *patch;
    pw_node *node;
    int rc;
    sk_param in, freq;
    void *ud;
    struct butlp_n *butlp;
    int sr;

    rc = sk_param_get(core, &freq);
    SK_ERROR_CHECK(rc);

    rc = sk_param_get(core, &in);
    SK_ERROR_CHECK(rc);

    patch = sk_core_patch(core);

    rc = pw_memory_alloc(patch, sizeof(struct butlp_n), &ud);
    SK_PW_ERROR_CHECK(rc);
    butlp = (struct butlp_n *)ud;

    sr = pw_patch_srate_get(patch);
    sp_butlp_init(&butlp->butlp, sr);

    rc = pw_patch_new_node(patch, &node);
    SK_PW_ERROR_CHECK(rc);

    rc = pw_node_cables_alloc(node, 3);
    SK_PW_ERROR_CHECK(rc);

    pw_node_set_block(node, 2);

    pw_node_get_cable(node, 0, &butlp->in);
    pw_node_get_cable(node, 1, &butlp->freq);
    pw_node_get_cable(node, 2, &butlp->out);

    pw_node_set_data(node, butlp);
    pw_node_set_compute(node, compute);
    pw_node_set_destroy(node, destroy);

    sk_param_set(core, node, &in, 0);
    sk_param_set(core, node, &freq, 1);
    sk_param_out(core, node, 2);
    return 0;
}
