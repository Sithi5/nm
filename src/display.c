#include "ft_nm.h"

void display_current_symbol(t_nm *nm) {
    char symbol_type;
    char *symbol_name;
    uint64_t symbol_value;

    symbol_type = get_current_symbol_type_char(nm);
    if (nm->elf_data.elf_class == ELFCLASS64) {
        symbol_name = (char *) (nm->mapped_data + nm->elf_data.strtab_section.elf64->sh_offset +
                                nm->elf_data.current_symbol.elf64->st_name);
        symbol_value = nm->elf_data.current_symbol.elf64->st_value;
        if (symbol_value == 0) {
            ft_printf("                 %c %s\n", symbol_type, symbol_name);
        } else {
            ft_printf("%016lx %c %s\n", (unsigned long) symbol_value, symbol_type, symbol_name);
        }
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        symbol_name = (char *) (nm->mapped_data + nm->elf_data.strtab_section.elf32->sh_offset +
                                nm->elf_data.current_symbol.elf32->st_name);
        symbol_value = nm->elf_data.current_symbol.elf32->st_value;
        if (symbol_value == 0) {
            ft_printf("         %c %s\n", symbol_type, symbol_name);
        } else {
            ft_printf("%08x %c %s\n", (unsigned) symbol_value, symbol_type, symbol_name);
        }
    }
}