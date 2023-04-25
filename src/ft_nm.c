#include "ft_nm.h"

void process_file(t_nm *nm) {
    nm->fd = open(nm->current_filename, O_RDONLY);
    if (nm->fd < 0) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Failed to open file\n", PROGRAM_NAME,
                   nm->current_filename);
        return;
    }

    if (fstat(nm->fd, &(nm->mapped_data_info)) < 0) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Failed to get file information\n", PROGRAM_NAME,
                   nm->current_filename);
        close(nm->fd);
        return;
    }

    nm->mapped_data =
        mmap(NULL, nm->mapped_data_info.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, nm->fd, 0);

    if (nm->mapped_data == MAP_FAILED) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Failed to map file into memory\n", PROGRAM_NAME,
                   nm->current_filename);
        close(nm->fd);
        return;
    }

    if (!is_valid_elf_header(nm))
        return;
    process_elf_file(nm);

    munmap(nm->mapped_data, nm->mapped_data_info.st_size);
    close(nm->fd);
}

int main(int argc, char *argv[]) {
    t_nm nm;

    ft_bzero(&nm, sizeof(t_nm));
    set_nm_structure(&nm);
    parse_args(argc, argv, &nm);

    for (int i = 0; i < nm.file_count; i++) {
        nm.current_file_index = i;
        nm.current_filename = nm.args.files_names[i];
        if (nm.file_count > 1)
            ft_printf("\n%s:\n", nm.current_filename);
        process_file(&nm);
    }

    return 0;
}