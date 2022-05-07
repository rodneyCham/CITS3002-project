
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "rakefile.h"

pid_t execAction(Action* act) {
	
	pid_t pid = fork();
	
	if(pid == 0) { // we are in the child process
		execl("./test_command", "./test_command", act->command, (char*)NULL);
		return 0;
	}
	else { // we are in the parent process
		return pid;
	}
	
	/*
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
	*/
}

void execActionset(Actionset* as) {
	printf("Actionset: (%i action/s)\n", as->aCount);
	
	pid_t* pids = malloc(as->aCount * sizeof(pid_t*));
	
	for(int i = 0; i < as->aCount; i++) {
		pids[i] = execAction(as->actions[i]);
		 // atm we are waiting for each action to finish before executing the next, but really, each actionset, we should start all processes consecutively then wait for them all to return
		// printf("\n");
	}
	
	for(int i = 0; i < as->aCount; i++) {
		// printf("\t\tbegan waiting for %i\n", pids[i]);
		waitpid(pids[i], NULL, 0);
		// printf("\t\tfinished waiting for %i\n", pids[i]);
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
