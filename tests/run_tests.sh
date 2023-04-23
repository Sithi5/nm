#!/bin/sh

TEST_ELF64_SYMBOLS_COUNT=1
TEST_ERRORS_MISC=1
TEST_ERRORS_FILE_TYPE=1
TEST_ELF64=0
TEST_DISPLAY_KO=1

_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_RED=`tput setaf 1`
_PURPLE=`tput setaf 5`
_END=`tput sgr0`


# echo filename
echo "\n\n\n\n${_PURPLE}$0 :${_END}\n"

test_elf64_symbols_count()
{
    test_name="test_elf64_symbols_count"
    test_number=1
	echo "\n${_YELLOW}${test_name}:${_END}\n"

    test_name="test with -a option, absolute_value binary"

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
        fi
    fi
    test_number=$((test_number + 1))

    test_name="test with -a option, .so file"

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

if [ $TEST_ELF64_SYMBOLS_COUNT -eq 1 ]
then
	test_elf64_symbols_count
fi

if [ $TEST_ELF64 -eq 1 ]
then
	test_elf64
fi

if [ $TEST_ERRORS_MISC -eq 1 ]
then
	test_errors_misc
fi

if [ $TEST_ERRORS_FILE_TYPE -eq 1 ]
then
	test_errors_file_type
fi

echo
echo "That's all folks!"