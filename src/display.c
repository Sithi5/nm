#include "ft_nm.h"

void display_args(const t_args *args)
{
    printf("a_flag: %s\n", args->a_flag ? "true" : "false");
    printf("g_flag: %s\n", args->g_flag ? "true" : "false");
    printf("u_flag: %s\n", args->u_flag ? "true" : "false");
    printf("r_flag: %s\n", args->r_flag ? "true" : "false");
    printf("p_flag: %s\n", args->p_flag ? "true" : "false");

    printf("Files:\n");
    for (int i = 0; i < MAX_FILES && args->files_names[i] != NULL; i++)
    {
        printf("  %s\n", args->files_names[i]);
    }
}