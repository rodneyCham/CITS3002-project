
#include <stdio.h>
#include "rakefile.h"
#include "parse.h"

FILE* STREAM; 			// A universally accessible filestream to read the Rakefile
char BUFFER[MAX_LINE_LEN];	// A buffer to store each line of the Rakefile (one at a time)

int main() {

	STREAM = fopen("Rakefile", "r");

	/*Rakefile* rf = parseRakefile();

	printf("Listening on port %i\n", rf->port);
	for(int i = 0; i < rf->hCount; i++) {
		printf("Host #%i is called %s\n", i, rf->hosts[i]);
	}

	for(int i = 0; i < rf->actionsets[0]->aCount; i++) {
		printf("Actionset 1's Action #%i is called %s\n", i, rf->actionsets[0]->actions[i]->command);
	}
	*/

	for(int i = 0; i < 100; i++) {
		read_line(STREAM, BUFFER);
	}

	fclose(STREAM);

	return 0;
}
