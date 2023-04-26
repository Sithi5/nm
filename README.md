# nm

`nm` is a command-line utility that displays the symbol table of an ELF (Executable and Linkable Format) file. It provides information about the symbols used by a program, such as functions, variables, and other objects. This tool can be useful for debugging, understanding the inner workings of a program, or analyzing the performance of a compiled binary.

## Features

- Displays the symbol table of ELF files
- Supports various output formats for easy parsing and analysis
- Can filter symbols based on their type, binding, or visibility
- Allows for demangling of C++ symbol names

## Requirements

Unix-like operating system (tested on macOS and Ubuntu)
C compiler (tested with gcc)

## Installation

Follow the steps below to install the nm project:

1. Clone the repository:

```bash
git clone --recursive https://github.com/sithi5/nm.git
```

> "Note: The --recursive flag is necessary to initialize and fetch the submodule(s) within the project."

2. Change to the project directory:

```bash
cd ft_nm
```

3. Compile the project using the provided Makefile:

```bash
make
```

4. (Optional) Adjust the compilation flags in the Makefile if needed. The available options are:
   WALL: Enable all warning flags
   WEXTRA: Enable extra warning flags
   WSHADOW: Enable warning flags for shadowing
   WERROR: Treat warnings as errors
   FSANITIZE: Enable address sanitizer
   DEBUG: Enable debug mode
   O2: Enable level 2 optimization
   To enable or disable an option, set the corresponding variable to "yes" or "no" in the Makefile. For example, to enable level 2 optimization, set O2 = yes.

There is additional Makefile rules:

| rules           | Description                                                                                                                                                              |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| all             | This rule (default) compiles the project and generates the binary. It is run when you execute make without specifying a target.                                          |
| tests           | This rule runs the project's test suite. After the project has been built, execute make tests to launch the test scripts.                                                |
| clean           | This rule removes the generated object files and other temporary files created during compilation. Use make clean to clean up these files.                               |
| fclean          | This rule runs clean and also removes the generated binary. Use make fclean to completely clean up the project, including the binary.                                    |
| re              | This rule runs fclean followed by all, effectively rebuilding the project from scratch. Use make re if you want to recompile the entire project and submodule.           |
| re-no-submodule | This rule is similar to re, but it does not rebuild the submodule(s). Use make re-no-submodule if you want to recompile the project without rebuilding the submodule(s). |
| help            | This rule displays helpful information about the Makefile and its targets. Use make help to show a brief description of each rule.                                       |
| docker-build    | This rule builds a Docker image for the project. Use make docker-build to create a Docker image with the specified configuration in the Dockerfile.                      |
| docker-run      | This rule creates and runs a Docker container from the built image. Use make docker-run to start a Docker container with the project.                                    |
| docker-exec     | This rule allows you to execute commands inside the running Docker container. Use make docker-exec to open an interactive shell inside the container.                    |
| docker-start    | This rule starts an existing Docker container. Use make docker-start to restart a stopped container.                                                                     |

## Options

| Option               | Description                                                          |
| -------------------- | -------------------------------------------------------------------- |
| -a, --all            | Display all symbols, including those that are not externally visible |
| -d, --dynamic        | Display only dynamic symbols                                         |
| -g, --extern-only    | Display only external symbols                                        |
| -p, --no-sort        | Do not sort the symbols (default is to sort alphabetically)          |
| -r, --reverse-sort   | Reverse the order of the sort                                        |
| -u, --undefined-only | Display only undefined symbols                                       |
| --help               | Display help information and exit                                    |

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
