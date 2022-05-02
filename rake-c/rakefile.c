#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strsplit.h"

#define MAX_LINE_LEN 512

// A struct to store the data for a single action
typedef struct Action {
	bool remote; 		// true if the action should be executed remotely
	char* command;		// the command to execute

	char** requirements;	// the files that the client must send the host for it to run the command
	int rCount;		// the number of requirements for the command
} Action;

// TODO
// are these functions actually necessary/good for readability?
// or should there be more?
Action* createAction() {
	Action* action = malloc(sizeof(Action));

	action->requirements = malloc(sizeof(char*));
	action->rCount = 0;

	return action;
}

// A struct to store the data for a single actionset
typedef struct Actionset {
	Action** actions; 	// an array of pointers to the actions that belong to the actionset (these must be able to be executed in any order on any machine)
	int aCount;		// the number of actions in the set
} Actionset;

Actionset* createActionset() {
	Actionset* as = malloc(sizeof(Actionset));

	as->aCount = 0;
	as->actions = malloc(sizeof(Action*));

	return as;
}

void addAction(Actionset* as, Action* action) {
	as->aCount++;
	as->actions = realloc(as->actions, as->aCount * sizeof(Action*));
	as->actions[as->aCount - 1] = action;
}


// A struct to store the data for the Rakefile
typedef struct Rakefile {
	int port;		// the default port

	char** hosts; 		// array of host addresses, including ports (e.g. https://localhost:6030)
	int hCount;		// the number of hosts to connect to

	Actionset** actionsets; //array of pointers to the actionsets in the Rakefile (these must be executed in order)
	int asCount;		// the number of actionsets in the Rakefile
} Rakefile;

Rakefile* createRakefile() {
	Rakefile* rf = malloc(sizeof(Rakefile));

// dont know if this is necessary given how realloc works???
	rf->hosts = malloc(sizeof(char*)); // this will make it so that there is always one more byte than is
	rf->hCount = 0;

	rf->actionsets = malloc(sizeof(Actionset*));
	rf->asCount = 0;

	return rf;
}

void addActionset(Rakefile* rf, Actionset* as) {
	rf->asCount++;
	rf->actionsets = realloc(rf->actionsets, rf->asCount * sizeof(Actionset*));
	rf->actionsets[rf->asCount - 1] = as;
}


FILE* STREAM; 			// A universally accessible filestream to read the Rakefile
char BUFFER[MAX_LINE_LEN];	// A buffer to store each line of the Rakefile (one at a time)

void read_line() {
	if(fgets(BUFFER, MAX_LINE_LEN, STREAM) == NULL) printf("EOF\n");
	printf("%s", BUFFER);
}


// Parses the action that begins on the line of the STREAM when this function is called
Action* parseAction() {
	Action* action = malloc(sizeof(Action));

	action->remote = false; // for now we only allow local actions
	action->command = malloc(sizeof(char) * (strlen(BUFFER) + 1));
	strcpy(action->command, BUFFER);

	printf("%s\n", action->command);

	action->requirements = malloc(sizeof(char*));
	action->rCount = 0;

	return action;
}


// Parses the actionset that begins on the line of the STREAM when this function is called
Actionset* parseActionset() {
	Actionset* as = malloc(sizeof(Actionset));

	printf("Reading actionset\n");

	if(fgets(BUFFER, MAX_LINE_LEN, STREAM) == NULL) printf("EOF\n"); // throw out the name of the actionset since it isn't necessary (unless we want to keep it for debug)
	printf("reading line: %s\n", BUFFER);

	int i = 0;

	while(i < 100) {
		addAction(as, parseAction());
		if(fgets(BUFFER, MAX_LINE_LEN, STREAM) == NULL) printf("EOF\n");
		i++;
	}

	return as;
}



// Parses the Rakefile that is being accessed by the global STREAM variable
// Allocates memory for the Rakefile struct and returns it's pointer
Rakefile* parseRakefile() { //TODO @rodney this function I am especially unsure of
	Rakefile* rf = createRakefile();


	// Read the port
	while(strncmp(BUFFER, "PORT", 4) != 0) { // compares
			// first 4 chars of BUFFER with "PORT" and return 0 if equal
		if(fgets(BUFFER, MAX_LINE_LEN, STREAM) == NULL) exit(-1);
	}

	int nwords; // kinda racist
	char** words = strsplit(BUFFER, &nwords);
	rf->port = atoi(words[2]); 	// this is based on assumption from Rakefile example on website
					// that there is a space either side of equals
	free_words(words);


	// Read the hosts TODO update this to add the default port unless it already has one, otherwise have a new structure for hosts or parallel array for the ports
	while(strncmp(BUFFER, "HOSTS", 5) != 0) {
		if(fgets(BUFFER, MAX_LINE_LEN, STREAM) == NULL) exit(-1);
	}

	words = strsplit(BUFFER, &nwords);
	rf->hCount = nwords - 2;
	for(int i = 0; i < rf->hCount; i++) {
		rf->hosts[i] = malloc(sizeof(char) * (strlen(words[i+2]) + 1));
		strcpy(rf->hosts[i], words[i+2]);
	}

	// Read the actionsets
	while(BUFFER[0] == '\t' || BUFFER[0] == ' ' || BUFFER[0] == '#' || BUFFER[0] == '\n') {
		if(fgets(BUFFER, MAX_LINE_LEN, STREAM) == NULL) exit(-1);
	}

	addActionset(rf, parseActionset());

	return rf;
}


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
		read_line();
	}

	fclose(STREAM);

	return 0;
}
