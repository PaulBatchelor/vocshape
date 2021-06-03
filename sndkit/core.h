#line 18 "core.org"
#ifndef SKPW_H
#define SKPW_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 105 "core.org"
typedef struct sk_core sk_core;
#line 206 "core.org"
typedef struct sk_stacklet sk_stacklet;
#line 241 "core.org"
typedef struct sk_stack sk_stack;
#line 563 "core.org"
typedef struct {
    char type;
    union {
        pw_cable *c;
        SKFLT f;
    } data;
} sk_param;
#line 815 "core.org"
typedef struct sk_register_entry sk_register_entry;
#line 853 "core.org"
typedef struct sk_regtbl sk_regtbl;
#line 1167 "core.org"
typedef struct sk_table sk_table;
#line 26 "core.org"
#ifdef SK_CORE_PRIV
#line 211 "core.org"
struct sk_stacklet {
    int type;
    SKFLT f;
    void *ptr;
};
#line 250 "core.org"
#define SK_STACKSIZE 16
struct sk_stack {
    sk_stacklet stack[SK_STACKSIZE];
    int pos;
};
#line 211 "core.org"
#line 823 "core.org"
struct sk_register_entry {
    sk_stacklet data;
    int flags;
};
#line 858 "core.org"
#define SK_REGSIZE 16
struct sk_regtbl {
    sk_register_entry r[SK_REGSIZE];
};
#line 1172 "core.org"
struct sk_table {
    SKFLT *tab;
    unsigned long sz;
};
#line 26 "core.org"
#line 110 "core.org"
struct sk_core {
    pw_patch *patch;
    sk_stack stack;
    sk_regtbl regtbl;
    unsigned long rng;
};
#line 29 "core.org"
#endif
#line 1298 "core.org"
#define SK_ERROR_CHECK(rc) if (rc) return rc;
#line 1303 "core.org"
#define SK_PW_ERROR_CHECK(rc) if(rc != PW_OK) {\
    fprintf(stderr, "Error: %s\n", pw_error(rc));\
    return 1;\
}
#line 1339 "core.org"
#define SK_CORE_RANDMAX 2147483648
#line 29 "core.org"
#line 61 "core.org"
sk_core * sk_core_new(int sr);
void sk_core_del(sk_core *core);
#line 123 "core.org"
void sk_core_compute(sk_core *core);
#line 141 "core.org"
size_t sk_core_computes(sk_core *core, SKFLT secs);
#line 165 "core.org"
pw_patch * sk_core_patch(sk_core *core);
#line 181 "core.org"
size_t sk_core_seconds_to_blocks(sk_core *core, SKFLT secs);
#line 224 "core.org"
void sk_stacklet_init(sk_stacklet *s);
#line 261 "core.org"
void sk_stack_init(sk_stack *s);
#line 306 "core.org"
int sk_stack_pop(sk_stack *stack, sk_stacklet **out);
int sk_stack_push(sk_stack *stack, sk_stacklet **out);
#line 350 "core.org"
int sk_stack_peak(sk_stack *stack, sk_stacklet **out);
#line 375 "core.org"
int sk_stack_dup(sk_stack *stack, sk_stacklet **out);
#line 406 "core.org"
int sk_core_dup(sk_core *core);
#line 438 "core.org"
int sk_stack_drop(sk_stack *stack, sk_stacklet **out);
#line 463 "core.org"
int sk_core_drop(sk_core *core);
#line 493 "core.org"
int sk_stack_swap(sk_stack *stack,
                  sk_stacklet **s1,
                  sk_stacklet **s2);
#line 527 "core.org"
int sk_core_swap(sk_core *core);
#line 576 "core.org"
int sk_param_get(sk_core *core, sk_param *p);
#line 619 "core.org"
int sk_param_get_constant(sk_core *core, SKFLT *val);
#line 649 "core.org"
int sk_param_set(sk_core *core,
                 pw_node *node,
                 sk_param *p,
                 int cid);
#line 687 "core.org"
int sk_core_constant(sk_core *core, SKFLT x);
#line 715 "core.org"
int sk_param_out(sk_core *core,
                 pw_node *node,
                 int cid);
#line 757 "core.org"
int sk_core_generic_push(sk_core *core, void *ptr);
int sk_core_generic_pop(sk_core *core, void **ptr);
#line 834 "core.org"
void sk_register_entry_init(sk_register_entry *e);
#line 868 "core.org"
void sk_regtbl_init(sk_regtbl *rs);
#line 894 "core.org"
int sk_core_regget(sk_core *core, int pos);
int sk_register_get(sk_regtbl *rt, int pos, sk_stacklet *s);
int sk_core_regset(sk_core *core, int pos);
int sk_register_set(sk_regtbl *rt, int pos, sk_stacklet *s);
#line 974 "core.org"
int sk_core_regmrk(sk_core *core, int pos);
int sk_register_mark(sk_regtbl *rt, int pos);
#line 1003 "core.org"
int sk_core_regclr(sk_core *core, int pos);
int sk_register_clear(sk_regtbl *rt, int pos);
#line 1034 "core.org"
int sk_core_regnext(sk_core *core, int start, int *pos);
int sk_register_nextfree(sk_regtbl *rt, int start);
#line 1104 "core.org"
int sk_core_hold(sk_core *core);
int sk_core_unhold(sk_core *core);
#line 1186 "core.org"
int sk_core_table_new(sk_core *core, unsigned long sz);
#line 1227 "core.org"
size_t sk_table_size(sk_table *t);
SKFLT* sk_table_data(sk_table *t);
#line 1248 "core.org"
int sk_core_table_push(sk_core *core, sk_table *tab);
int sk_core_table_pop(sk_core *core, sk_table **tab);
#line 1317 "core.org"
void sk_core_srand(sk_core *core, unsigned long val);
#line 1332 "core.org"
unsigned long sk_core_rand(sk_core *core);
#line 32 "core.org"

#ifdef __plan9__
#pragma incomplete sk_core
#pragma incomplete sk_stack
#pragma incomplete sk_regtbl
#pragma incomplete sk_table
#endif
#endif
