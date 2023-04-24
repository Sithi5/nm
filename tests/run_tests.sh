#!/bin/sh

# Tests configuration
VERBOSE=0

# Activate or deactivate tests
TEST_ALL=1
TEST_ELF64_SYMBOLS_COUNT=0
TEST_ELF32_SYMBOLS_COUNT=0
TEST_ELF64=1
TEST_ERRORS_MISC=0
TEST_ERRORS_FILE_TYPE=0

# Colors

_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_RED=`tput setaf 1`
_PURPLE=`tput setaf 5`
_END=`tput sgr0`

# echo filename
echo "\n\n\n\n${_PURPLE}$0 :${_END}\n"

compare_nm_and_ft_nm_symbols_count() {
    test_name=$1
    test_number=$2
    prog=$3
    options=$4

    nm_line_count=$(nm "$prog" $options | wc -l)
    ft_nm_line_count=$(./ft_nm "$prog" $options | wc -l)
    if [ "$nm_line_count" -eq "$ft_nm_line_count" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$VERBOSE" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_line_count"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_line_count"
            echo "${_YELLOW}--- diff:${_END}"
            echo "$(($nm_line_count - $ft_nm_line_count))"
        fi
    fi
}

compare_nm_and_ft_nm_output() {
    test_name=$1
    test_number=$2
    prog=$3
    options=$4

    nm_output=$(nm "$prog" $options)
    ft_nm_output=$(./ft_nm "$prog" $options)
    if [ "$nm_output" = "$ft_nm_output" ]
    then
        echo "${_GREEN}[$test_number] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[$test_number] ${test_name}: -> KO${_END}"
        if [ "$VERBOSE" -eq 1 ]; then
            echo "${_YELLOW}--- nm output:${_END}"
            echo "$nm_output"
            echo "$nm_output" > nm_output.txt
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_output"
            echo "$ft_nm_output" > ft_nm_output.txt
            echo "${_YELLOW}--- diff:${_END}"
            diff nm_output.txt ft_nm_output.txt
            rm nm_output.txt ft_nm_output.txt
        fi
    fi
}

compare_ft_nm_error_with_expected_output() {
    test_name=$1
    test_number=$2
    prog=$3
    expected_output=$4

    ft_nm_stderr=$(./ft_nm "$prog" 2>&1)
    if [ "$ft_nm_stderr" = "$expected_output" ]; then
        echo "${_GREEN}[${test_number}] ${test_name}: -> OK${_END}"
    else
        echo "${_RED}[${test_number}] ${test_name}: -> KO${_END}"
        if [ "$VERBOSE" -eq 1 ]; then
            echo "${_YELLOW}--- expected output:${_END}"
            echo "$expected_output"
            echo "${_YELLOW}--- ft_nm output:${_END}"
            echo "$ft_nm_stderr"
        fi
    fi
}


test_elf64_symbols_count()
{
    test_name="test_elf64_symbols_count without any option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count "simple binary file" $test_number  ./absolute_value ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./libft_malloc_aarch64_Linux.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file harder" $test_number ./libasan.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".o file" $test_number ./absolute_value.o ""

    test_name="test_elf64_symbols_count with -a option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./absolute_value "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./libft_malloc_aarch64_Linux.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file harder" $test_number ./libasan.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number ./absolute_value.o "-a"

    test_name="test_elf64_symbols_count with -g option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./absolute_value "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./libft_malloc_aarch64_Linux.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file harder" $test_number ./libasan.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number ./absolute_value.o "-g"

    test_name="test_elf64_symbols_count with -u option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./absolute_value "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./libft_malloc_aarch64_Linux.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file harder" $test_number ./libasan.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number ./absolute_value.o "-u"
}


test_elf32_symbols_count()
{
    test_name="test_elf32_symbols_count without any option"
	echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./absolute_value_32 ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./my_simple_lib_32.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".o file" $test_number ./absolute_value_32.o ""

    test_name="test_elf32_symbols_count with -a option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./absolute_value_32 "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./my_simple_lib_32.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number  ./absolute_value_32.o "-a"

    test_name="test_elf32_symbols_count with -g option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./absolute_value_32 "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./my_simple_lib_32.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number  ./absolute_value_32.o "-g"

    test_name="test_elf32_symbols_count with -u option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./absolute_value_32 "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./my_simple_lib_32.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number  ./absolute_value_32.o "-u"
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
        if [ "$VERBOSE" -eq 1 ]; then
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

    sub_test_name="test folders"
    prog=./foldertest
    expected_output="ft_nm: $prog: Failed to map file into memory"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"
    test_number=$((test_number + 1))

    sub_test_name="test not elf files (.c file)"
    prog=./absolute_value.c
    expected_output="ft_nm: $prog: Not an ELF file"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"
    test_number=$((test_number + 1))

    sub_test_name="test not elf files (.a file)"
    prog=./libft.a
    expected_output="ft_nm: $prog: Not an ELF file"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"
    test_number=$((test_number + 1))

    sub_test_name="test file doesn't exist"
    prog=./DoesNotExist
    expected_output="ft_nm: $prog: Failed to open file"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"
    test_number=$((test_number + 1))
}

test_elf64()
{
    test_name="test_elf64 without option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./absolute_value ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./my_simple_lib.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./libasan.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./absolute_value.o ""

    test_name="test_elf64 with -a option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./absolute_value "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./my_simple_lib.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./libasan.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./absolute_value.o "-a"

    test_name="test_elf64 with -g option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./absolute_value "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./my_simple_lib.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./libasan.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./absolute_value.o "-g"

    test_name="test_elf64 with -u option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./absolute_value "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./my_simple_lib.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./libasan.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./absolute_value.o "-u"

    test_name="test_elf64 with -r option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./absolute_value "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./my_simple_lib.so "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./libasan.so "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./absolute_value.o "-r"

    test_name="test_elf64 with -p option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./absolute_value "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./my_simple_lib.so "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./libasan.so "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./absolute_value.o "-p"
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
