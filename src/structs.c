#include "ft_nm.h"

void set_nm_structure(t_nm *nm) {
    nm->args.a_flag = false;
    nm->args.g_flag = false;
    nm->args.u_flag = false;
    nm->args.r_flag = false;
    nm->args.p_flag = false;
    nm->file_count = 0;
    nm->fd = -1;
    ft_bzero(nm->args.files_names, sizeof(nm->args.files_names));
    nm->mapped_data_info = (struct stat){0};
    nm->mapped_data = NULL;
    nm->elf_data = (t_elf_data){0};
    nm->elf_data.elf_header.elf32 = NULL;
    nm->elf_data.elf_header.elf64 = NULL;
    nm->elf_data.section_headers.elf32 = NULL;
    nm->elf_data.section_headers.elf64 = NULL;
    nm->elf_data.symbols_tab_section.elf32 = NULL;
    nm->elf_data.symbols_tab_section.elf64 = NULL;
    nm->elf_data.symbols_strtab_section.elf32 = NULL;
    nm->elf_data.symbols_strtab_section.elf64 = NULL;
    nm->elf_data.sections_strtab_section.elf32 = NULL;
    nm->elf_data.sections_strtab_section.elf64 = NULL;
    nm->elf_data.symbols.elf32 = NULL;
    nm->elf_data.symbols.elf64 = NULL;
    nm->elf_data.current_symbol.elf32 = NULL;
    nm->elf_data.current_symbol.elf64 = NULL;
    nm->elf_data.current_symbol_name = NULL;
}