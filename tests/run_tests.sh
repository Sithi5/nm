#!/bin/sh

TEST_ELF64=1

_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_RED=`tput setaf 1`
_CYAN=`tput setaf 3`
_END=`tput sgr0`


# echo filename
echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"

test_elf64()
{
	echo "\n${_GREEN}Test with elf64:${_END}\n"

    ./ft_nm ft_nm > ft_nm.out
    nm ft_nm > nm.out

    if diff -q ft_nm.out nm.out >/dev/null 2>&1
    then
        echo "${_GREEN}test_elf64 OK${_END}"
    else
        echo "${_RED}test_elf64 KO${_END}"
    fi
}

if [ $TEST_ELF64 -eq 1 ]
then
	test_elf64
fi

echo
echo "That's all folks!"