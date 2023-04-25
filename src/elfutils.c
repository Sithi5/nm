#include "ft_nm.h"

uint64_t get_symbol_address_from_index(t_nm *nm, int index) {
    uint64_t address;

    address = 0;
    if (nm->elf_data.elf_class == ELFCLASS64) {
        address = nm->elf_data.symbols.elf64[index].st_value;
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        address = nm->elf_data.symbols.elf32[index].st_value;
    }

    return address;   // return 0 for unknown ELF class
}

char *get_symbol_name_from_index(t_nm *nm, size_t symbol_index) {
    size_t shndx;
    Elf64_Shdr *shdr64;
    Elf32_Shdr *shdr32;

    if (nm->elf_data.elf_class == ELFCLASS64) {
        if (ELF64_ST_TYPE(nm->elf_data.symbols.elf64[symbol_index].st_info == STT_SECTION)) {
            shndx = nm->elf_data.symbols.elf64[symbol_index].st_shndx;
            shdr64 = &nm->elf_data.section_headers.elf64[shndx];
            return (char *) (nm->mapped_data + nm->elf_data.sh_strtab_section.elf64->sh_offset +
                             shdr64->sh_name);
        } else {
            return (char *) (nm->mapped_data + nm->elf_data.symtab_strtab_section.elf64->sh_offset +
                             nm->elf_data.symbols.elf64[symbol_index].st_name);
        }
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        if (ELF32_ST_TYPE(nm->elf_data.symbols.elf32[symbol_index].st_info == STT_SECTION)) {
            shndx = nm->elf_data.symbols.elf32[symbol_index].st_shndx;
            shdr32 = &nm->elf_data.section_headers.elf32[shndx];
            return (char *) (nm->mapped_data + nm->elf_data.sh_strtab_section.elf32->sh_offset +
                             shdr32->sh_name);
        } else {
            return (char *) (nm->mapped_data + nm->elf_data.symtab_strtab_section.elf32->sh_offset +
                             nm->elf_data.symbols.elf32[symbol_index].st_name);
        }
    } else {
        return NULL;
    }
}

bool should_display_symbol(t_nm *nm) {
    unsigned char bind;
    unsigned char type;
    uint16_t shndx;

    if (nm->elf_data.elf_class == ELFCLASS64) {
        bind = ELF64_ST_BIND(nm->elf_data.current_symbol.elf64->st_info);
        type = ELF64_ST_TYPE(nm->elf_data.current_symbol.elf64->st_info);
        shndx = nm->elf_data.current_symbol.elf64->st_shndx;
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        bind = ELF32_ST_BIND(nm->elf_data.current_symbol.elf32->st_info);
        type = ELF32_ST_TYPE(nm->elf_data.current_symbol.elf32->st_info);
        shndx = nm->elf_data.current_symbol.elf32->st_shndx;
    } else {
        return false;
    }

    DEBUG ? ft_printf("DEBUG: symbol (name=%s, type=%u, bind=%u, shndx=%u, symbol_type_char=%c, "
                      "symbol_addr=%li)\t",
                      nm->elf_data.current_symbol_name, type, bind, shndx,
                      get_current_symbol_type_char(nm),
                      get_symbol_address_from_index(nm, nm->elf_data.current_symbol_index))
          : 0;
    // Skip compiler generated symbols
    if (ft_strncmp(nm->elf_data.current_symbol_name, "$", 1) == 0 ||
        (nm->elf_data.current_symbol_name[0] == '\0' && shndx == SHN_UNDEF)) {
        DEBUG ? ft_printf("SKIPPED\n") : 0;
        return false;
    }

    // If -a flag is set, display all symbols except compiler generated symbols
    if (nm->args.a_flag) {
        DEBUG ? ft_printf("TO DISPLAY -a\n") : 0;
        return true;
    }

    // If no flags are set, display all global and weak symbols, and local symbols
    // except STT_NOTYPE, UNDEF and STT_SECTION
    if (!nm->args.a_flag && !nm->args.g_flag && !nm->args.u_flag) {
        if ((bind == STB_GLOBAL || bind == STB_WEAK ||
             (bind == STB_LOCAL && shndx != SHN_UNDEF && type != STT_FILE)) &&
            (type != STT_SECTION)) {
            DEBUG ? ft_printf("TO DISPLAY no flag\n") : 0;
            return true;
        }
    }

    // If -g flag is set, display only global and weak symbols
    if (nm->args.g_flag && (bind == STB_GLOBAL || bind == STB_WEAK)) {
        DEBUG ? ft_printf("TO DISPLAY -g\n") : 0;
        return true;
    }

    // If -u flag is set, display only undefined symbols
    if (nm->args.u_flag && shndx == SHN_UNDEF) {
        DEBUG ? ft_printf("TO DISPLAY -u\n") : 0;
        return true;
    }

    DEBUG
    ? ft_printf("SKIPPED\n") : 0;
    return false;
}

