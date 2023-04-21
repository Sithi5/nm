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

    // If no flags are set, display all global and local symbols
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

    // If -g flag is set, display all global symbols
    if (nm->args.g_flag && bind == STB_LOCAL) {
        return true;
    }

    // If -u flag is set, display all undefined symbols
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

static char get_symbol_char_from_section_header(t_section section_header, int elf_class) {
    char symbol_char = '?';

    if (elf_class == ELFCLASS64) {
        if (section_header.elf64->sh_type == SHT_NOBITS &&
            section_header.elf64->sh_flags & SHF_ALLOC)
            symbol_char = 'B';
        else if (section_header.elf64->sh_type == SHT_PROGBITS &&
                 section_header.elf64->sh_flags & SHF_ALLOC) {
            if (section_header.elf64->sh_flags & SHF_EXECINSTR)
                symbol_char = 'T';
            else if (section_header.elf64->sh_flags & SHF_WRITE)
                symbol_char = 'D';
            else
                symbol_char = 'R';
        } else if (section_header.elf64->sh_type == SHT_PROGBITS &&
                   section_header.elf64->sh_flags & SHF_WRITE)
            symbol_char = 'd';
    } else if (elf_class == ELFCLASS32) {
        if (section_header.elf32->sh_type == SHT_NOBITS &&
            section_header.elf32->sh_flags & SHF_ALLOC)
            symbol_char = 'B';
        else if (section_header.elf32->sh_type == SHT_PROGBITS &&
                 section_header.elf32->sh_flags & SHF_ALLOC) {
            if (section_header.elf32->sh_flags & SHF_EXECINSTR)
                symbol_char = 'T';
            else if (section_header.elf32->sh_flags & SHF_WRITE)
                symbol_char = 'D';
            else
                symbol_char = 'R';
        } else if (section_header.elf32->sh_type == SHT_PROGBITS &&
                   section_header.elf32->sh_flags & SHF_WRITE)
            symbol_char = 'd';
    }

    return symbol_char;
}

char get_current_symbol_type_char(t_nm *nm) {
    unsigned char bind;
    unsigned char type;
    t_section section_header;
    char symbol_char = '?';

    if (nm->elf_data.elf_class == ELFCLASS64) {
        section_header.elf64 =
            &(nm->elf_data.section_headers.elf64[nm->elf_data.current_symbol.elf64->st_shndx]);
        bind = ELF64_ST_BIND(nm->elf_data.current_symbol.elf64->st_info);
        type = ELF64_ST_TYPE(nm->elf_data.current_symbol.elf64->st_info);
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        section_header.elf32 =
            &(nm->elf_data.section_headers.elf32[nm->elf_data.current_symbol.elf32->st_shndx]);
        bind = ELF32_ST_BIND(nm->elf_data.current_symbol.elf32->st_info);
        type = ELF32_ST_TYPE(nm->elf_data.current_symbol.elf32->st_info);
    } else {
        return '?';
    }

    symbol_char = get_symbol_char_from_type_and_bind(type, bind);

    if (nm->elf_data.elf_class == ELFCLASS64) {
        if (nm->elf_data.current_symbol.elf64->st_shndx == SHN_UNDEF)
            symbol_char = 'U';
        else if (nm->elf_data.current_symbol.elf64->st_shndx == SHN_ABS)
            symbol_char = 'A';
        else
            symbol_char = get_symbol_char_from_section_header(section_header, ELFCLASS64);
    } else if (nm->elf_data.elf_class == ELFCLASS32) {
        if (nm->elf_data.current_symbol.elf32->st_shndx == SHN_UNDEF)
            symbol_char = 'U';
        else if (nm->elf_data.current_symbol.elf32->st_shndx == SHN_ABS)
            symbol_char = 'A';
        else
            symbol_char = get_symbol_char_from_section_header(section_header, ELFCLASS32);
    }

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