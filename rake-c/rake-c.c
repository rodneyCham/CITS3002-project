
#include <stdio.h>
#include "rakefile.h"
#include "parse.h"
#include "exec.h"

int main(int argc, char** argv) {
	
	Rakefile* rf;
	
	
	printf("Parsing Rakefile:\n");
	
	if(argc == 2) rf = parseRakefile(argv[2]);
	else rf = parseRakefile("Rakefile");
	
	
	
	printf("\n\nExecuting Rakefile:\n");
	
	execRakefile(rf);

	return 0;
}
