#include <stdio.h>
#include <unistd.h>

int main( void ) {
	char *argv[3] = {"sequential_min_max", "1", "10"};

	int pid = fork();

	if ( pid == 0 ) {
        execv("sequential_min_max",  argv);
	}

	wait(0);

	// printf( "Finished executing the parent process\n"
	//         " - the child won't get here--you will only see this once\n" );

	return 0;
}