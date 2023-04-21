#ifndef NM_H
#define NM_H

/****************************************************************************/
/*                          INCLUDES                                        */
/****************************************************************************/

#include "../Libft/includes/libft.h"
#include <elf.h>        // ELF header
#include <fcntl.h>      // open
#include <sys/mman.h>   // mmap
#include <sys/stat.h>   // fstat

/****************************************************************************/
/*                          DEFINES                                         */
/****************************************************************************/

#define PROGRAM_NAME    "ft_nm"
#define PROGRAM_VERSION "1.0.0"
#define PROGRAM_AUTHOR  "mabouce"

#define MAX_FILES 256

/****************************************************************************/
/*                           ENUM                                           */
/****************************************************************************/

/* Error codes */
enum e_error {
    ERROR_TOO_MANY_INPUT_FILES,
    NB_OF_ERROR_CODES /* Always keep last */
};

typedef enum { ELF_CLASS_NONE = 0, ELF_CLASS_32 = 1, ELF_CLASS_64 = 2 } t_elf_class;

/****************************************************************************/
/*                          UNIONS                                          */
/****************************************************************************/

typedef union u_elf_header {
    Elf32_Ehdr *elf32;
    Elf64_Ehdr *elf64;
} t_elf_header;

typedef union u_section {
    Elf32_Shdr *elf32;
    Elf64_Shdr *elf64;
} t_section;

typedef union u_symbol {
    Elf32_Sym *elf32;
    Elf64_Sym *elf64;
} t_symbol;

/****************************************************************************/
/*                          STRUCTS                                         */
/****************************************************************************/

typedef struct s_args {
    bool a_flag;
    bool g_flag;
    bool u_flag;
    bool r_flag;
    bool p_flag;
    char *files_names[MAX_FILES];
} t_args;

typedef struct s_elf_data {
    t_elf_header elf_header;
    t_section section_headers;
    t_section symtab_section;
    t_section strtab_section;
    size_t symtab_size;
    size_t symtab_entry_size;
    size_t symtab_entry_count;
    t_symbol symbols;
    t_symbol current_symbol;
    char *current_symbol_name;
    t_elf_class elf_class;
} t_elf_data;

typedef struct s_nm {
    t_args args;
    int file_count;
    int current_file_index;
    char *current_filename;
    int fd;
    unsigned char *mapped_data;
    struct stat mapped_data_info;
    t_elf_data elf_data;
} t_nm;

/****************************************************************************/
/*                          GLOBALS                                         */
/****************************************************************************/

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

// display.c
void display_current_symbol(t_nm *nm);

// args.c
void parse_args(int argc, char *argv[], t_nm *nm);

// struct.c
void set_nm_structure(t_nm *nm);

// utils.c
bool is_elf_file(unsigned char *file_data);

// elf64.c
void process_elf_file(t_nm *nm);

// elf32.c
void process_elf32_file(t_nm *nm);

// elfutils.c
char get_current_symbol_type_char(t_nm *nm);
bool should_display_symbol(const t_nm *nm);
void swap_symbols_index(t_nm *nm, size_t symbol_index_1, size_t symbol_index_2);
char *get_symbol_name_in_strtab_section(t_nm *nm, t_symbol symbol);

// sort.c
void sort_symbols(t_nm *nm);

#endif