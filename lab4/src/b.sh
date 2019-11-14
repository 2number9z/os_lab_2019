rm boom
gcc parallel_sum.c /projects/2number9z-os_lab_2019/lab3/src/utils.c -L. -lsum -lpthread -o boom
LD_LIBRARY_PATH=$(pwd)
./boom --seed 1 --array_size 10000000 --threads_num 10