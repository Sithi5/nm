#include "ft_nm.h"

char *get_symbol_name_from_index(t_nm *nm, size_t symbol_index) {
    size_t shndx;
    Elf64_Shdr *shdr64;
    Elf32_Shdr *shdr32;

    if (nm->elf_data.elf_class == ELFCLASS64) {
        if (ELF64_ST_TYPE(nm->elf_data.symbols.elf64[symbol_index].st_info == STT_SECTION)) {
            shndx = nm->elf_data.symbols.elf64[symbol_index].st_shndx;
            shdr64 = &nm->elf_data.section_headers.elf64[shndx];
            return (char *) (nm->mapped_data +
                             nm->elf_data.sections_strtab_section.elf64->sh_offset +
                             shdr64->sh_name);
        } else {
            return (char *) (nm->mapped_data +
                             nm->elf_data.symbols_strtab_section.elf64->sh_offset +
                             nm->elf_data.symbols.elf64[symbol_index].st_name);
        }
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        if (ELF32_ST_TYPE(nm->elf_data.symbols.elf32[symbol_index].st_info == STT_SECTION)) {
            shndx = nm->elf_data.symbols.elf32[symbol_index].st_shndx;
            shdr32 = &nm->elf_data.section_headers.elf32[shndx];
            return (char *) (nm->mapped_data +
                             nm->elf_data.sections_strtab_section.elf32->sh_offset +
                             shdr32->sh_name);
        } else {
            return (char *) (nm->mapped_data +
                             nm->elf_data.symbols_strtab_section.elf32->sh_offset +
                             nm->elf_data.symbols.elf64[symbol_index].st_name);
        }
    } else {
        return NULL;
    }
}

bool should_display_symbol(t_nm *nm) {
    unsigned char bind;
    unsigned char type;
    uint16_t shndx;
    Elf64_Shdr *section_header64 = NULL;
    Elf32_Shdr *section_header32 = NULL;

    if (nm->elf_data.elf_class == ELFCLASS64) {
        bind = ELF64_ST_BIND(nm->elf_data.current_symbol.elf64->st_info);
        type = ELF64_ST_TYPE(nm->elf_data.current_symbol.elf64->st_info);
        shndx = nm->elf_data.current_symbol.elf64->st_shndx;
        section_header64 = &nm->elf_data.section_headers.elf64[shndx];
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        bind = ELF32_ST_BIND(nm->elf_data.current_symbol.elf32->st_info);
        type = ELF32_ST_TYPE(nm->elf_data.current_symbol.elf32->st_info);
        shndx = nm->elf_data.current_symbol.elf32->st_shndx;
        section_header32 = &nm->elf_data.section_headers.elf32[shndx];
    } else {
        return false;
    }

    DEBUG ? ft_printf("DEBUG: symbol (name=%s, type=%u, bind=%u, shndx=%u)\t",
                      nm->elf_data.current_symbol_name, type, bind, shndx)
          : 0;

    // Skip compiler generated symbols
    if (ft_strncmp(nm->elf_data.current_symbol_name, "$", 1) == 0 ||
        (nm->elf_data.current_symbol_name[0] == '\0' && shndx == SHN_UNDEF)) {
        DEBUG ? ft_printf("SKIPPED\n") : 0;
        return false;
    }

    // If no flags are set, display all global and weak symbols, and local symbols except STT_NOTYPE
    // with SHN_UNDEF or in the .init_array section
    if (!nm->args.a_flag && !nm->args.g_flag && !nm->args.u_flag) {
        if (bind == STB_GLOBAL || bind == STB_WEAK ||
            (bind == STB_LOCAL && type != STT_NOTYPE && shndx == SHN_UNDEF)) {
            DEBUG ? ft_printf("TO DISPLAY\n") : 0;
            return true;
        }
    }

    // If -a flag is set, display all symbols
    if (nm->args.a_flag) {
        DEBUG ? ft_printf("TO DISPLAY\n") : 0;
        return true;
    }

    // If -g flag is set, display only global and weak symbols
    if (nm->args.g_flag && (bind == STB_GLOBAL || bind == STB_WEAK)) {
        DEBUG ? ft_printf("TO DISPLAY\n") : 0;
        return true;
    }

    // If -u flag is set, display only undefined symbols
    if (nm->args.u_flag && shndx == SHN_UNDEF) {
        DEBUG ? ft_printf("TO DISPLAY\n") : 0;
        return true;
    }

    DEBUG ? ft_printf("SKIPPED\n") : 0;
    return false;
}

