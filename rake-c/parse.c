
// Rewriting whole code 
// Every time we read a line we should loop through buffer until reaching either a hashtag or end of line, replace hashtag with '\0'
// for hosts default port vs specified port -> loop through to check for ':' and if it doesn't exist append it with default PORT value
// should write a "get_next_nonemtpy_line" function for readability

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include <stdlib.h>

#include "rakefile.h"
#include "strsplit.h"
#include "parse.h"

#define VERBOSE true

// returns false if there is an error or if the EOF is reached
// this function automatically removes any # or newline symbols by replacing with the nullbyte, so when is_line_empty is called,
// 		comment lines are automatically marked as empty, and when reading a line with a comment at the end, the comment is ignored
bool read_line(FILE* fp, char* buf) {
	if(fgets(buf, MAX_LINE_LEN, fp) == NULL) return false;
	int i = 0;
	while(buf[i] != '\0') {
		if(buf[i] == '#' || buf[i] == '\n') {
			buf[i] = '\0';
			break;
		}
		i++;
	}
	return true;
}

// Checks if a line is empty by looping through the string until finding a non-space character (returns false) or the null byte (returns true) 
bool is_line_empty(char* line) {
	int i = 0;
	while(line[i] != '\0') {
		if(!isspace((int)line[i])) return false;
		i++;
	}
	return true;
}

// After this function is called, the next non-empty line in the file will be in the buffer
// Returns true if a new non-empty line has been read
// Returns false if an error occurs or if the end of the file has been reached
bool get_next_nonempty_line(FILE* fp, char* buf) {
	while(read_line(fp, buf)) {
		if(!is_line_empty(buf)) {
			if(VERBOSE) printf("\t%s\n", buf);
			return true;
		}
	}
	return false;
}

// Loops through a string to check for a specific character
// This function is used to check for semicolons in the host values
bool contains(char* string, char c) {
	int i = 0;
	while(string[i] != '\0') {
		if(string[i] == c) return true;
		i++;
	}
	return false;
}

// Counts the number of tabs in a line (to check if it is a new actionset, new action, or requirements line
int count_tabs(char* string) {
	int i = 0;
	while(string[i] == '\t') i++;
	return i;
}


Rakefile* parseRakefile(char* filename) {
	
	// Open file to read from and declare buffer to read into
	FILE* fp = fopen("Rakefile", "r");
	char buf[MAX_LINE_LEN];
	
	// Declare variables for strsplit function // TODO can we make these static so that parseActionset can also use them?
	char** words;
	int wCount;
	
	// Create empty Rakefile struct
	Rakefile* rf = createRakefile();
	
	
	//// Read PORT ////
	get_next_nonempty_line(fp, buf); // the first non-empty line is the one specifying the PORT value
	
	words = strsplit(buf, &wCount);
	
	// The third word of the line is the port value, since the format is, e.g.:
	// 			PORT = 6333
	rf->port = malloc(sizeof(char) * (strlen(words[2]) + 1)); 
	strcpy(rf->port, words[2]);
	
	free_words(words);
	///////////////////
	
	
	
	
	//// Read HOSTS ////
	get_next_nonempty_line(fp, buf); // the next non-empty line is the one specifying the HOSTS
	
	words = strsplit(buf, &wCount);
	
	rf->hCount = wCount - 2;  // the first 2 words in the word array are "HOSTS" then "=", the rest are the names of the hosts
	
	// TODO: Should we put this in the rakefile.c file in a function addHosts(*rf, hCount, **hosts) (if so then we need the contains function in rakefile.c) 
	for(int i = 0; i < rf->hCount; i++) {
		
		// If the host has the port specified, just copy the string into the hosts array
		if(contains(words[i+2], ':')) {
			rf->hosts[i] = malloc(sizeof(char) * (strlen(words[i+2]) + 1));
			strcpy(rf->hosts[i], words[i+2]);
		}
		
		// Otherwise, we need to append the default port before copying into the hosts array
		else {
			rf->hosts[i] = malloc(sizeof(char) * (strlen(words[i+2] + strlen(rf->port) + 2)));
			snprintf(rf->hosts[i], strlen(words[i+2]) + strlen(rf->port) + 2, "%s:%s", words[i+2], rf->port);
		}
	}
	
	free_words(words);
	////////////////////
	
	// Skip the name of the first actionset
	get_next_nonempty_line(fp, buf);
	
	//// Read Actionsets ////
	while(get_next_nonempty_line(fp, buf)) {
		addActionset(rf, parseActionset(fp, buf));
	}
	/////////////////////////
	
	fclose(fp);
	
	return rf;
}

// This function is called when the buffer contains the first action of the actionset
Actionset* parseActionset(FILE* fp, char* buf) {
	// printf("as takes control\n");
	
	char** words;
	int wCount;
	
	Actionset* as = createActionset();
	
	while(true) {
		switch(count_tabs(buf)) {
			case 0: // if the line has no tabs then the next actionset has been reached so return the current one
				return as;
				break;
			
			case 1: // if the line has one tab then it is an action line, so we need to parse an action and add it to the current actionset
				addAction(as, parseAction(fp, buf));
				if(!get_next_nonempty_line(fp, buf)) return as; // if we have reached the end of the file, then return the current actionset, otherwise, go to the next line and repeat
				break;
				
			case 2: // if the line has two tabs then it is a requirements line, so we need to parse the requirements and add them to the most recently parsed action, which is the action in position aCount - 1
				words = strsplit(buf, &wCount);
				as->actions[as->aCount - 1]->rCount = wCount - 1;
				addRequirements(
					as->actions[as->aCount - 1], 
					wCount - 1,
					words + 1
				);
				free_words(words);
				if(!get_next_nonempty_line(fp, buf)) return as; // if we have reached the end of the file, then return the current actionset, otherwise, go to the next line and repeat
				break;
		}
			
	}
	
	return as;
}

// This function is called when the buffer holds a line specifying an action (1 tab), e.g.:
// 		remote-cc -c cubes.c
Action* parseAction(FILE* fp, char* buf) {
	
	Action* act = createAction();
	
	// Check if the action is remote by comparing the first 8 characters of the line with the string "\tremote-"
	act->remote = !strncmp(buf, "\tremote-", 8); // strncmp returns 0 if strings are equal
	
	// If the action *is* remote, then we need to skip the first 8 characters of the line when reading the command (1 character for the tab and 7 characters for "remote-"
	if(act->remote) {
		act->command = malloc(sizeof(char) * (strlen(buf) - 8));
		strcpy(act->command, buf + 8); // start reading at the 9th character in buf
	}
	// if the action is not remote then we only need to skip the first character (the tab)
	else {
		act->command = malloc(sizeof(char) * (strlen(buf)));
		strcpy(act->command, buf + 1); // start reading at the 2nd character in buf
	}
	
	return act;
}
