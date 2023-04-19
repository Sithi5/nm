#include "ft_nm.h"

static void ft_perror(int status, t_nm *nm)
{
    if (status == ERROR_TOO_MANY_INPUT_FILES)
    {
        ft_putstrerr("Too many files, maximum allowed is ");
        ft_putnbrerr(MAX_FILES);
        ft_putstrerr("\n");
    }
}

void exit_clean(int status, t_nm *nm)
{
    ft_perror(status, nm);
    if (nm->fd > 0)
    {
        close(nm->fd);
    }
    if (nm->mapped_data != NULL)
    {
        munmap(nm->mapped_data, nm->mapped_data_info.st_size);
    }
    exit(status);
}