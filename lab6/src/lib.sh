gcc -c libmult.c -fPIC -o libmult.o
gcc -shared libmult.o -o libmult.so
LD_LIBRARY_PATH=$(pwd)