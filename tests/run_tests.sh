#!/bin/sh

# Tests configuration
VERBOSE=1

# Activate or deactivate tests
TEST_ALL=1
TEST_ELF64_SYMBOLS_COUNT=0
TEST_ELF32_SYMBOLS_COUNT=0
TEST_ELF64=0
TEST_ELF32=0
TEST_ERRORS_MISC=0
TEST_ERRORS_FILE_TYPE=0
TEST_ERRORS_FILE_CORRUPTED=1
TEST_TMP=1

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

test_tmp()
{
    test_name="TEST_TMP"
    echo "\n\n${_YELLOW}${test_name}:${_END}"
    test_number=1
}

test_elf64_symbols_count()
{
    test_name="TEST_ELF64_SYMBOLS_COUNT"
    echo "\n\n${_YELLOW}${test_name}:${_END}"

    test_name="test_elf64_symbols_count without any option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count "simple binary file" $test_number  ./bin/absolute_value ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./lib/libft_malloc_aarch64_Linux.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file harder" $test_number ./lib/libasan.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".o file" $test_number ./obj/absolute_value.o ""

    test_name="test_elf64_symbols_count with -a option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./bin/absolute_value "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./lib/libft_malloc_aarch64_Linux.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file harder" $test_number ./lib/libasan.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number ./obj/absolute_value.o "-a"

    test_name="test_elf64_symbols_count with -g option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./bin/absolute_value "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./lib/libft_malloc_aarch64_Linux.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file harder" $test_number ./lib/libasan.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number ./obj/absolute_value.o "-g"

    test_name="test_elf64_symbols_count with -u option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./bin/absolute_value "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./lib/libft_malloc_aarch64_Linux.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file harder" $test_number ./lib/libasan.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number ./obj/absolute_value.o "-u"
}


test_elf32_symbols_count()
{
    test_name="TEST_ELF32_SYMBOLS_COUNT"
    echo "\n\n${_YELLOW}${test_name}:${_END}"

    test_name="test_elf32_symbols_count without any option"
	echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./bin/absolute_value_32 ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./lib/my_simple_lib_32.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".o file" $test_number ./obj/absolute_value_32.o ""

    test_name="test_elf32_symbols_count with -a option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./bin/absolute_value_32 "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./lib/my_simple_lib_32.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number  ./obj/absolute_value_32.o "-a"

    test_name="test_elf32_symbols_count with -g option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./bin/absolute_value_32 "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./lib/my_simple_lib_32.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number  ./obj/absolute_value_32.o "-g"

    test_name="test_elf32_symbols_count with -u option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_symbols_count  "simple binary file" $test_number ./bin/absolute_value_32 "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count  ".so file" $test_number ./lib/my_simple_lib_32.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_symbols_count ".o file" $test_number  ./obj/absolute_value_32.o "-u"
}


test_errors_misc()
{
    test_name="TEST_ERRORS_MISC"
    echo "\n\n${_YELLOW}${test_name}:${_END}"

    test_number=1
    test_name="test no symbols"
    prog=./bin/no_symbols
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

    test_name="test stripped files"
    echo "\n\n${_YELLOW}${test_name}:${_END}"
    prog=./bin/ls
    expected_output="ft_nm: $prog: no symbols"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"
    test_number=$((test_number + 1))

}

