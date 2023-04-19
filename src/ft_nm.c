#include "ft_nm.h"

void process_elf64_file(unsigned char *file_data, const char *filename)
{
    return;
}

void process_elf32_file(unsigned char *file_data, const char *filename)
{
    return;
}

void process_elf_file(unsigned char *file_data, const char *filename)
{
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)file_data;

    // Check if the file is 32-bit or 64-bit
    if (elf_header->e_ident[EI_CLASS] == ELFCLASS32)
    {
        // Process 32-bit ELF file
        process_elf32_file(file_data, filename);
    }
    else if (elf_header->e_ident[EI_CLASS] == ELFCLASS64)
    {
        // Process 64-bit ELF file
        process_elf64_file(file_data, filename);
    }
    else
    {
        ft_printf("Unknown ELF class: %s\n", filename);
    }
}

void process_file(const char *filename, t_nm *nm)
{
    nm->fd = open(filename, O_RDONLY);
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
        process_file(nm.args.files_names[i], &nm);
    }

    return 0;
}