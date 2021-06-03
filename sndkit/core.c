#line 44 "core.org"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "patchwerk.h"
#define SK_CORE_PRIV
#include "core.h"
#line 285 "core.org"
enum {
   SK_TYPE_NONE,
   SK_TYPE_CONSTANT,
   SK_TYPE_CABLE,
   SK_TYPE_TABLE,
   SK_TYPE_GENERIC
};
#line 44 "core.org"
#line 67 "core.org"
sk_core * sk_core_new(int sr)
{
    sk_core *core;
    pw_patch *patch;

    core = malloc(sizeof(sk_core));
    core->patch = malloc(pw_patch_size());

    patch = core->patch;
    pw_patch_init(patch, 64);
    pw_patch_alloc(patch, 8, 10);
    pw_patch_srate_set(patch, sr);

    sk_stack_init(&core->stack);
    sk_regtbl_init(&core->regtbl);

    sk_core_srand(core, 0);
    return core;
}
#line 90 "core.org"
void sk_core_del(sk_core *core)
{
    if (core == NULL) return;

    pw_patch_destroy(core->patch);
    pw_patch_free_nodes(core->patch);
    free(core->patch);

    free(core);
    core = NULL;
}
#line 128 "core.org"
void sk_core_compute(sk_core *core)
{
    pw_patch_compute(core->patch);
}
#line 146 "core.org"
size_t sk_core_computes(sk_core *core, SKFLT secs)
{
    size_t nblocks, n;
    nblocks = sk_core_seconds_to_blocks(core, secs);

    for (n = 0; n < nblocks; n++) {
        sk_core_compute(core);
    }

    return nblocks;
}
#line 170 "core.org"
pw_patch * sk_core_patch(sk_core *core)
{
    return core->patch;
}
#line 186 "core.org"
size_t sk_core_seconds_to_blocks(sk_core *core, SKFLT secs)
{
    size_t nblocks;
    int sr;

    sr = pw_patch_srate_get(core->patch);
    nblocks = floor((sr * secs) / 64) + 1;

    return nblocks;
}
#line 229 "core.org"
void sk_stacklet_init(sk_stacklet *s)
{
    s->type = SK_TYPE_NONE;
    s->f = 0;
    s->ptr = NULL;
}
#line 269 "core.org"
void sk_stack_init(sk_stack *s)
{
    int i;

    for (i = 0; i < SK_STACKSIZE; i++) {
        sk_stacklet_init(&s->stack[i]);
    }

    s->pos = -1;
}
#line 312 "core.org"
int sk_stack_pop(sk_stack *stack, sk_stacklet **out)
{
    sk_stacklet *s;
    /* no items on stack */
    if (stack->pos < 0) return 1;

    /* stack overflow */
    if (stack->pos >= SK_STACKSIZE) return 2;

    s = &stack->stack[stack->pos];
    stack->pos--;

    *out = s;
    return 0;
}
#line 331 "core.org"
int sk_stack_push(sk_stack *stack, sk_stacklet **out)
{
    sk_stacklet *s;

    if (stack->pos >= (SK_STACKSIZE - 1)) return 1;
    stack->pos++;
    s = &stack->stack[stack->pos];

    sk_stacklet_init(s);
    *out = s;
    return 0;
}
#line 355 "core.org"
int sk_stack_peak(sk_stack *stack, sk_stacklet **out)
{
    sk_stacklet *s;
    if (stack->pos < 0) return 1;
    if (stack->pos >= SK_STACKSIZE) return 2;

    s = &stack->stack[stack->pos];
    *out = s;
    return 0;
}
#line 380 "core.org"
int sk_stack_dup(sk_stack *stack, sk_stacklet **out)
{
    int rc;
    sk_stacklet *a, *b;

    rc = sk_stack_peak(stack, &a);
    SK_ERROR_CHECK(rc);

    rc = sk_stack_push(stack, &b);
    SK_ERROR_CHECK(rc);

    *b = *a;

    if (out != NULL) *out = b;

    return 0;
}
#line 411 "core.org"
int sk_core_dup(sk_core *core)
{
    sk_stacklet *s;
    int rc;

    rc = sk_stack_dup(&core->stack, &s);
    SK_ERROR_CHECK(rc);

    if (s->type == SK_TYPE_CABLE) {
        pw_stack *stack;
        stack = pw_patch_stack(core->patch);
        pw_stack_dup(stack);
    }

    return 0;
}
#line 443 "core.org"
int sk_stack_drop(sk_stack *stack, sk_stacklet **out)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_pop(stack, &s);

    SK_ERROR_CHECK(rc);

    if (out != NULL) *out = s;
    return 0;
}
#line 468 "core.org"
int sk_core_drop(sk_core *core)
{
    int rc;
    sk_stacklet *s;
    rc = sk_stack_drop(&core->stack, &s);

    SK_ERROR_CHECK(rc);

    if (s->type == SK_TYPE_CABLE) {
        pw_stack *stack;
        stack = pw_patch_stack(core->patch);
        pw_stack_pop(stack, NULL);
    }

    return 0;
}
#line 500 "core.org"
int sk_stack_swap(sk_stack *stack,
                  sk_stacklet **out1,
                  sk_stacklet **out2)
{
    sk_stacklet tmp;
    int pos;

    pos = stack->pos;
    if (pos < 2) return 1;

    tmp = stack->stack[pos];

    stack->stack[pos] = stack->stack[pos - 1];
    stack->stack[pos - 1] = tmp;

    if (out1 != NULL) *out1 = &stack->stack[pos - 1];
    if (out2 != NULL) *out2 = &stack->stack[pos];

    return 0;
}
#line 532 "core.org"
int sk_core_swap(sk_core *core)
{
    int rc;
    sk_stacklet *s[2];
    rc = sk_stack_swap(&core->stack, &s[0], &s[1]);

    SK_ERROR_CHECK(rc);

    if (s[0]->type == SK_TYPE_CABLE && s[1]->type == SK_TYPE_CABLE) {
        pw_stack *stack;
        stack = pw_patch_stack(core->patch);
        pw_stack_swap(stack);
    }

    return 0;
}
#line 587 "core.org"
int sk_param_get(sk_core *core, sk_param *p)
{
    sk_stack *stk;
    sk_stacklet *s;
    int rc;

    stk = &core->stack;

    rc = sk_stack_pop(stk, &s);
    SK_ERROR_CHECK(rc);

    if (s->type == SK_TYPE_CONSTANT) {
        p->type = 0;
        p->data.f = s->f;
    } else if (s->type == SK_TYPE_CABLE) {
        p->type = 1;
        p->data.c = (pw_cable *)s->ptr;
        pw_cable_pop(p->data.c);
    } else {
        /* Wrong type! */
        return 1;
    }

    return 0;
}
#line 624 "core.org"
int sk_param_get_constant(sk_core *core, SKFLT *val)
{
    sk_stack *stk;
    sk_stacklet *s;
    int rc;

    stk = &core->stack;

    rc = sk_stack_pop(stk, &s);
    SK_ERROR_CHECK(rc);

    if (s->type != SK_TYPE_CONSTANT) {
        /* Wrong type! */
        return 1;
    }

    *val = s->f;
    return 0;
}
#line 657 "core.org"
int sk_param_set(sk_core *core,
                 pw_node *node,
                 sk_param *p,
                 int cid)
{
    pw_cable *c;

    pw_node_get_cable(node, cid, &c);
    if (p->type == 0) {
        pw_cable_set_value(c, p->data.f);
    } else {
        int rc;
        rc = pw_cable_connect(p->data.c, c);
        SK_PW_ERROR_CHECK(rc);
    }
    return 0;
}
#line 692 "core.org"
int sk_core_constant(sk_core *core, SKFLT x)
{
    int rc;
    sk_stacklet *s;
    sk_stack *stk;

    stk = &core->stack;

    rc = sk_stack_push(stk, &s);
    SK_ERROR_CHECK(rc);

    s->type = SK_TYPE_CONSTANT;
    s->f = x;

    return 0;
}
#line 728 "core.org"
int sk_param_out(sk_core *core,
                 pw_node *node,
                 int cid)
{
    pw_cable *c;
    sk_stacklet *s;
    sk_stack *stk;
    int rc;

    stk = &core->stack;

    rc = sk_stack_push(stk, &s);
    SK_ERROR_CHECK(rc);
    rc = pw_node_get_cable(node, cid, &c);
    SK_PW_ERROR_CHECK(rc);

    s->type = SK_TYPE_CABLE;
    s->ptr = c;

    return 0;
}
#line 763 "core.org"
int sk_core_generic_push(sk_core *core, void *ptr)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_push(&core->stack, &s);
    SK_ERROR_CHECK(rc);

    s->type = SK_TYPE_GENERIC;
    s->ptr = ptr;

    return rc;
}
#line 780 "core.org"
int sk_core_generic_pop(sk_core *core, void **ptr)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_pop(&core->stack, &s);

    SK_ERROR_CHECK(rc);

    if (s->type != SK_TYPE_GENERIC) {
        return 1;
    }

    if (ptr != NULL) *ptr = s->ptr;

    return rc;
}
#line 839 "core.org"
void sk_register_entry_init(sk_register_entry *e)
{
    sk_stacklet_init(&e->data);
    e->flags = 0;
}
#line 873 "core.org"
void sk_regtbl_init(sk_regtbl *rs)
{
    int i;

    for (i = 0; i < SK_REGSIZE; i++) {
        sk_register_entry_init(&rs->r[i]);
    }
}
#line 902 "core.org"
int sk_register_get(sk_regtbl *rt, int pos, sk_stacklet *s)
{
    if (pos < 0 || pos >= SK_REGSIZE) return 1;

    *s = rt->r[pos].data;
    return 0;
}
#line 917 "core.org"
int sk_core_regget(sk_core *core, int pos)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_push(&core->stack, &s);
    SK_ERROR_CHECK(rc);
    rc = sk_register_get(&core->regtbl, pos, s);
    SK_ERROR_CHECK(rc);

    /* also push to buffer stack if cable */
    if (s->type == SK_TYPE_CABLE) {
        pw_cable *c;
        pw_buffer *b;
        pw_stack *bstack;
        c = (pw_cable *) s->ptr;
        b = pw_cable_get_buffer(c);
        bstack = pw_patch_stack(core->patch);
        pw_stack_push_buffer(bstack, b);
    }

    return 0;
}
#line 944 "core.org"
int sk_register_set(sk_regtbl *rt, int pos, sk_stacklet *s)
{
    if (pos < 0 || pos >= SK_REGSIZE) return 1;

    rt->r[pos].data = *s;
    return 0;
}
#line 955 "core.org"
int sk_core_regset(sk_core *core, int pos)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_pop(&core->stack, &s);
    SK_ERROR_CHECK(rc);
    rc = sk_register_set(&core->regtbl, pos, s);
    SK_ERROR_CHECK(rc);

    return 0;
}
#line 980 "core.org"
int sk_register_mark(sk_regtbl *rt, int pos)
{
    if (pos < 0 || pos >= SK_REGSIZE) return 1;

    rt->r[pos].flags |= 1;
    return 0;
}
#line 991 "core.org"
int sk_core_regmrk(sk_core *core, int pos)
{
    return sk_register_mark(&core->regtbl, pos);
}
#line 1009 "core.org"
int sk_register_clear(sk_regtbl *rt, int pos)
{
    if (pos < 0 || pos >= SK_REGSIZE) return 1;

    rt->r[pos].flags = 0;
    return 0;
}
#line 1020 "core.org"
int sk_core_regclr(sk_core *core, int pos)
{
    return sk_register_clear(&core->regtbl, pos);
}
#line 1044 "core.org"
int sk_register_nextfree(sk_regtbl *rt, int start)
{
    int pos;
    int i;
    sk_register_entry *reg;

    reg = rt->r;

    if (start < 0 || start >= SK_REGSIZE) start = 0;

    pos = start;

    for (i = 0; i < SK_REGSIZE; i++) {
        if (!(reg[pos].flags & 1)) return pos;
        pos = (pos + 1) % SK_REGSIZE;
    }

    return -1;
}
#line 1071 "core.org"
int sk_core_regnext(sk_core *core, int start, int *pos)
{
    *pos = sk_register_nextfree(&core->regtbl, start);

    if (*pos < 0) return 1;

    return 0;
}
#line 1110 "core.org"
int sk_core_hold(sk_core *core)
{
    int rc;
    sk_stacklet *s;
    rc = sk_stack_peak(&core->stack, &s);
    SK_ERROR_CHECK(rc);

    if (s->type != SK_TYPE_CABLE) {
        /* Wrong type, kiddo */
        return 2;
    }

    rc = pw_patch_bhold(core->patch, NULL);
    SK_PW_ERROR_CHECK(rc);

    return 0;
}
#line 1131 "core.org"
int sk_core_unhold(sk_core *core)
{
    sk_param cable;
    pw_buffer *buf;
    pw_cable *c;
    int rc;

    rc = sk_param_get(core, &cable);
    SK_ERROR_CHECK(rc);

    if (cable.type != 1) {
        /* Your princess is is another castle. */
        return 2;
    }

    c = cable.data.c;
    buf = pw_cable_get_buffer(c);
    rc = pw_patch_bunhold(core->patch, buf);
    SK_PW_ERROR_CHECK(rc);

    return 0;
}
#line 1191 "core.org"
static void free_table(pw_pointer *p)
{
    sk_table *tab;

    tab = pw_pointer_data(p);

    free(tab->tab);
    free(tab);
}

