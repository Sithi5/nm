#include "ft_nm.h"
bool should_display_symbol(const t_nm *nm) {
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

    // Skip certain symbol types
    if (type == STT_FILE || type == STT_SECTION || type == STT_TLS) {
        return false;
    }

    // If no flags are set, display all global and weak symbols, and local symbols except STT_NOTYPE
    // with SHN_UNDEF
    if (!nm->args.a_flag && !nm->args.g_flag && !nm->args.u_flag) {
        if (bind == STB_GLOBAL || bind == STB_WEAK ||
            (bind == STB_LOCAL && type != STT_NOTYPE && shndx != SHN_UNDEF)) {
            return true;
        }
    }

    // If -a flag is set, display all symbols
    if (nm->args.a_flag) {
        return true;
    }

    // If -g flag is set, display only global and weak symbols
    if (nm->args.g_flag && (bind == STB_GLOBAL || bind == STB_WEAK)) {
        return true;
    }

    // If -u flag is set, display only undefined symbols
    if (nm->args.u_flag && shndx == SHN_UNDEF) {
        return true;
    }

    return false;
}
static char get_symbol_char_from_type_and_bind(unsigned char type, unsigned char bind) {
    char symbol_char = '?';

    if (bind == STB_WEAK) {
        switch (type) {
        case STT_OBJECT:
            symbol_char = 'V';
            break;
        case STT_FUNC:
            symbol_char = 'W';
            break;
        case STT_SECTION:
            symbol_char = 'v';
            break;
        default:
            break;
        }
    } else {
        switch (type) {
        case STT_NOTYPE:
            symbol_char = 'n';
            break;
        case STT_OBJECT:
            symbol_char = 'D';
            break;
        case STT_FUNC:
            symbol_char = 'T';
            break;
        case STT_SECTION:
            symbol_char = 'S';
            break;
        case STT_FILE:
            symbol_char = 'F';
            break;
        case STT_COMMON:
            symbol_char = 'C';
            break;
        case STT_TLS:
            symbol_char = 'T';
            break;
        default:
            break;
        }
    }

    return symbol_char;
}

char get_current_symbol_type_char(t_nm *nm) {
    unsigned char bind;
    unsigned char type;
    uint16_t shndx;
    char symbol_char = '?';

    if (nm->elf_data.elf_class == ELFCLASS64) {
        bind = ELF64_ST_BIND(nm->elf_data.current_symbol.elf64->st_info);
        type = ELF64_ST_TYPE(nm->elf_data.current_symbol.elf64->st_info);
        shndx = nm->elf_data.current_symbol.elf64->st_shndx;
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        bind = ELF32_ST_BIND(nm->elf_data.current_symbol.elf32->st_info);
        type = ELF32_ST_TYPE(nm->elf_data.current_symbol.elf32->st_info);
        shndx = nm->elf_data.current_symbol.elf32->st_shndx;
    } else {
        return '?';
    }

    // Handle special cases for shndx
    if (shndx == SHN_UNDEF) {
        return (bind == STB_WEAK) ? 'w' : 'U';
    } else if (shndx == SHN_ABS) {
        return (bind == STB_WEAK) ? (isupper(type) ? 'W' : 'w') : 'A';
    } else if (shndx == SHN_COMMON) {
        return 'C';
    }

    // Use the original function to get the symbol character from type and bind
    symbol_char = get_symbol_char_from_type_and_bind(type, bind);

    return bind == STB_LOCAL ? ft_tolower(symbol_char) : symbol_char;
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