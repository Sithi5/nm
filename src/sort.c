
#include "ft_nm.h"

static char *get_symbol_name_from_index(t_nm *nm, size_t index) {
    if (nm->elf_data.elf_class == ELFCLASS64) {
        return (char *) (nm->mapped_data + nm->elf_data.strtab_section.elf64->sh_offset +
                         nm->elf_data.symbols.elf64[index].st_name);
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        return (char *) (nm->mapped_data + nm->elf_data.strtab_section.elf32->sh_offset +
                         nm->elf_data.symbols.elf32[index].st_name);
    }
    return NULL;
}

void sort_symbols(t_nm *nm) {
    bool swapped;
    int comparison = 0;

    for (size_t i = 0; i < nm->elf_data.symtab_entry_count - 1; i++) {
        swapped = false;
        for (size_t j = 0; j < nm->elf_data.symtab_entry_count - 1; j++) {
            comparison =
                ft_strcmp(get_symbol_name_from_index(nm, j), get_symbol_name_from_index(nm, j + 1));

            if ((!nm->args.r_flag && comparison > 0) || (nm->args.r_flag && comparison < 0)) {
                swap_symbols_index(nm, j, j + 1);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}