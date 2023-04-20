#include "ft_nm.h"

bool should_display_symbol(void *sym_ptr, const t_nm *nm)
{
    Elf32_Sym *sym32;
    Elf64_Sym *sym64;
    unsigned char bind;
    unsigned char type;
    uint16_t shndx;

    if (nm->elf_class == ELFCLASS32)
    {
        sym32 = (Elf32_Sym *)sym_ptr;
        bind = ELF32_ST_BIND(sym32->st_info);
        type = ELF32_ST_TYPE(sym32->st_info);
        shndx = sym32->st_shndx;
    }
    else if (nm->elf_class == ELFCLASS64)
    {
        sym64 = (Elf64_Sym *)sym_ptr;
        bind = ELF64_ST_BIND(sym64->st_info);
        type = ELF64_ST_TYPE(sym64->st_info);
        shndx = sym64->st_shndx;
    }
    else
    {
        return false;
    }

    // If no flags are set, display all global and local symbols
    if (!nm->args.a_flag && !nm->args.g_flag && !nm->args.u_flag)
    {
        if (bind == STB_GLOBAL || bind == STB_WEAK || (bind == STB_LOCAL && type != STT_NOTYPE && shndx != SHN_UNDEF))
        {
            return true;
        }
    }

    // If -a flag is set, display all symbols
    if (nm->args.a_flag)
    {
        return true;
    }

    // If -g flag is set, display all global symbols
    if (nm->args.g_flag && bind == STB_LOCAL)
    {
        return true;
    }

    // If -u flag is set, display all undefined symbols
    if (nm->args.u_flag && shndx == SHN_UNDEF)
    {
        return true;
    }

    return false;
}

static char get_symbol_type_char64(Elf64_Sym *symbol, Elf64_Shdr *section_headers)
{
    unsigned char bind = ELF64_ST_BIND(symbol->st_info);
    Elf64_Shdr *section_header = &section_headers[symbol->st_shndx];

    if (symbol->st_shndx == SHN_UNDEF)
        return 'U';
    if (symbol->st_shndx == SHN_ABS)
        return 'A';
    if (symbol->st_shndx == SHN_COMMON)
        return 'C';
    if (section_header->sh_type == SHT_NOBITS && section_header->sh_flags & SHF_ALLOC)
        return 'B';
    if (section_header->sh_type == SHT_PROGBITS && section_header->sh_flags & SHF_ALLOC)
    {
        if (section_header->sh_flags & SHF_EXECINSTR)
            return 'T';
        else
            return 'D';
    }

    return '?';
}

static char get_symbol_type_char32(Elf32_Sym *symbol, Elf32_Shdr *section_headers)
{
    unsigned char bind = ELF32_ST_BIND(symbol->st_info);
    unsigned char type = ELF32_ST_TYPE(symbol->st_info);
    Elf32_Shdr *section_header = &section_headers[symbol->st_shndx];

    if (symbol->st_shndx == SHN_UNDEF)
        return 'U';
    if (symbol->st_shndx == SHN_ABS)
        return 'A';
    if (symbol->st_shndx == SHN_COMMON)
        return 'C';
    if (section_header->sh_type == SHT_NOBITS && section_header->sh_flags & SHF_ALLOC)
        return 'B';
    if (section_header->sh_type == SHT_PROGBITS && section_header->sh_flags & SHF_ALLOC)
    {
        if (section_header->sh_flags & SHF_EXECINSTR)
            return 'T';
        else
            return 'D';
    }

    return '?';
}

char get_symbol_type_char(t_nm *nm, void *symbol, void *section_headers)
{
    if (nm->elf_class == ELFCLASS32)
        return get_symbol_type_char32(nm->elf32_data.symbols, nm->elf32_data.section_headers);
    else if (nm->elf_class == ELFCLASS64)
        return get_symbol_type_char64(nm->elf64_data.symbols, nm->elf64_data.section_headers);
    else
        return '?';
}