#include <stdio.h>

int const absolute_value __attribute__((section(".gnu.linkonce.wi"))) = 42;

int main() {
    printf("The value of the absolute symbol is %d\n", absolute_value);
    return 0;
}