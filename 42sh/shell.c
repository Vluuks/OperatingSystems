#include "ast.h"
#include "front.h"

void my_free_tree(void *pt)
{
    free_tree((node_t *)pt);
}



void initialize(void)
{
    // This code will be called once at startup
}

void run_command(node_t* node)
{
    // ...
    register_global_mem(node, &my_free_tree);
    // (for testing:)
    print_tree(node);

    // don't forget:
    free_from_global_mem(node);
}
