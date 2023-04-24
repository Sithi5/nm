# nm

## Features

## Requirements

Unix-like operating system (tested on macOS and Ubuntu)
C compiler (tested with gcc)

## Options

## Usefull definition

### ELF File Format

An ELF file consists of several sections, each with its own purpose:

- **Header:** The first section of an ELF file, containing information about the file as a whole.
- **Sections:** A collection of sections that contain the actual data and code of the program or library.
- **Program Headers:** Information that the system needs to load the program into memory and start running it.
- **Symbol Tables:** Tables that list the symbols used by the program, including functions, variables, and other objects.
- **Dynamic Linking Information:** Information used by the system to dynamically link the program with other libraries.

Each section and header has a specific format, with fields that contain different types of information. The exact format and contents of these sections can vary depending on the architecture and operating system for which the ELF file is intended.

| Section               | Description                                                                                              |
| --------------------- | -------------------------------------------------------------------------------------------------------- |
| Header                | Contains information about the file as a whole, including the machine architecture and entry point.      |
| Program Headers       | Defines segments of the program that need to be loaded into memory, along with their permissions.        |
| Section Headers       | Defines the attributes of each section in the file, such as name, size, and location.                    |
| Section Data          | Contains the actual data for each section, such as code, data, and symbol tables.                        |
| String Table          | Contains strings used throughout the ELF file, such as section names and symbol names.                   |
| Symbol Table          | Contains information about the symbols used by the program, including functions and variables.           |
| Dynamic Linking       | Contains information used by the dynamic linker to resolve symbols and load shared libraries.            |
| Relocation            | Contains information used to modify the program's code and data to adjust for the final memory location. |
| Debugging Information | Contains debugging symbols and other information used by debuggers and other tools.                      |

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
