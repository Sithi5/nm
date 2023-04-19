#include "ft_nm.h"

static void ft_perror(int status, t_nm *nm)
{
    if (status == ERROR_FAILED_TO_OPEN_FILE)
    {
        ft_putstrerr("Failed to open file: ");
        ft_putstrerr(nm->args.files_names[nm->current_file_index]);
        ft_putstrerr("\n");
    }
    else if (status == ERROR_FAILED_TO_GET_FILE_INFO)
    {
        ft_putstrerr("Failed to get file information: ");
        ft_putstrerr(nm->args.files_names[nm->current_file_index]);
        ft_putstrerr("\n");
    }
    else if (status == ERROR_FAILED_TO_MAP_FILE)
    {
        ft_putstrerr("Failed to map file into memory: ");
        ft_putstrerr(nm->args.files_names[nm->current_file_index]);
        ft_putstrerr("\n");
    }
    else if (status == ERROR_TOO_MANY_INPUT_FILES)
    {
        ft_putstrerr("Too many files, maximum allowed is ");
        ft_putnbrerr(MAX_FILES);
        ft_putstrerr("\n");
    }
    else if (status == ERROR_NOT_AN_ELF_FILE)
    {
        ft_putstrerr("Not an ELF file: ");
        ft_putstrerr(nm->args.files_names[nm->current_file_index]);
        ft_putstrerr("\n");
    }
    if (errno != 0)
    {
        ft_putstrerr("errno: ");
        ft_putnbrerr(errno);
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