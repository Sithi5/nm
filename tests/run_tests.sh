#!/bin/sh

TEST_ALL=0
TEST_ELF64_SYMBOLS_COUNT=1
TEST_ELF32_SYMBOLS_COUNT=1
TEST_ERRORS_MISC=0
TEST_ERRORS_FILE_TYPE=0
TEST_ELF64=0
TEST_DISPLAY_KO=0

_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_RED=`tput setaf 1`
_PURPLE=`tput setaf 5`
_END=`tput sgr0`


# echo filename
echo "\n\n\n\n${_PURPLE}$0 :${_END}\n"


test_elf64_symbols_count()
{
    test_name="test_elf64_symbols_count with -a option"
	echo "\n${_YELLOW}${test_name}:${_END}\n"

    test_number=1
    test_name="simple binary file"
    prog=./absolute_value
    nm_line_count=$(nm "$prog" -a | wc -l)
    ft_nm_line_count=$(./ft_nm "$prog" -a | wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi

    test_number=$((test_number + 1))
    test_name=".so file"
    prog=./libft_malloc_aarch64_Linux.so
    nm_line_count=$(nm "$prog" -a | wc -l)
    ft_nm_line_count=$(./ft_nm "$prog" -a | wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi

    test_number=$((test_number + 1))
    test_name=".o file"
    prog=./absolute_value.o
    nm_line_count=$(nm "$prog" -a | wc -l)
    ft_nm_line_count=$(./ft_nm "$prog" -a | wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi


    test_name="test_elf64_symbols_count without any option"
	echo "\n${_YELLOW}${test_name}:${_END}\n"


    test_number=1
    test_name="simple binary file"
    prog=./absolute_value
    nm_line_count=$(nm "$prog"| wc -l)
    ft_nm_line_count=$(./ft_nm "$prog"| wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi

    test_number=$((test_number + 1))
    test_name=".so file"
    prog=./libft_malloc_aarch64_Linux.so
    nm_line_count=$(nm "$prog"| wc -l)
    ft_nm_line_count=$(./ft_nm "$prog"| wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi

    test_number=$((test_number + 1))
    test_name=".o file"
    prog=./absolute_value.o
    nm_line_count=$(nm "$prog"| wc -l)
    ft_nm_line_count=$(./ft_nm "$prog"| wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi

}


test_elf32_symbols_count()
{
    test_name="test_elf32_symbols_count with -a option"
	echo "\n${_YELLOW}${test_name}:${_END}\n"

    test_number=1
    test_name="simple binary file"
    prog=./absolute_value_32
    nm_line_count=$(nm "$prog" -a | wc -l)
    ft_nm_line_count=$(./ft_nm "$prog" -a | wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi

    test_number=$((test_number + 1))
    test_name=".so file"
    prog=./libft_malloc_aarch64_Linux.so
    nm_line_count=$(nm "$prog" -a | wc -l)
    ft_nm_line_count=$(./ft_nm "$prog" -a | wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi

    test_number=$((test_number + 1))
    test_name=".o file"
    prog=./absolute_value_32.o
    nm_line_count=$(nm "$prog" -a | wc -l)
    ft_nm_line_count=$(./ft_nm "$prog" -a | wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi


    test_name="test_elf64_symbols_count without any option"
	echo "\n${_YELLOW}${test_name}:${_END}\n"


    test_number=1
    test_name="simple binary file"
    prog=./absolute_value_32
    nm_line_count=$(nm "$prog"| wc -l)
    ft_nm_line_count=$(./ft_nm "$prog"| wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi

    test_number=$((test_number + 1))
    test_name=".o file"
    prog=./absolute_value_32.o
    nm_line_count=$(nm "$prog"| wc -l)
    ft_nm_line_count=$(./ft_nm "$prog"| wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi
}


test_errors_misc()
{
    test_name="test_errors_misc"
    test_number=1
	echo "\n${_YELLOW}${test_name}:${_END}\n"

    test_name="test no symbols"

    prog=./no_symbols
    nm_stderr=$(nm "$prog" 2> nm_stderr.txt)
    ft_nm_stderr=$(./ft_nm "$prog" 2> ft_nm_stderr.txt)

    # Remove the first line (program name) from nm_stderr.txt and ft_nm_stderr.txt
    sed -i '1d' nm_stderr.txt
    sed -i '1d' ft_nm_stderr.txt
    if diff -q nm_stderr.txt ft_nm_stderr.txt >/dev/null; then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            cat nm_stderr.txt
            echo "${_YELLOW}--- ft_nm output:${_END}"
            cat ft_nm_stderr.txt
            echo "${_YELLOW}--- diff:${_END}"
            diff nm_stderr.txt ft_nm_stderr.txt
        fi
    fi
    test_number=$((test_number + 1))

}

test_errors_file_type()
{
    test_name="test_errors_file_type"
    test_number=1
	echo "\n${_YELLOW}${test_name}:${_END}\n"

    test_name="test folders"

    prog=./foldertest
    ft_nm_stderr=$(./ft_nm "$prog" 2>&1)
    expected_output="ft_nm: $prog: Failed to map file into memory"

    if [ "$ft_nm_stderr" = "$expected_output" ]; then
        echo "${_GREEN}${test_name}[${test_number}]: -> OK${_END}"
    else
        echo "${_RED}${test_name}[${test_number}]: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- expected output:${_END}"
            echo "$expected_output"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_stderr"
        fi
    fi
    test_number=$((test_number + 1))

   test_name="test not elf files (c file)"

    prog=./absolute_value.c
    ft_nm_stderr=$(./ft_nm "$prog" 2>&1)
    expected_output="ft_nm: $prog: Not an ELF file"

    if [ "$ft_nm_stderr" = "$expected_output" ]; then
        echo "${_GREEN}${test_name}[${test_number}]: -> OK${_END}"
    else
        echo "${_RED}${test_name}[${test_number}]: -> KO${_END}"
        if [ "$TEST_DISPLAY_KO" -eq 1 ]; then
            echo "${_YELLOW}--- expected output:${_END}"
            echo "$expected_output"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_stderr"
        fi
    fi
    test_number=$((test_number + 1))

}

test_elf64()
{
	echo "\n${_GREEN}test_elf64:${_END}\n"

    ./ft_nm ft_nm > ft_nm.out
    nm ft_nm > nm.out

    if diff -q ft_nm.out nm.out >/dev/null 2>&1
    then
        echo "${_GREEN}test_elf64 OK${_END}"
    else
        echo "${_RED}test_elf64 KO${_END}"
    fi
}

if [ $TEST_ELF64_SYMBOLS_COUNT -eq 1 ]|| [ "$TEST_ALL" -eq 1 ]
then
	test_elf64_symbols_count
fi

if [ $TEST_ELF32_SYMBOLS_COUNT -eq 1 ]|| [ "$TEST_ALL" -eq 1 ]
then
	test_elf32_symbols_count
fi

if [ $TEST_ELF64 -eq 1 ]|| [ "$TEST_ALL" -eq 1 ]
then
	test_elf64
fi

if [ $TEST_ERRORS_MISC -eq 1 ]|| [ "$TEST_ALL" -eq 1 ]
then
	test_errors_misc
fi

if [ "$TEST_ERRORS_FILE_TYPE" -eq 1 ] || [ "$TEST_ALL" -eq 1 ]
then
    test_errors_file_type
fi

echo
echo "That's all folks!"