int sk_core_table_new(sk_core *core, unsigned long sz)
{
    sk_table *tab;
    int rc;

    tab = malloc(sizeof(sk_table));

    if (tab == NULL) return 1;

    tab->tab = malloc(sz * sizeof(SKFLT));
    memset(tab->tab, 0, sz * sizeof(SKFLT));
    tab->sz = sz;

    pw_patch_append_userdata(core->patch, free_table, tab);

    rc = sk_core_table_push(core, tab);
    SK_ERROR_CHECK(rc);

    return 0;
}
#line 1233 "core.org"
size_t sk_table_size(sk_table *t)
{
    return t->sz;
}

SKFLT* sk_table_data(sk_table *t)
{
    return t->tab;
}
#line 1254 "core.org"
int sk_core_table_push(sk_core *core, sk_table *tab)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_push(&core->stack, &s);

    SK_ERROR_CHECK(rc);

    s->type = SK_TYPE_TABLE;
    s->ptr = tab;

    return rc;
}
#line 1272 "core.org"
int sk_core_table_pop(sk_core *core, sk_table **tab)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_pop(&core->stack, &s);

    SK_ERROR_CHECK(rc);

    if (s->type != SK_TYPE_TABLE) {
        printf("uh oh type is %d, %g\n", s->type, s->f);
        printf("%d items on stack\n", core->stack.pos);
        return 1;
    }

    *tab = (sk_table *)s->ptr;

    return rc;
}
#line 1322 "core.org"
void sk_core_srand(sk_core *core, unsigned long val)
{
    core->rng = val;
}
#line 1344 "core.org"
unsigned long sk_core_rand(sk_core *core)
{
    core->rng = (1103515245 * core->rng + 12345) % SK_CORE_RANDMAX;
    return core->rng;
}
#line 44 "core.org"
