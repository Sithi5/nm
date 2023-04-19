#include "ft_nm.h"

bool should_display_symbol(void *sym_ptr, const t_nm *nm, int elf_class)
{
    if (elf_class == ELFCLASS32)
    {
        Elf32_Sym *sym = (Elf32_Sym *)sym_ptr;
    }
    else if (elf_class == ELFCLASS64)
    {
        Elf64_Sym *sym = (Elf64_Sym *)sym_ptr;
    }
    else
    {
        return false;
    }

    return false;
}