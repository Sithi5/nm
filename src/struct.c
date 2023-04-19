#include "ft_nm.h"

void set_nm_structure(t_nm *nm)
{
    nm->args.a_flag = false;
    nm->args.g_flag = false;
    nm->args.u_flag = false;
    nm->args.r_flag = false;
    nm->args.p_flag = false;
    nm->file_count = 0;
    ft_bzero(nm->args.files_names, sizeof(nm->args.files_names));
}