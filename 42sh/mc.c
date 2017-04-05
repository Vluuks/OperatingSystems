#include "mc.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

mc *init_mc()
{
    mc *res = malloc(sizeof(mc));
    if (NULL == res) exit(EXIT_FAILURE);
    res->n = 0;
    res->header = NULL;
    return res;
}

void register_mem(mc *m, void *pt, const free_fun fun)
{
    m_node *new_node = malloc(sizeof(m_node));
    if (NULL == new_node) {
        free_all_mem(m);
        fun(pt);
        exit(EXIT_FAILURE);
    }
    new_node->fun = fun;
    new_node->pt = pt;
    new_node->next = m->header;
    m->header = new_node;
    m->n++;
}

void free_all_mem(mc *m)
{
    m_node *cur = m->header, *next;
    for (size_t i = 0; i < m->n; i++) {
        next = cur->next;
        cur->fun(cur->pt);
        free(cur);
        cur = next;
    }
    free(m);
}

m_node *unregister_mem(mc *m, const void *pt)
{
    m_node *cur = m->header->next, *prev = m->header;
    m->n--;
    if (pt == prev->pt) {
        m->header = cur;
        return prev;
    }
    for (size_t i = 0; i < m->n; i++) {
        if (pt == cur->pt) {
            prev->next = cur->next;
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }
    assert(0);
}

void free_mem(mc *m, void *pt)
{
    m_node *to_free = unregister_mem(m, pt);
    to_free->fun(to_free->pt);
    free(to_free);
}
