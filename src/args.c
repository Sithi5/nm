#include "ft_nm.h"

void usage()
{
    printf("Usage: %s [options] <object files>\n\n", PROGRAM_NAME);
    exit(1);
}

void parse_args(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage();
    }
    for (int i = 1; i < argc; i++)
    {
    }
}