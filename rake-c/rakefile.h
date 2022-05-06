#ifndef RAKEFILE_H
#define RAKEFILE_H

#include <stdbool.h>

/*
	double pointers to make passing (when action is added) each action will be passed as a single value 
	
	if 1 pointer, then in parsing code either have to 
*/

// A struct to store the data for a single action
typedef struct Action {
	bool remote; 		// true if the action should be executed remotely
	char* command;		// the command to execute

	char** requirements;	// the files that the client must send the host for it to run the command
	int rCount;		// the number of requirements for the command
} Action;

// A struct to store the data for a single actionset
typedef struct Actionset {
	Action** actions; 	// an array of pointers to the actions that belong to the actionset (these must be able to be executed in any order on any machine)
	int aCount;		// the number of actions in the set
} Actionset;

// A struct to store the data for the Rakefile
typedef struct Rakefile {
	char* port;		// the default port (may as well be string because never used as int)

	char** hosts; 		// array of host addresses, including ports (e.g. https://localhost:6030)
	int hCount;		// the number of hosts to connect to

	Actionset** actionsets; //array of pointers to the actionsets in the Rakefile (these must be executed in order)
	int asCount;		// the number of actionsets in the Rakefile
} Rakefile;


Action* createAction();
void addRequirements(Action* action, int rCount, char** requirements);

Actionset* createActionset();
void addAction(Actionset* as, Action* action);

Rakefile* createRakefile();
void addActionset(Rakefile* rf, Actionset* as);


#endif
