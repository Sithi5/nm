#include "ft_nm.h"

void display_args(const t_args *args)
{
    ft_printf("a_flag: %s\n", args->a_flag ? "true" : "false");
    ft_printf("g_flag: %s\n", args->g_flag ? "true" : "false");
    ft_printf("u_flag: %s\n", args->u_flag ? "true" : "false");
    ft_printf("r_flag: %s\n", args->r_flag ? "true" : "false");
    ft_printf("p_flag: %s\n", args->p_flag ? "true" : "false");

    ft_printf("Files:\n");
    for (int i = 0; i < MAX_FILES && args->files_names[i] != NULL; i++)
    {
        ft_printf("  %s\n", args->files_names[i]);
    }
}

void display_file_error(const char *error_message, t_nm *nm)
{
    ft_putstrerr(error_message);
    ft_putstrerr(nm->current_filename);
    ft_putchar('\n');
}

static void display_symbol32(t_nm *nm, Elf64_Sym *symbol, char *strings_table)
{
    char symbol_type = get_symbol_type_char(nm, symbol, nm->elf32_data.section_headers);
    ft_printf("%08x %c %s\n", (unsigned)symbol->st_value, symbol_type, &strings_table[symbol->st_name]);
}

static void display_symbol64(t_nm *nm, Elf64_Sym *symbol, char *strings_table)
{
    char symbol_type = get_symbol_type_char(nm, symbol, nm->elf64_data.section_headers);
    ft_printf("%016lx %c %s\n", (unsigned long)symbol->st_value, symbol_type, &strings_table[symbol->st_name]);
}

void display_symbol(t_nm *nm, void *symbol, void *section_headers, char *strings_table)
{
    if (nm->elf_class == ELFCLASS32)
        display_symbol32(nm, (Elf32_Sym *)symbol, strings_table);
    else if (nm->elf_class == ELFCLASS64)
        display_symbol64(nm, (Elf64_Sym *)symbol, strings_table);
}