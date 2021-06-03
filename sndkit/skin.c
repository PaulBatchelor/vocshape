#include <sndkit/patchwerk.h>
#include <sndkit/core.h>
#include <stdlib.h>

struct skin_n {
    SKFLT *in;
    pw_cable *out;
};

static void compute(pw_node *node)
{
    int blksize;
    int n;
    struct skin_n *in;

    blksize = pw_node_blksize(node);

    in = (struct skin_n *)pw_node_get_data(node);

    for (n = 0; n < blksize; n++) {
        pw_cable_set(in->out, n, *in->in);
    }
}

static void destroy(pw_node *node)
{
    pw_patch *patch;
    int rc;
    void *ud;
    rc = pw_node_get_patch(node, &patch);
    if (rc != PW_OK) return;
    pw_node_cables_free(node);
    ud = pw_node_get_data(node);
    pw_memory_free(patch, &ud);
}

int sk_node_in(sk_core *core, SKFLT *param)
{
    pw_patch *patch;
    pw_node *node;
    int rc;
    void *ud;
    struct skin_n *in;
    int sr;

    patch = sk_core_patch(core);

    rc = pw_memory_alloc(patch, sizeof(struct skin_n), &ud);
    SK_PW_ERROR_CHECK(rc);
    in = (struct skin_n *)ud;
    in->in = param;

    sr = pw_patch_srate_get(patch);

    rc = pw_patch_new_node(patch, &node);
    SK_PW_ERROR_CHECK(rc);

    rc = pw_node_cables_alloc(node, 1);
    SK_PW_ERROR_CHECK(rc);

    pw_node_set_block(node, 0);

    pw_node_get_cable(node, 0, &in->out);

    pw_node_set_data(node, in);
    pw_node_set_compute(node, compute);
    pw_node_set_destroy(node, destroy);

    sk_param_out(core, node, 0);
    return 0;
}
