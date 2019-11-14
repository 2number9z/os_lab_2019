parallel : parallel_min_max.c /projects/2number9z-os_lab_2019/lab3/src/utils.c /projects/2number9z-os_lab_2019/lab3/src/find_min_max.c
	gcc -o parallel parallel_min_max.c /projects/2number9z-os_lab_2019/lab3/src/utils.c /projects/2number9z-os_lab_2019/lab3/src/find_min_max.c

mem : process_memory.c
	gcc -o mem process_memory.c

sum : parallel_sum.c sum.c /projects/2number9z-os_lab_2019/lab3/src/utils.c
	gcc -c sum.c -o sum.o
	gcc -shared sum.o -o libsum.so
	gcc parallel_sum.c /projects/2number9z-os_lab_2019/lab3/src/utils.c -L. -lsum -lpthread -o sum