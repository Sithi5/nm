#include "ft_nm.h"

static void usage() {
    ft_printf("Usage: %s [option(s)] [file(s)]\n"
              " List symbols in [file(s)] (a.out by default).\n"
              " The options are:\n"
              "  -a, --debug-syms       Display all symbols, even debugger-only symbols; normally "
              "these are not listed.\n"
              "  -g, --extern-only      Display only external symbols\n"
              "  -u, --undefined-only   Display only undefined symbols\n"
              "  -r, --reverse-sort     Reverse the sense of the sort\n"
              "  -p, --no-sort          Do not sort the symbols\n"
              "  -h, --help             Display this information\n",
              PROGRAM_NAME);
    exit(1);
}

static void add_file(char *filename, t_nm *nm) {
    if (nm->file_count >= MAX_FILES) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Too many files, maximum allowed is %d\n", PROGRAM_NAME,
                   nm->current_filename, MAX_FILES);
        exit(ERROR_TOO_MANY_INPUT_FILES);
    }
    nm->args.files_names[nm->file_count++] = filename;
}

void parse_args(int argc, char *argv[], t_nm *nm) {
    for (int i = 1; i < argc; i++) {
        if (ft_strcmp(argv[i], "-h") == 0 || ft_strcmp(argv[i], "--help") == 0) {
            usage();
        } else if (ft_strcmp(argv[i], "-a") == 0 || ft_strcmp(argv[i], "--debug-syms") == 0) {
            nm->args.a_flag = true;
        } else if (ft_strcmp(argv[i], "-g") == 0 || ft_strcmp(argv[i], "--extern-only") == 0) {
            nm->args.g_flag = true;
        } else if (ft_strcmp(argv[i], "-u") == 0 || ft_strcmp(argv[i], "--undefined-only") == 0) {
            nm->args.u_flag = true;
        } else if (ft_strcmp(argv[i], "-r") == 0 || ft_strcmp(argv[i], "--reverse-sort") == 0) {
            nm->args.r_flag = true;
        } else if (ft_strcmp(argv[i], "-p") == 0 || ft_strcmp(argv[i], "--no-sort") == 0) {
            nm->args.p_flag = true;
        } else {
            add_file(argv[i], nm);
        }
    }
    if (nm->file_count == 0) {
        add_file("a.out", nm);
    }
}