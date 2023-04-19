#include "ft_nm.h"

void display_args(const t_args *args)
{
    ft_printf("a_flag: %s\n", args->a_flag ? "true" : "false");
    ft_printf("g_flag: %s\n", args->g_flag ? "true" : "false");
    ft_printf("u_flag: %s\n", args->u_flag ? "true" : "false");
    ft_printf("r_flag: %s\n", args->r_flag ? "true" : "false");
    ft_printf("p_flag: %s\n", args->p_flag ? "true" : "false");

    ft_printf("Files:\n");
    for (int i = 0; i < MAX_FILES && args->files_names[i] != NULL; i++)
    {
        ft_printf("  %s\n", args->files_names[i]);
    }
}

void display_file_error(const char *error_message, t_nm *nm)
{
    ft_putstrerr(error_message);
    ft_putstrerr(nm->current_filename);
    ft_putchar('\n');
}