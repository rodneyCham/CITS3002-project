
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	if(argc != 2) {
		printf("Failed execution\n");
		return -1;
	}
	else {
		srand(getpid());

		int seconds = rand() % 5 + 1;
		sleep(mu);
		printf("\tExecuted the command: \"%s\" (%i seconds)\n", argv[1], seconds);
		return 0;
	}
}
