
#include "ft_nm.h"

void swap_symbols64(Elf64_Sym *symbol1, Elf64_Sym *symbol2)
{
    Elf64_Sym temp;
    ft_memcpy(&temp, symbol1, sizeof(Elf64_Sym));
    ft_memcpy(symbol1, symbol2, sizeof(Elf64_Sym));
    ft_memcpy(symbol2, &temp, sizeof(Elf64_Sym));
}

void sort_symbols64(t_nm *nm)
{
    bool swapped;

    for (size_t i = 0; i < nm->elf64_data.symtab_entry_count - 1; i++)
    {
        swapped = false;
        for (size_t j = 0; j < nm->elf64_data.symtab_entry_count - 1; j++)
        {
            Elf64_Sym *symbol1 = nm->elf64_data.symbols + j;
            Elf64_Sym *symbol2 = nm->elf64_data.symbols + (j + 1);
            char *symbol1_name = (char *)(nm->mapped_data + nm->elf64_data.strtab_section->sh_offset + symbol1->st_name);
            char *symbol2_name = (char *)(nm->mapped_data + nm->elf64_data.strtab_section->sh_offset + symbol2->st_name);

            int comparison = ft_strcmp(symbol1_name, symbol2_name);
            if ((!nm->args.r_flag && comparison > 0) || (nm->args.r_flag && comparison < 0))
            {
                swap_symbols64(symbol1, symbol2);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

void process_elf64_file(t_nm *nm)
{
    nm->elf64_data.elf_header = (Elf64_Ehdr *)nm->mapped_data;
    nm->elf64_data.section_headers = (Elf64_Shdr *)(nm->mapped_data + nm->elf64_data.elf_header->e_shoff);
    // Find the symbol table and string table sections by iterating through the section headers
    for (int i = 0; i < nm->elf64_data.elf_header->e_shnum; i++)
    {
        if (nm->elf64_data.section_headers[i].sh_type == SHT_SYMTAB)
        {
            nm->elf64_data.symtab_section = &nm->elf64_data.section_headers[i];
            nm->elf64_data.strtab_section = &nm->elf64_data.section_headers[nm->elf64_data.section_headers[i].sh_link]; // The string table is linked to the symbol table
            break;
        }
    }

    if (!nm->elf64_data.symtab_section)
    {
        ft_dprintf(STDERR_FILENO, "%s: %s: no symbols\n", PROGRAM_NAME, nm->current_filename);
        return;
    }
    if (!nm->elf64_data.strtab_section)
    {
        ft_dprintf(STDERR_FILENO, "%s: %s: Failed to find string table\n", PROGRAM_NAME, nm->current_filename);
        return;
    }

    nm->elf64_data.symtab_size = nm->elf64_data.symtab_section->sh_size;
    nm->elf64_data.symtab_entry_size = sizeof(Elf64_Sym);
    nm->elf64_data.symtab_entry_count = nm->elf64_data.symtab_size / nm->elf64_data.symtab_entry_size;
    nm->elf64_data.symbols = (Elf64_Sym *)(nm->mapped_data + nm->elf64_data.symtab_section->sh_offset);

    sort_symbols64(nm);

    for (size_t i = 0; i < nm->elf64_data.symtab_entry_count; i++)
    {
        Elf64_Sym *symbol = &(nm->elf64_data.symbols)[i];
        char *symbol_name = (char *)(nm->mapped_data + nm->elf64_data.strtab_section->sh_offset + symbol->st_name);

        if (symbol_name == NULL || symbol_name[0] == '\0' || symbol_name[0] == '/')
        {
            continue;
        }

        if (should_display_symbol((void *)&(nm->elf64_data.symbols[i]), nm))
        {
            display_symbol(nm, symbol);
        }
    }
}