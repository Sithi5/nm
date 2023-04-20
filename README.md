# nm

## Features

## Requirements

Unix-like operating system (tested on macOS and Ubuntu)
C compiler (tested with gcc)

## Options

## Usefull definition

### Section header

The section header is a data structure in the ELF (Executable and Linkable Format) file format that describes the layout and contents of the sections in an object file or executable. A section is a named portion of an object file or executable that contains related data, such as code, data, symbols, and relocation information.

The section header table is an array of section headers, located in the ELF header, that describes the layout and contents of the sections in an object file or executable. Each entry in the section header table corresponds to a section in the file and provides information about the section, such as its name, type, size, and location.

Some common section types include:

- .text: contains the executable instructions of the program
- .data: contains initialized global and static variables
- .rodata: contains read-only data, such as string literals
- .bss: contains uninitialized global and static variables
- .symtab: contains the symbol table, which maps symbol names to their corresponding addresses in the program
- .rel.\*: contains relocation information for code or data that refers to symbols in other object files or libraries

Programs that work with ELF files, such as compilers, linkers, and debuggers, use the section header to locate and manipulate the sections in the file.

## Resources

### elf file

- [elf.h](https://code.woboq.org/linux/include/elf.h.html)