int get_section_headers_count(t_nm *nm) {
    return nm->elf_data.elf_class == ELFCLASS64 ? nm->elf_data.elf_header.elf64->e_shnum
                                                : nm->elf_data.elf_header.elf32->e_shnum;
}

char get_current_symbol_type_char(t_nm *nm) {
    unsigned char bind;
    unsigned char symbol_type;
    uint16_t shndx;
    char symbol_char;
    Elf64_Shdr *section_header = NULL;

    symbol_char = '?';
    if (nm->elf_data.elf_class == ELFCLASS64) {
        bind = ELF64_ST_BIND(nm->elf_data.current_symbol.elf64->st_info);
        symbol_type = ELF64_ST_TYPE(nm->elf_data.current_symbol.elf64->st_info);
        shndx = nm->elf_data.current_symbol.elf64->st_shndx;
        section_header = &nm->elf_data.section_headers.elf64[shndx];
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        bind = ELF32_ST_BIND(nm->elf_data.current_symbol.elf32->st_info);
        symbol_type = ELF32_ST_TYPE(nm->elf_data.current_symbol.elf32->st_info);
        shndx = nm->elf_data.current_symbol.elf32->st_shndx;
        section_header = (Elf64_Shdr *) &nm->elf_data.section_headers.elf32[shndx];
    }

    if (shndx == SHN_UNDEF) {
        return (bind == STB_WEAK) ? symbol_type == STT_OBJECT ? 'v' : 'w'
                                  : 'U';   // Weak or strong undefined symbol
    } else if (shndx == SHN_ABS) {         // Absolute symbol
        symbol_char = 'A';
    } else if (shndx == SHN_COMMON) {   // Common symbol
        symbol_char = 'C';
    } else if (ft_strncmp(nm->elf_data.current_symbol_name, ".debug", 6) == 0) {
        return 'N';
    } else {
        if (section_header) {
            switch (section_header->sh_type) {
            case SHT_PROGBITS:
                if (section_header->sh_flags & SHF_WRITE) {
                    symbol_char = 'D';
                } else if (section_header->sh_flags & SHF_EXECINSTR) {
                    symbol_char = 'T';
                } else if (section_header->sh_flags & SHF_ALLOC) {
                    symbol_char = 'R';
                } else {
                    symbol_char = 'n';
                }
                break;
            case SHT_INIT_ARRAY:   // Part of initialized data section
            case SHT_FINI_ARRAY:
            case SHT_DYNAMIC:
            case SHT_PREINIT_ARRAY:
                symbol_char = 'D';
                break;
            case SHT_NOBITS:
                if (section_header->sh_flags & SHF_ALLOC && section_header->sh_flags & SHF_WRITE) {
                    symbol_char = 'B';
                }
                break;
            default:
                if (section_header->sh_flags & SHF_ALLOC &&
                    !(section_header->sh_flags & SHF_WRITE) &&
                    !(section_header->sh_flags & SHF_EXECINSTR)) {
                    symbol_char = 'R';
                } else {
                    symbol_char = 'n';
                }
                break;
            }
        }
    }

    if (bind == STB_WEAK) {

        if (symbol_type == STT_OBJECT) {
            symbol_char = 'V';   // Weak symbol with a weak object specified
        } else {
            symbol_char = 'W';   // Weak symbol not specifically tagged as a weak object
        }
    }
    if (bind == STB_LOCAL) {
        symbol_char = ft_tolower(symbol_char);
    }

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