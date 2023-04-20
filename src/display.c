#include "ft_nm.h"

static void display_symbol32(t_nm *nm, Elf32_Sym *symbol)
{
    char symbol_type = get_symbol_type_char(nm, symbol);
    char *symbol_name = (char *)(nm->mapped_data + nm->elf32_data.strtab_section->sh_offset + symbol->st_name);
    if ((unsigned)symbol->st_value == 0)
        ft_printf("         %c %s\n", symbol_type, symbol_name);
    else
        ft_printf("%08x %c %s\n", (unsigned)symbol->st_value, symbol_type, symbol_name);
}

static void display_symbol64(t_nm *nm, Elf64_Sym *symbol)
{
    char symbol_type = get_symbol_type_char(nm, symbol);
    char *symbol_name = (char *)(nm->mapped_data + nm->elf64_data.strtab_section->sh_offset + symbol->st_name);

    if ((unsigned long)symbol->st_value == 0)
        ft_printf("                 %c %s\n", symbol_type, symbol_name);
    else
        ft_printf("%016lx %c %s\n", (unsigned long)symbol->st_value, symbol_type, symbol_name);
}

void display_symbol(t_nm *nm, void *symbol)
{
    if (nm->elf_class == ELFCLASS32)
        display_symbol32(nm, (Elf32_Sym *)symbol);
    else if (nm->elf_class == ELFCLASS64)
        display_symbol64(nm, (Elf64_Sym *)symbol);
}