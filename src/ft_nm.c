#include "ft_nm.h"

int main(int argc, char *argv[])
{
    t_nm nm;

    ft_bzero(&nm, sizeof(t_nm));
    set_nm_structure(&nm);
    parse_args(argc, argv, &nm);
    display_args(&(nm.args));

    return 0;
}