test_errors_file_type()
{
    test_name="TEST_ERRORS_FILE_TYPE"
    echo "\n\n${_YELLOW}${test_name}:${_END}"

    test_number=1
    sub_test_name="test folders"
    prog=./foldertest
    expected_output="ft_nm: $prog: Failed to map file into memory"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"
    test_number=$((test_number + 1))

    sub_test_name="test not elf files (.c file)"
    prog=./src/absolute_value.c
    expected_output="ft_nm: $prog: Not an ELF file"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"
    test_number=$((test_number + 1))

    sub_test_name="test not elf files (.a file)"
    prog=./lib/libft.a
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
    test_name="TEST_ELF64"
    echo "\n\n${_YELLOW}${test_name}:${_END}"

    test_name="test_elf64 without option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./bin/absolute_value ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./lib/libasan.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./obj/absolute_value.o ""

    test_name="test_elf64 with -a option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "absolute_value" $test_number  ./bin/absolute_value "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin1_x64" $test_number  ./bin/bin1_x64 "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin2_x64" $test_number  ./bin/bin2_x64 "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin3_x64" $test_number  ./bin/bin3_x64 "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin4_x64" $test_number  ./bin/bin4_x64 "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin5_x64" $test_number  ./bin/bin5_x64 "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./lib/libasan.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "lib1_x64" $test_number  ./lib/lib1_x64.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "libft_malloc_aarch64_Linux" $test_number  ./lib/libft_malloc_aarch64_Linux.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "absolute_value.o" $test_number ./obj/absolute_value.o "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj1_x64" $test_number ./obj/obj1_x64.o "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj2_x64" $test_number ./obj/obj2_x64.o "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj3_x64" $test_number ./obj/obj3_x64.o "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj4_x64" $test_number ./obj/obj4_x64.o "-a"
    test_number=$((test_number + 1))

    test_name="test_elf64 with -g option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "absolute_value" $test_number  ./bin/absolute_value "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin1_x64" $test_number  ./bin/bin1_x64 "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin2_x64" $test_number  ./bin/bin2_x64 "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin3_x64" $test_number  ./bin/bin3_x64 "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin4_x64" $test_number  ./bin/bin4_x64 "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin5_x64" $test_number  ./bin/bin5_x64 "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./lib/libasan.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "lib1_x64" $test_number  ./lib/lib1_x64.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "libft_malloc_aarch64_Linux" $test_number  ./lib/libft_malloc_aarch64_Linux.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "absolute_value.o" $test_number ./obj/absolute_value.o "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj1_x64" $test_number ./obj/obj1_x64.o "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj2_x64" $test_number ./obj/obj2_x64.o "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj3_x64" $test_number ./obj/obj3_x64.o "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj4_x64" $test_number ./obj/obj4_x64.o "-g"
    test_number=$((test_number + 1))

    test_name="test_elf64 with -u option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "absolute_value" $test_number  ./bin/absolute_value "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin1_x64" $test_number  ./bin/bin1_x64 "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin2_x64" $test_number  ./bin/bin2_x64 "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin3_x64" $test_number  ./bin/bin3_x64 "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin4_x64" $test_number  ./bin/bin4_x64 "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin5_x64" $test_number  ./bin/bin5_x64 "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./lib/libasan.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "lib1_x64" $test_number  ./lib/lib1_x64.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "libft_malloc_aarch64_Linux" $test_number  ./lib/libft_malloc_aarch64_Linux.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "absolute_value.o" $test_number ./obj/absolute_value.o "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj1_x64" $test_number ./obj/obj1_x64.o "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj2_x64" $test_number ./obj/obj2_x64.o "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj3_x64" $test_number ./obj/obj3_x64.o "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj4_x64" $test_number ./obj/obj4_x64.o "-u"
    test_number=$((test_number + 1))

    test_name="test_elf64 with -p option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "absolute_value" $test_number  ./bin/absolute_value "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin1_x64" $test_number  ./bin/bin1_x64 "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin2_x64" $test_number  ./bin/bin2_x64 "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin3_x64" $test_number  ./bin/bin3_x64 "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin4_x64" $test_number  ./bin/bin4_x64 "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin5_x64" $test_number  ./bin/bin5_x64 "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib.so "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./lib/libasan.so "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "lib1_x64" $test_number  ./lib/lib1_x64.so "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "libft_malloc_aarch64_Linux" $test_number  ./lib/libft_malloc_aarch64_Linux.so "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "absolute_value.o" $test_number ./obj/absolute_value.o "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj1_x64" $test_number ./obj/obj1_x64.o "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj2_x64" $test_number ./obj/obj2_x64.o "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj3_x64" $test_number ./obj/obj3_x64.o "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj4_x64" $test_number ./obj/obj4_x64.o "-p"
    test_number=$((test_number + 1))

    test_name="test_elf64 with -r option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "absolute_value" $test_number  ./bin/absolute_value "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin1_x64" $test_number  ./bin/bin1_x64 "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin2_x64" $test_number  ./bin/bin2_x64 "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin3_x64" $test_number  ./bin/bin3_x64 "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin4_x64" $test_number  ./bin/bin4_x64 "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin5_x64" $test_number  ./bin/bin5_x64 "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib.so "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file harder" $test_number ./lib/libasan.so "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "lib1_x64" $test_number  ./lib/lib1_x64.so "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "libft_malloc_aarch64_Linux" $test_number  ./lib/libft_malloc_aarch64_Linux.so "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "absolute_value.o" $test_number ./obj/absolute_value.o "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj1_x64" $test_number ./obj/obj1_x64.o "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj2_x64" $test_number ./obj/obj2_x64.o "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj3_x64" $test_number ./obj/obj3_x64.o "-r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj4_x64" $test_number ./obj/obj4_x64.o "-r"
    test_number=$((test_number + 1))
}


