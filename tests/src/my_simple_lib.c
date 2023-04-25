int dynamic() { return 42; }

// gcc -shared -o my_simple_lib.so my_simple_lib.c
// gcc -m32 -shared -o my_simple_lib_32.so my_simple_lib.c