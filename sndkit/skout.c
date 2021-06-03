#include <sndkit/patchwerk.h>
#include <sndkit/core.h>
#include <stdlib.h>

struct out_n {
    pw_cable *in;
    SKFLT *buf;
};

static void compute(pw_node *node)
{
    int blksize;
    int n;
    struct out_n *out;

    blksize = pw_node_blksize(node);

    out = (struct out_n *)pw_node_get_data(node);

    for (n = 0; n < blksize; n++) {
        PWFLT in;
        in = pw_cable_get(out->in, n);
        out->buf[n] = in;
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

int sk_node_out(sk_core *core, SKFLT *buf)
{
    pw_patch *patch;
    pw_node *node;
    int rc;
    sk_param in;
    void *ud;
    struct out_n *out;
    int sr;

    rc = sk_param_get(core, &in);
    SK_ERROR_CHECK(rc);

    patch = sk_core_patch(core);

    rc = pw_memory_alloc(patch, sizeof(struct out_n), &ud);
    SK_PW_ERROR_CHECK(rc);
    out = (struct out_n *)ud;
    out->buf = buf;

    sr = pw_patch_srate_get(patch);

    rc = pw_patch_new_node(patch, &node);
    SK_PW_ERROR_CHECK(rc);

    rc = pw_node_cables_alloc(node, 1);
    SK_PW_ERROR_CHECK(rc);

    pw_node_get_cable(node, 0, &out->in);

    pw_node_set_data(node, out);
    pw_node_set_compute(node, compute);
    pw_node_set_destroy(node, destroy);

    sk_param_set(core, node, &in, 0);
    return 0;
}
