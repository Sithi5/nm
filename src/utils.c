#include "ft_nm.h"

bool is_elf_file(unsigned char *file_data)
{
    return file_data[0] == 0x7f &&
           file_data[1] == 'E' &&
           file_data[2] == 'L' &&
           file_data[3] == 'F';
}