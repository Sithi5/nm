#ifndef NM_H
#define NM_H

/****************************************************************************/
/*                          INCLUDES                                        */
/****************************************************************************/

#include "../Libft/includes/libft.h"
#include <elf.h>      // ELF header
#include <sys/mman.h> // mmap
#include <fcntl.h>    // open
#include <sys/stat.h> // fstat

/****************************************************************************/
/*                          DEFINES                                         */
/****************************************************************************/

#define PROGRAM_NAME "ft_nm"
#define PROGRAM_VERSION "1.0.0"
#define PROGRAM_AUTHOR "mabouce"

#ifdef DEBUG
#define DEBUG true
#else
#define DEBUG false
#endif

#define MAX_FILES 256

/****************************************************************************/
/*                           ENUM                                           */
/****************************************************************************/

/* Error codes */
enum e_error
{
    ERROR_TOO_MANY_INPUT_FILES,
    NB_OF_ERROR_CODES /* Always keep last */
};

/****************************************************************************/
/*                          STRUCTS                                         */
/****************************************************************************/

typedef struct s_args
{
    bool a_flag;
    bool g_flag;
    bool u_flag;
    bool r_flag;
    bool p_flag;
    char *files_names[MAX_FILES];
} t_args;

typedef struct s_nm
{
    t_args args;
    int file_count;
    int current_file_index;
    char *current_filename;
    int fd;
    unsigned char *mapped_data;
    struct stat mapped_data_info;
} t_nm;

/****************************************************************************/
/*                          GLOBALS                                         */
/****************************************************************************/

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

// display.c
void display_args(const t_args *args);
void display_file_error(const char *error_message, t_nm *nm);

// args.c
void parse_args(int argc, char *argv[], t_nm *nm);

// struct.c
void set_nm_structure(t_nm *nm);

// utils.c
bool is_elf_file(unsigned char *file_data);

// errors.c
void exit_clean(int status, t_nm *nm);

// elf64.c
void process_elf64_file(t_nm *nm);

// elf32.c
void process_elf32_file(t_nm *nm);

#endif