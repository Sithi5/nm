
#include "ft_nm.h"

static bool set_elf_data_header(t_nm *nm) {
    nm->elf_data.elf_class = nm->mapped_data[EI_CLASS];

    if (nm->elf_data.elf_class == ELFCLASS64) {
        nm->elf_data.elf_header.elf64 = (Elf64_Ehdr *) nm->mapped_data;
        if (nm->elf_data.elf_header.elf64->e_shoff > (size_t) nm->mapped_data_info.st_size) {
            ft_dprintf(STDERR_FILENO, "%s: %s: Invalid section header offset for elf64 file\n",
                       PROGRAM_NAME, nm->current_filename);
            return 0;
        }
        nm->elf_data.section_headers.elf64 =
            (Elf64_Shdr *) (nm->mapped_data + nm->elf_data.elf_header.elf64->e_shoff);
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        nm->elf_data.elf_header.elf32 = (Elf32_Ehdr *) nm->mapped_data;
        if (nm->elf_data.elf_header.elf32->e_shoff > nm->mapped_data_info.st_size) {
            ft_dprintf(STDERR_FILENO, "%s: %s: Invalid section header offset for elf32 file\n",
                       PROGRAM_NAME, nm->current_filename);
            return 0;
        }
        nm->elf_data.section_headers.elf32 =
            (Elf32_Shdr *) (nm->mapped_data + nm->elf_data.elf_header.elf32->e_shoff);
    }
    return 1;
}

bool find_symbol_and_string_table_sections(t_nm *nm) {
    size_t e_shnum;
    uint32_t sh_type;

    e_shnum = get_section_headers_count(nm);
    for (size_t i = 0; i < e_shnum; i++) {
        sh_type = nm->elf_data.elf_class == ELFCLASS64
                      ? nm->elf_data.section_headers.elf64[i].sh_type
                      : nm->elf_data.section_headers.elf32[i].sh_type;
        Elf64_Shdr *section_header = &(nm->elf_data.section_headers.elf64[i]);

        // Check sh_offset and sh_size
        if (section_header->sh_offset + section_header->sh_size >
            (size_t) nm->mapped_data_info.st_size) {
            ft_dprintf(STDERR_FILENO, "%s: %s: Invalid sh_offset or sh_size in section header\n",
                       PROGRAM_NAME, nm->current_filename);
            return false;
        }
        if (sh_type == SHT_SYMTAB) {
            if (nm->elf_data.elf_class == ELFCLASS64) {
                nm->elf_data.symtab_section.elf64 = &(nm->elf_data.section_headers.elf64[i]);
                nm->elf_data.symtab_strtab_section.elf64 =
                    &nm->elf_data.section_headers
                         .elf64[nm->elf_data.section_headers.elf64[i].sh_link];
            } else if (nm->elf_data.elf_class == ELFCLASS32) {
                nm->elf_data.symtab_section.elf32 = &(nm->elf_data.section_headers.elf32[i]);
                nm->elf_data.symtab_strtab_section.elf32 =
                    &nm->elf_data.section_headers
                         .elf32[nm->elf_data.section_headers.elf32[i].sh_link];
            }
        } else if (sh_type == SHT_STRTAB) {
            if (nm->elf_data.elf_class == ELFCLASS64 &&
                i == nm->elf_data.elf_header.elf64->e_shstrndx) {
                nm->elf_data.sh_strtab_section.elf64 = &(nm->elf_data.section_headers.elf64[i]);
            } else if (nm->elf_data.elf_class == ELFCLASS32 &&
                       i == nm->elf_data.elf_header.elf32->e_shstrndx) {
                nm->elf_data.sh_strtab_section.elf32 = &(nm->elf_data.section_headers.elf32[i]);
            }
        }
    }
    return true;
}

static void set_elf_data_symbols(t_nm *nm) {
    DEBUG ? ft_printf("DEBUG: Setting symbols\n") : 0;
    if (nm->elf_data.elf_class == ELFCLASS64) {
        nm->elf_data.symbols_tab_size = nm->elf_data.symtab_section.elf64->sh_size;
        nm->elf_data.symbols_tab_entry_size = sizeof(Elf64_Sym);
        nm->elf_data.symbols.elf64 =
            (Elf64_Sym *) (nm->mapped_data + nm->elf_data.symtab_section.elf64->sh_offset);
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        nm->elf_data.symbols_tab_size = nm->elf_data.symtab_section.elf32->sh_size;
        nm->elf_data.symbols_tab_entry_size = sizeof(Elf32_Sym);
        nm->elf_data.symbols.elf32 =
            (Elf32_Sym *) (nm->mapped_data + nm->elf_data.symtab_section.elf32->sh_offset);
    }
    nm->elf_data.symbols_tab_entry_count =
        nm->elf_data.symbols_tab_size / nm->elf_data.symbols_tab_entry_size;
}

static void process_symbols(t_nm *nm) {
    DEBUG ? ft_printf("DEBUG: Processing symbols\n") : 0;
    for (size_t i = 0; i < nm->elf_data.symbols_tab_entry_count; i++) {
        if (nm->elf_data.elf_class == ELFCLASS64) {
            nm->elf_data.current_symbol.elf64 = &(nm->elf_data.symbols.elf64[i]);

        } else if (nm->elf_data.elf_class == ELFCLASS32) {
            nm->elf_data.current_symbol.elf32 = &(nm->elf_data.symbols.elf32[i]);
        }

        nm->elf_data.current_symbol_name = get_symbol_name_from_index(nm, i);
        nm->elf_data.current_symbol_index = i;

        if (should_display_symbol(nm)) {
            DEBUG ? 0 : display_current_symbol(nm);
        }
    }
}

void process_elf_file(t_nm *nm) {
    DEBUG ? ft_printf("DEBUG: Processing file: %s\n", nm->current_filename) : 0;
    if (!set_elf_data_header(nm))
        return;
    if (!find_symbol_and_string_table_sections(nm))
        return;
    if ((nm->elf_data.elf_class == ELFCLASS64 && !(nm->elf_data.symtab_section.elf64)) ||
        (nm->elf_data.elf_class == ELFCLASS32 && !(nm->elf_data.symtab_section.elf32))) {
        ft_dprintf(STDERR_FILENO, "%s: %s: no symbols\n", PROGRAM_NAME, nm->current_filename);
        return;
    }
    if ((nm->elf_data.elf_class == ELFCLASS64 && !nm->elf_data.symtab_strtab_section.elf64) ||
        (nm->elf_data.elf_class == ELFCLASS32 && !nm->elf_data.symtab_strtab_section.elf32)) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Failed to find symbols string table\n", PROGRAM_NAME,
                   nm->current_filename);
        return;
    }
    if ((nm->elf_data.elf_class == ELFCLASS64 && !nm->elf_data.sh_strtab_section.elf64) ||
        (nm->elf_data.elf_class == ELFCLASS32 && !nm->elf_data.sh_strtab_section.elf32)) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Failed to find sections string table\n", PROGRAM_NAME,
                   nm->current_filename);
        return;
    }
    set_elf_data_symbols(nm);
    if (!nm->args.p_flag) {
        quick_sort_symbols(nm);
    }
    process_symbols(nm);
}