#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Action {
	bool remote;
	char** words;
	char** requirements;
} Action;

typedef struct Actionset {
	char* name;
	Action** actions; //array of pointers to actions
} Actionset;

typedef struct Rakefile {
	int port;
	char** hosts; //array of strings
	Actionset** actionsets; //array of pointers to actionsets
} Rakefile;

int main() {
	printf("yeet\n");

	Actionset actionsets[5];
	Rakefile *rakefile = malloc(sizeof(Rakefile) + 2 * sizeof(char*) + 2 * sizeof(Actionset*));
	rakefile->port = 6330;
	rakefile->hosts = malloc(2 * sizeof(char*));
	rakefile->hosts[0] = "yeet";

	printf("port is %i\n", rakefile->port);
	printf("the size of an actionset pointer is %li\n", sizeof(Actionset*));
	printf("the size of an int is %li\n", sizeof(int));
	printf("the size of an int pointer is %li\n", sizeof(int*));
	printf("%s\n", rakefile->hosts[0]);
	return 0;
}