test_elf32()
{
    test_name="TEST_ELF32"
    echo "\n\n${_YELLOW}${test_name}:${_END}"

    test_name="test_elf32 without option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./bin/absolute_value_32 ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib_32.so ""
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./obj/absolute_value_32.o ""

    test_name="test_elf32 with -a option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./bin/absolute_value_32 "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin2_x32" $test_number  ./bin/bin2_x32 "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "lib1_x32" $test_number  ./lib/lib1_x32.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib_32.so "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "absolute_value_32.o" $test_number ./obj/absolute_value_32.o "-a"
    compare_nm_and_ft_nm_output  "obj1_x32" $test_number ./obj/obj1_x32.o "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj2_x32" $test_number ./obj/obj2_x32.o "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj3_x32" $test_number ./obj/obj3_x32.o "-a"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj4_x32" $test_number ./obj/obj4_x32.o "-a"
    test_number=$((test_number + 1))

    test_name="test_elf32 with -g option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./bin/absolute_value_32 "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib_32.so "-g"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./obj/absolute_value_32.o "-g"

    test_name="test_elf32 with -u option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./bin/absolute_value_32 "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib_32.so "-u"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./obj/absolute_value_32.o "-u"

    test_name="test_elf32 with -p option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./bin/absolute_value_32 "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib_32.so "-p"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".o file" $test_number ./obj/absolute_value_32.o "-p"

    test_name="test_elf32 with -a and -r option"
    echo "\n${_YELLOW}${test_name}:${_END}\n"
    test_number=1
    compare_nm_and_ft_nm_output "simple binary file" $test_number  ./bin/absolute_value_32 "-a -r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "bin2_x32" $test_number  ./bin/bin2_x32 "-a -r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output "lib1_x32" $test_number  ./lib/lib1_x32.so "-a -r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  ".so file" $test_number ./lib/my_simple_lib_32.so "-a -r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "absolute_value_32.o" $test_number ./obj/absolute_value_32.o "-a -r"
    compare_nm_and_ft_nm_output  "obj1_x32" $test_number ./obj/obj1_x32.o "-a -r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj2_x32" $test_number ./obj/obj2_x32.o "-a -r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj3_x32" $test_number ./obj/obj3_x32.o "-a -r"
    test_number=$((test_number + 1))
    compare_nm_and_ft_nm_output  "obj4_x32" $test_number ./obj/obj4_x32.o "-a -r"
    test_number=$((test_number + 1))
}


