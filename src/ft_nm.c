#include "ft_nm.h"

void process_elf_file(t_nm *nm)
{
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)nm->mapped_data;

    if (elf_header->e_ident[EI_CLASS] == ELFCLASS32)
    {
        process_elf32_file(nm);
    }
    else if (elf_header->e_ident[EI_CLASS] == ELFCLASS64)
    {
        process_elf64_file(nm);
    }
    else
    {
        display_file_error("Unknown ELF class: ", nm);
    }
}

void process_file(t_nm *nm)
{
    nm->fd = open(nm->current_filename, O_RDONLY);
    if (nm->fd < 0)
    {
        display_file_error("Failed to open file: ", nm);
        return;
    }

    if (fstat(nm->fd, &(nm->mapped_data_info)) < 0)
    {
        display_file_error("Failed to get file information: ", nm);
        close(nm->fd);
        return;
    }

    nm->mapped_data = mmap(NULL, nm->mapped_data_info.st_size, PROT_READ, MAP_PRIVATE, nm->fd, 0);
    if (nm->mapped_data == MAP_FAILED)
    {
        display_file_error("Failed to map file into memory: ", nm);
        close(nm->fd);
        return;
    }

    if (!is_elf_file(nm->mapped_data))
    {
        display_file_error("Not an ELF file: ", nm);
        close(nm->fd);
        munmap(nm->mapped_data, nm->mapped_data_info.st_size);
        return;
    }

    process_elf_file(nm);

    // Clean up
    munmap(nm->mapped_data, nm->mapped_data_info.st_size);
    close(nm->fd);
}

int main(int argc, char *argv[])
{
    t_nm nm;

    ft_bzero(&nm, sizeof(t_nm));
    set_nm_structure(&nm);
    parse_args(argc, argv, &nm);

    for (int i = 0; i < nm.file_count; i++)
    {
        nm.current_file_index = i;
        nm.current_filename = nm.args.files_names[i];
        process_file(&nm);
    }

    return 0;
}