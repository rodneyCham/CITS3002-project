
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	if(argc != 2) {
		printf("\t\033[0;36mtest_command\033[0;37m: Failed execution\n");
		return -1;
	}
	else {
		srand(getpid());

		int seconds = rand() % 5 + 1;
		sleep(seconds);
		printf("\t\033[0;36mtest_command\033[0;37m: Executed the command: \"%s\" (%i seconds)\n", argv[1], seconds);
		return 0;
	}
}