test_errors_file_corrupted()
{
    test_name="TEST_ERRORS_FILE_CORRUPTED"
    echo "\n\n${_YELLOW}${test_name}:${_END}"

    test_name="easy header tests"
    echo "\n\n${_YELLOW}${test_name}:${_END}"

    test_number=1
    sub_test_name="test obj1_identabi_corrupted_x64"
    prog=./obj/obj1_identabi_corrupted_x64.o
    compare_nm_and_ft_nm_output "test obj1_identabi_corrupted_x64" $test_number "./obj/obj1_identabi_corrupted_x64.o" "-g"

    test_number=$((test_number + 1))
    sub_test_name="obj1_identclass_corrupted_x64"
    prog=./obj/obj1_identclass_corrupted_x64.o
    expected_output="ft_nm: $prog: Unsupported ELF file class"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj1_identclass2_corrupted_x64"
    prog=./obj/obj1_identclass2_corrupted_x64.o
    expected_output="ft_nm: $prog: Unsupported ELF file class"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj1_identclass3_x32_corrupted_x64"
    prog=./obj/obj1_identclass3_x32_corrupted_x64.o
    expected_output="ft_nm: $prog: no symbols"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj1_identdata_corrupted_x64"
    prog=./obj/obj1_identdata_corrupted_x64.o
    expected_output="ft_nm: $prog: Unsupported ELF file data encoding"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj1_identelf_corrupted_x64"
    prog=./obj/obj1_identelf_corrupted_x64.o
    expected_output="ft_nm: $prog: Not an ELF file"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj1_identelf2_corrupted_x64"
    prog=./obj/obj1_identelf2_corrupted_x64.o
    expected_output="ft_nm: $prog: Not an ELF file"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj1_identosabi_corrupted_x64"
    prog=./obj/obj1_identosabi_corrupted_x64.o
    expected_output="ft_nm: $prog: Unsupported ELF file OS ABI"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj1_identpad_corrupted_x64"
    prog=./obj/obj1_identpad_corrupted_x64.o
    compare_nm_and_ft_nm_output "test obj1_identabi_corrupted_x64" $test_number "./obj/obj1_identabi_corrupted_x64.o" "-g"

    test_number=$((test_number + 1))
    sub_test_name="obj1_identversion_corrupted_x64"
    prog=./obj/obj1_identversion_corrupted_x64.o
    expected_output="ft_nm: $prog: Unsupported ELF file version"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_name="harder section corrupted tests"
    echo "\n\n${_YELLOW}${test_name}:${_END}"

    test_number=$((test_number + 1))
    sub_test_name="obj2_eshoff_corrupted_x64"
    prog=./obj/obj2_eshoff_corrupted_x64.o
    expected_output="ft_nm: $prog: Invalid section header offset for elf64 file"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj2_eshoff2_corrupted_x64"
    prog=./obj/obj2_eshoff2_corrupted_x64.o
    expected_output="ft_nm: $prog: no symbols"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj2_eshoff3_corrupted_x64"
    prog=./obj/obj2_eshoff3_corrupted_x64.o
    expected_output="ft_nm: $prog: Invalid section header offset for elf64 file"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj3_ehsize_corrupted_x64"
    prog=./obj/obj3_ehsize_corrupted_x64.o
    compare_nm_and_ft_nm_output "test obj1_identabi_corrupted_x64" $test_number "./obj/obj1_identabi_corrupted_x64.o" "-g"

    test_number=$((test_number + 1))
    sub_test_name="obj3_shentsize_corrupted_x64"
    prog=./obj/obj3_shentsize_corrupted_x64.o
    compare_nm_and_ft_nm_output "test obj1_identabi_corrupted_x64" $test_number "./obj/obj1_identabi_corrupted_x64.o" "-g"

    test_number=$((test_number + 1))
    sub_test_name="obj3_shnum_corrupted_x64"
    prog=./obj/obj3_shnum_corrupted_x64.o
    expected_output="ft_nm: $prog: e_shnum is corrupted or invalid"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj3_shnum2_corrupted_x64"
    prog=./obj/obj3_shnum2_corrupted_x64.o
    expected_output="ft_nm: $prog: no symbols"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj4_shstrndx_corrupted_x64"
    prog=./obj/obj4_shstrndx_corrupted_x64.o
    expected_output="ft_nm: $prog: Failed to find sections string table"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"

    test_number=$((test_number + 1))
    sub_test_name="obj4_shstrndx2_corrupted_x64"
    prog=./obj/obj4_shstrndx2_corrupted_x64.o
    expected_output="ft_nm: $prog: Failed to find sections string table"
    compare_ft_nm_error_with_expected_output "$sub_test_name" $test_number "$prog" "$expected_output"
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

if [ $TEST_ELF32 -eq 1 ]|| [ "$TEST_ALL" -eq 1 ]
then
	test_elf32
fi

if [ $TEST_ERRORS_MISC -eq 1 ]|| [ "$TEST_ALL" -eq 1 ]
then
	test_errors_misc
fi

if [ "$TEST_ERRORS_FILE_TYPE" -eq 1 ] || [ "$TEST_ALL" -eq 1 ]
then
    test_errors_file_type
fi

if [ "$TEST_ERRORS_FILE_CORRUPTED" -eq 1 ] || [ "$TEST_ALL" -eq 1 ]
then
    test_errors_file_corrupted
fi

if [ "$TEST_TMP" -eq 1 ] || [ "$TEST_ALL" -eq 1 ]
then
    test_tmp
fi

echo
echo "That's all folks!"
