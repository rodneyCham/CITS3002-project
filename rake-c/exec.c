
#include <stdio.h>
#include "rakefile.h"


void execAction(Action* act) {
	printf("\t%s", act->command);
	printf(" | is ");
	if(!act->remote) printf("not ");
	printf("remote");
	if(act->rCount > 0) {
		printf(" | requires: ");
		for(int k = 0; k < act->rCount; k++) {
			printf("%s ", act->requirements[k]);
		}
	}
}

void execActionset(Actionset* as) {
	printf("Actionset: (%i action/s)\n", as->aCount);
	for(int i = 0; i < as->aCount; i++) {
		execAction(as->actions[i]);
		printf("\n");
	}
}

void execRakefile(Rakefile* rf) {
	printf("\tPort is: %s\n", rf->port);
	printf("\tHosts are:");
	for(int i = 0; i < rf->hCount; i++) {
		printf(" %s", rf->hosts[i]);
	}
	printf("\n");
	for(int i = 0; i < rf->asCount; i++) {
		execActionset(rf->actionsets[i]);
	}
}