char get_current_symbol_type_char(t_nm *nm) {
    unsigned char bind;
    unsigned char symbol_type;
    uint16_t shndx;
    char symbol_char;
    Elf64_Shdr *section_header64 = NULL;
    Elf32_Shdr *section_header32 = NULL;

    symbol_char = '?';
    if (nm->elf_data.elf_class == ELFCLASS64) {
        bind = ELF64_ST_BIND(nm->elf_data.current_symbol.elf64->st_info);
        symbol_type = ELF64_ST_TYPE(nm->elf_data.current_symbol.elf64->st_info);
        shndx = nm->elf_data.current_symbol.elf64->st_shndx;
        section_header64 = &nm->elf_data.section_headers.elf64[shndx];
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        bind = ELF32_ST_BIND(nm->elf_data.current_symbol.elf32->st_info);
        symbol_type = ELF32_ST_TYPE(nm->elf_data.current_symbol.elf32->st_info);
        shndx = nm->elf_data.current_symbol.elf32->st_shndx;
        section_header32 = &nm->elf_data.section_headers.elf32[shndx];
    }

    if (shndx == SHN_UNDEF) {
        return (bind == STB_WEAK) ? 'w' : 'U';
    } else if (shndx == SHN_ABS) {
        symbol_char = 'A';
    } else if (shndx == SHN_COMMON) {
        symbol_char = 'C';
    } else if (nm->elf_data.elf_class == ELFCLASS64 && section_header64) {
        switch (section_header64->sh_type) {
        case SHT_PROGBITS:
            if (section_header64->sh_flags & SHF_WRITE) {
                symbol_char = 'D';
            } else if (section_header64->sh_flags & SHF_EXECINSTR) {
                symbol_char = 'T';
            } else {
                symbol_char = 'R';
            }
            break;
        case SHT_NOBITS:
            if (section_header64->sh_flags & SHF_ALLOC && section_header64->sh_flags & SHF_WRITE) {
                symbol_char = 'B';
            }
            break;
        default:
            symbol_char = '?';
            break;
        }
    } else if (nm->elf_data.elf_class == ELFCLASS32 && section_header32) {
        switch (section_header32->sh_type) {
        case SHT_PROGBITS:
            if (section_header32->sh_flags & SHF_WRITE) {
                symbol_char = 'D';
            } else if (section_header32->sh_flags & SHF_EXECINSTR) {
                symbol_char = 'T';
            } else {
                symbol_char = 'R';
            }
            break;
        case SHT_NOBITS:
            if (section_header32->sh_flags & SHF_ALLOC && section_header32->sh_flags & SHF_WRITE) {
                symbol_char = 'B';
            }
            break;
        default:
            symbol_char = '?';
            break;
        }
    } else {
        symbol_char = '?';
    }

    symbol_char = (bind == STB_LOCAL) ? ft_tolower(symbol_char) : symbol_char;
    return symbol_char;
}

void swap_symbols_index(t_nm *nm, size_t symbol_index_1, size_t symbol_index_2) {
    Elf64_Sym temp64;
    Elf32_Sym temp32;

    if (nm->elf_data.elf_class == ELFCLASS64) {
        temp64 = nm->elf_data.symbols.elf64[symbol_index_1];
        nm->elf_data.symbols.elf64[symbol_index_1] = nm->elf_data.symbols.elf64[symbol_index_2];
        nm->elf_data.symbols.elf64[symbol_index_2] = temp64;
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        temp32 = nm->elf_data.symbols.elf32[symbol_index_1];
        nm->elf_data.symbols.elf32[symbol_index_1] = nm->elf_data.symbols.elf32[symbol_index_2];
        nm->elf_data.symbols.elf32[symbol_index_2] = temp32;
    }
}