
#include "ft_nm.h"

void sort_symbols(t_nm *nm) {
    DEBUG ? ft_printf("DEBUG: sort_symbols()\n") : 0;
    bool swapped;
    int comparison = 0;

    for (size_t i = 0; i < nm->elf_data.symbols_tab_entry_count - 1; i++) {
        swapped = false;
        for (size_t j = 0; j < nm->elf_data.symbols_tab_entry_count - 1; j++) {
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