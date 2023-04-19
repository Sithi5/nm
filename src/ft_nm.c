#include "ft_nm.h"

bool should_display_symbol(const Elf64_Sym *sym, const t_nm *nm)
{
    if (nm->args.a_flag)
    {
    }

    if (nm->args.g_flag)
    {
    }

    if (nm->args.u_flag)
    {
    }
    return false;
}

void process_elf32_file(t_nm *nm)
{
    ft_printf("Processing 32-bit ELF file: %s\n", nm->current_filename);
    return;
}

void process_elf64_file(t_nm *nm)
{
    // Read the 64-bit ELF header
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)nm->mapped_data;

    // Read section headers and find the symbol and string table sections
    Elf64_Shdr *section_headers = (Elf64_Shdr *)(nm->mapped_data + elf_header->e_shoff);
    Elf64_Shdr *symtab_section = NULL;
    Elf64_Shdr *strtab_section = NULL;

    for (int i = 0; i < elf_header->e_shnum; i++)
    {
        if (section_headers[i].sh_type == SHT_SYMTAB)
        {
            symtab_section = &section_headers[i];
            strtab_section = &section_headers[section_headers[i].sh_link];
            break;
        }
    }

    if (!symtab_section || !strtab_section)
    {
        display_file_error("Failed to find symbol table or string table in: ", nm);
        return;
    }

    // Read symbol entries and display the symbol information
    Elf64_Sym *symbols = (Elf64_Sym *)(file_data + symtab_section->sh_offset);
    char *strings = (char *)(file_data + strtab_section->sh_offset);

    for (size_t i = 0; i < symtab_section->sh_size / sizeof(Elf64_Sym); i++)
    {
        Elf64_Sym *sym = &symbols[i];
        char *sym_name = strings + sym->st_name;

        if (should_display_symbol(sym, nm))
        {
        }
    }
}

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