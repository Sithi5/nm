#include "ft_nm.h"

void display_current_symbol(t_nm *nm) {
    DEBUG ? ft_printf("DEBUG: display_current_symbol()\n") : 0;
    char symbol_type_char;
    uint64_t symbol_value;

    symbol_type_char = get_current_symbol_type_char(nm);
    if (nm->elf_data.elf_class == ELFCLASS64) {
        symbol_value = nm->elf_data.current_symbol.elf64->st_value;
        if (symbol_value == 0 &&
            (symbol_type_char == 'w' || symbol_type_char == 'U' || symbol_type_char == 'v')) {
            ft_printf("                 %c %s\n", symbol_type_char,
                      nm->elf_data.current_symbol_name);
        } else {
            ft_printf("%016lx %c %s\n", (unsigned long) symbol_value, symbol_type_char,
                      nm->elf_data.current_symbol_name);
        }
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        symbol_value = nm->elf_data.current_symbol.elf32->st_value;
        if (symbol_value == 0 &&
            (symbol_type_char == 'w' || symbol_type_char == 'U' || symbol_type_char == 'v')) {
            ft_printf("         %c %s\n", symbol_type_char, nm->elf_data.current_symbol_name);
        } else {
            ft_printf("%08x %c %s\n", (unsigned) symbol_value, symbol_type_char,
                      nm->elf_data.current_symbol_name);
        }
    }
}