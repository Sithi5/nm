
#include "ft_nm.h"

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
    Elf64_Sym *symbols = (Elf64_Sym *)(nm->mapped_data + symtab_section->sh_offset);
    char *strings = (char *)(nm->mapped_data + strtab_section->sh_offset);

    for (size_t i = 0; i < symtab_section->sh_size / sizeof(Elf64_Sym); i++)
    {
        if (should_display_symbol((void *)&symbols[i], nm, ELFCLASS64))
        {
        }
    }
}