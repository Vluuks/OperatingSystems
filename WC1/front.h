#ifndef FRONT_H
#define FRONT_H
#include "mc.h"

extern mc *all_mem;

void register_global_mem(void *pt, const free_fun fun);
void free_from_global_mem(void *pt);
#endif
