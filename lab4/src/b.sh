rm boom
gcc parallel_sum.c /projects/2number9z-os_lab_2019/lab3/src/utils.c -lpthread -o boom
./boom --seed 1 --array_size 10 --threads_num 2