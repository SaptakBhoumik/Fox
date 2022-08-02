clang src/fox/core.c -I include/ -c   -O2 -pg
clang++ test.cpp -I include/ -std=c++20 -c  -O2 -pg
clang++ src/fox/fox.cpp -I include/  -std=c++20 -c  -O2 -pg
clang++ src/fox_dissambler/dissambler.cpp -I include/  -std=c++20 -c  -O2 -pg
clang++ src/fox_file/file.cpp -I include/  -std=c++20 -c  -O2 -pg
clang++ test.o file.o core.o fox.o dissambler.o -pg -o test