
#include "ft_nm.h"

static void set_elf_data_header(t_nm *nm) {
    nm->elf_data.elf_class = nm->mapped_data[EI_CLASS];

    if (nm->elf_data.elf_class == ELFCLASS64) {
        nm->elf_data.elf_header.elf64 = (Elf64_Ehdr *) nm->mapped_data;
        nm->elf_data.section_headers.elf64 =
            (Elf64_Shdr *) (nm->mapped_data + nm->elf_data.elf_header.elf64->e_shoff);
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        nm->elf_data.elf_header.elf32 = (Elf32_Ehdr *) nm->mapped_data;
        nm->elf_data.section_headers.elf32 =
            (Elf32_Shdr *) (nm->mapped_data + nm->elf_data.elf_header.elf32->e_shoff);
    }
}

static int get_section_headers_count(t_nm *nm) {
    return nm->elf_data.elf_class == ELFCLASS64 ? nm->elf_data.elf_header.elf64->e_shnum
                                                : nm->elf_data.elf_header.elf32->e_shnum;
}

void find_symbol_and_string_table_sections(t_nm *nm) {
    size_t e_shnum;
    uint32_t sh_type;

    e_shnum = get_section_headers_count(nm);
    for (size_t i = 0; i < e_shnum; i++) {
        sh_type = nm->elf_data.elf_class == ELFCLASS64
                      ? nm->elf_data.section_headers.elf64[i].sh_type
                      : nm->elf_data.section_headers.elf32[i].sh_type;
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

        if (should_display_symbol(nm)) {
            DEBUG ? 0 : display_current_symbol(nm);
        }
    }
}

void process_elf_file(t_nm *nm) {
    DEBUG ? ft_printf("DEBUG: Processing file: %s\n", nm->current_filename) : 0;
    set_elf_data_header(nm);
    find_symbol_and_string_table_sections(nm);
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