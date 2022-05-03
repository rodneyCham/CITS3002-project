#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "rakefile.h"

// TODO
// are these functions actually necessary/good for readability?
// or should there be more?
Action* createAction() {
	Action* action = malloc(sizeof(Action));

	action->requirements = malloc(sizeof(char*));
	action->rCount = 0;

	return action;
}




Actionset* createActionset() {
	Actionset* as = malloc(sizeof(Actionset));

	as->aCount = 0;
	as->actions = malloc(sizeof(Action*));

	return as;
}

void addAction(Actionset* as, Action* action) { // Actionset as
	as->aCount++;
	as->actions = realloc(as->actions, as->aCount * sizeof(Action*)); // as.aCount (*as).aCount
	as->actions[as->aCount - 1] = action;
}



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







