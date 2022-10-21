clang src/fox/core.c -I include/ -c   -O2 
clang++ test.cpp -I include/ -std=c++20 -c  -O2 
clang++ src/fox/fox.cpp -I include/  -std=c++20 -c  -O2 
clang++ src/fox_dissambler/dissambler.cpp -I include/  -std=c++20 -c  -O2 
clang++ src/fox_file/file.cpp -I include/  -std=c++20 -c  -O2 
clang++ test.o file.o core.o fox.o dissambler.o -o test