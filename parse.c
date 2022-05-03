
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "strsplit.h"

void read_line(FILE* fp, char* buf) {
	if(fgets(buf, MAX_LINE_LEN, fp) == NULL) printf("EOF\n");
	printf("%s", buf);
}

// Parses the action that begins on the line of the fp when this function is called
Action* parseAction(FILE* fp, char* buf) {
	Action* action = malloc(sizeof(Action));

	action->remote = false; // for now we only allow local actions
	action->command = malloc(sizeof(char) * (strlen(buf) + 1));
	strcpy(action->command, buf);

	printf("%s\n", action->command);

	action->requirements = malloc(sizeof(char*));
	action->rCount = 0;

	return action;
}


// Parses the actionset that begins on the line of the fp when this function is called
Actionset* parseActionset(FILE* fp, char* buf) {
	Actionset* as = malloc(sizeof(Actionset));

	printf("Reading actionset\n");

	if(fgets(buf, MAX_LINE_LEN, fp) == NULL) printf("EOF\n"); // throw out the name of the actionset since it isn't necessary (unless we want to keep it for debug)
	printf("reading line: %s\n", buf);

	int i = 0;

	while(i < 100) {
		addAction(as, parseAction(fp, buf));
		if(fgets(buf, MAX_LINE_LEN, fp) == NULL) printf("EOF\n");
		i++;
	}

	return as;
}



// Parses the Rakefile that is being accessed by the global fp variable
// Allocates memory for the Rakefile struct and returns it's pointer
Rakefile* parseRakefile(FILE* fp, char* buf) { //TODO @rodney this function I am especially unsure of
	Rakefile* rf = createRakefile();


	// Read the port
	while(strncmp(buf, "PORT", 4) != 0) { // compares
			// first 4 chars of buf with "PORT" and return 0 if equal
		if(fgets(buf, MAX_LINE_LEN, fp) == NULL) exit(-1);
	}

	int nwords; // kinda racist
	char** words = strsplit(buf, &nwords);
	rf->port = atoi(words[2]); 	// this is based on assumption from Rakefile example on website
					// that there is a space either side of equals
	free_words(words);


	// Read the hosts TODO update this to add the default port unless it already has one, otherwise have a new structure for hosts or parallel array for the ports
	while(strncmp(buf, "HOSTS", 5) != 0) {
		if(fgets(buf, MAX_LINE_LEN, fp) == NULL) exit(-1);
	}

	words = strsplit(buf, &nwords);
	rf->hCount = nwords - 2;
	for(int i = 0; i < rf->hCount; i++) {
		rf->hosts[i] = malloc(sizeof(char) * (strlen(words[i+2]) + 1));
		strcpy(rf->hosts[i], words[i+2]);
	}

	// Read the actionsets
	while(buf[0] == '\t' || buf[0] == ' ' || buf[0] == '#' || buf[0] == '\n') {
		if(fgets(buf, MAX_LINE_LEN, fp) == NULL) exit(-1);
	}

	addActionset(rf, parseActionset(fp, buf));

	return rf;
}