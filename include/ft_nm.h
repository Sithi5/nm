#ifndef NM_H
#define NM_H

/****************************************************************************/
/*                          INCLUDES                                        */
/****************************************************************************/

#include "../Libft/includes/libft.h"

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
    ERROR_ARGS,
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
} t_nm;

/****************************************************************************/
/*                          GLOBALS                                         */
/****************************************************************************/

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

// display.c
void display_args(const t_args *args);

// args.c
void parse_args(int argc, char *argv[], t_nm *nm);

// struct.c
void set_nm_structure(t_nm *nm);

#endif