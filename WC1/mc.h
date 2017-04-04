#ifndef MC_H
#define MC_H
#include <unistd.h>

struct m_node;

typedef void (*free_fun)(void *pt);

typedef struct m_node {
    struct m_node *next;
    void *pt;
    free_fun fun;
} m_node;

typedef struct mc {
    m_node *header;
    size_t n;
} mc;

void register_mem(mc *m, void *pt, const free_fun fun);
void free_all_mem(mc *m);
void free_mem(mc *m, void *pt);
mc *init_mc();
m_node *unregister_mem(mc *m, const void *pt);

#endif
