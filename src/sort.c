
#include "ft_nm.h"

void bubble_sort_symbols(t_nm *nm) {
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

/**
 * Returns the index of the pivot element.
 * @param nm
 * @param low
 * @param high
 * @return
 */
int partition(t_nm *nm, int low, int high) {
    int pivot_index = low + (high - low) / 2;
    const char *pivot = get_symbol_name_from_index(nm, pivot_index);
    int i = low;
    int j = high;

    while (i <= j) {
        while (nm->args.r_flag ? ft_strcmp(get_symbol_name_from_index(nm, i), pivot) > 0
                               : ft_strcmp(get_symbol_name_from_index(nm, i), pivot) < 0) {
            i++;
        }

        while (nm->args.r_flag ? ft_strcmp(get_symbol_name_from_index(nm, j), pivot) < 0
                               : ft_strcmp(get_symbol_name_from_index(nm, j), pivot) > 0) {
            j--;
        }

        if (i <= j) {
            swap_symbols_index(nm, i, j);
            i++;
            j--;
        }
    }
    return i;
}

/**
 * Sorts the symbols table using the quick sort algorithm.
 * @param nm
 * @param low
 * @param high
 */
void quick_sort(t_nm *nm, int low, int high) {
    if (low < high) {
        int partition_index = partition(nm, low, high);
        quick_sort(nm, low, partition_index - 1);
        quick_sort(nm, partition_index, high);
    }
}

/**
 * Sorts the symbols table using the quick sort algorithm.
 * @param nm
 */
void quick_sort_symbols(t_nm *nm) {
    DEBUG ? ft_printf("DEBUG: quick_sort_symbols()\n") : 0;

    int symbol_count = nm->elf_data.symbols_tab_entry_count;
    if (symbol_count > 1) {
        quick_sort(nm, 0, symbol_count - 1);
    }
}