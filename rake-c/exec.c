
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "rakefile.h"
#include "strsplit.h"

pid_t execAction(Action* act) {
	
	char** words;
	int wCount;
	
	words = strsplit(act->command, &wCount);
	
	pid_t pid = fork();
	
	if(pid == 0) {
		execvp(words[0], words);
		return 0;
	}
	
	else {
		return pid;
	}
	
	/*
	pid_t pid = fork();
	
	if(pid == 0) { // we are in the child process
		execl("./test_command", "./test_command", act->command, (char*)NULL); // act->command == "cc -o cube cube.c"
		// execlp("ls", "ls", (char*)NULL);
		return 0;
	}
	else { // we are in the parent process
		return pid;
	}
	*/
	
	/*
	char** words;
	int wCount;
	
	words = strsplit(command, &wCount);
	
	// https://stackoverflow.com/questions/33884291/pipes-dup2-and-exec
	
	int stdout_pipe[2];
	int stderr_pipe[2];
	
	if(pipe(stdout_pipe) || pipe(stderr_pipe)) exit(0);
	pid_t pid = fork();
	
	if(pid == 0) {
		dup2(stdout_pipe[1], STDOUT_FILENO);
		dup2(stderr_pipe[1], STDERR_FILENO);
		close(stdout_pipe[0]);
		close(stderr_pipe[0]);
		execvp(words[0], words);
	}
	
	else {
		close(stdout_pipe[1]);
		close(stderr_pipe[1]);
		int status;
		
		waitpid(pid, &status, 0);
		
		printf("Exited with status %i\n", status);
		
		char stdout_buffer[256];
		char stderr_buffer[256];
		
		int n_stdout_bytes = read(stdout_pipe[0], stdout_buffer, 256);
		int n_stderr_bytes = read(stderr_pipe[0], stderr_buffer, 256);
		
		stdout_buffer[n_stdout_bytes] = '\0';
		stderr_buffer[n_stderr_bytes] = '\0';
		
		printf("The stdout was: (%i bytes) \"%s\"\n", n_stdout_bytes, stdout_buffer);
		printf("The stderr was: (%i bytes) \"%s\"\n", n_stderr_bytes, stderr_buffer);
		
		free_words(words);
	}
	*/
	
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
	int* statuses = malloc(as->aCount * sizeof(int*));
	
	for(int i = 0; i < as->aCount; i++) {
		printf("########################starting action %i with command %s\n", i, as->actions[i]->command);
		pids[i] = execAction(as->actions[i]);
	}
	
	for(int i = 0; i < as->aCount; i++) {
		waitpid(pids[i], statuses + i, 0);
		printf("########################finished waiting for action %i\n", i);
	}
	
	for(int i = 0; i < as->aCount; i++) {
		printf("########################action %i exited with status %i\n", i, statuses[i]);
	}
	
	free(pids);
	free(statuses);
}

void execRakefile(Rakefile* rf) {
	
	/*
	While you may not use this code/approach at the end, have your rake client program execute all actions on your client's computer. 
	Start by using a very simple Rakefile - just a single actionset (initially of just one command), and no remote execution - don't have any remote-XX actions. 
	The action can be very simple - it does not have involve compilation or linking. 
	
	This step will require you to implement code to invoke/execute a new external process (on the current computer), wait for it to finish, and capture/use its exit status. (DONE)

	Some actions will produce output. Where will the output go? 
	How are you going to obtain/capture/access and report their output? (access the stdout of a child process from its parent)
	Actions may also fail, and may report that failure in different ways to different 'locations'. 
	Ensure that you can detect an action that has failed, and consider how you are going to obtain/capture/access and report its failure.

	Next, support the invocation/execution of multiple actions by the rake client, on the client's computer. 
	Initially, just execute each action individually, sequentially, waiting for each action to finish (successfully), before starting the next.

	Finally, add a second and a third actionset, and have each of them execute on the client's computer. 
	Each actionset (in whole) must execute successfully before the next actionset is commenced.
	*/
	
	/*
	char* command = rf->actionsets[0]->actions[0]->command;
	
	char** words;
	int wCount;
	
	words = strsplit(command, &wCount);
	
	// https://stackoverflow.com/questions/33884291/pipes-dup2-and-exec
	
	int stdout_pipe[2];
	int stderr_pipe[2];
	
	if(pipe(stdout_pipe) || pipe(stderr_pipe)) exit(0);
	pid_t pid = fork();
	
	if(pid == 0) {
		dup2(stdout_pipe[1], STDOUT_FILENO);
		dup2(stderr_pipe[1], STDERR_FILENO);
		close(stdout_pipe[0]);
		close(stderr_pipe[0]);
		execvp(words[0], words);
	}
	
	else {
		close(stdout_pipe[1]);
		close(stderr_pipe[1]);
		int status;
		
		waitpid(pid, &status, 0);
		
		printf("Exited with status %i\n", status);
		
		char stdout_buffer[256];
		char stderr_buffer[256];
		
		int n_stdout_bytes = read(stdout_pipe[0], stdout_buffer, 256);
		int n_stderr_bytes = read(stderr_pipe[0], stderr_buffer, 256);
		
		stdout_buffer[n_stdout_bytes] = '\0';
		stderr_buffer[n_stderr_bytes] = '\0';
		
		printf("The stdout was: (%i bytes) \"%s\"\n", n_stdout_bytes, stdout_buffer);
		printf("The stderr was: (%i bytes) \"%s\"\n", n_stderr_bytes, stderr_buffer);
		
		free_words(words);
	}
	*/
	
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


