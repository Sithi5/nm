#include "ft_nm.h"

bool is_valid_elf_magic(t_nm *nm) {
    if (!(nm->mapped_data[0] == 0x7f && nm->mapped_data[1] == 'E' && nm->mapped_data[2] == 'L' &&
          nm->mapped_data[3] == 'F')) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Not an ELF file\n", PROGRAM_NAME, nm->current_filename);
        close(nm->fd);
        munmap(nm->mapped_data, nm->mapped_data_info.st_size);
        return false;
    }
    return true;
}

bool is_valid_elf_class(t_nm *nm) {
    unsigned char elf_class = nm->mapped_data[EI_CLASS];
    if (elf_class != ELFCLASS32 && elf_class != ELFCLASS64) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Unsupported ELF file class\n", PROGRAM_NAME,
                   nm->current_filename);
        return false;
    }
    return true;
}

bool is_valid_os_abi(t_nm *nm) {
    if ((nm->mapped_data[EI_OSABI] > ELFOSABI_OPENBSD &&
         nm->mapped_data[EI_OSABI] != ELFOSABI_ARM_AEABI &&
         nm->mapped_data[EI_OSABI] != ELFOSABI_ARM &&
         nm->mapped_data[EI_OSABI] != ELFOSABI_STANDALONE)) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Unsupported ELF file OS ABI\n", PROGRAM_NAME,
                   nm->current_filename);
        return false;
    }
    return true;
}

bool is_valid_ei_data(t_nm *nm) {
    if (nm->mapped_data[EI_DATA] != ELFDATA2LSB && nm->mapped_data[EI_DATA] != ELFDATA2MSB) {
        ft_dprintf(STDERR_FILENO, "%s: %s: Unsupported ELF file data encoding\n", PROGRAM_NAME,
                   nm->current_filename);
        return false;
    }
    return true;
}

bool wrong_elf_header(t_nm *nm) {
    close(nm->fd);
    munmap(nm->mapped_data, nm->mapped_data_info.st_size);
    return false;
}

bool is_valid_elf_header(t_nm *nm) {

    if (!is_valid_elf_magic(nm))
        return wrong_elf_header(nm);
    if (!is_valid_elf_class(nm))
        return wrong_elf_header(nm);
    if (!is_valid_ei_data(nm))
        return wrong_elf_header(nm);
    if (!is_valid_os_abi(nm))
        return wrong_elf_header(nm);

    return true;
}