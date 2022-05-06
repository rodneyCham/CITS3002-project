#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>

#include "rakefile.h"

#define MAX_LINE_LEN 512

Action* parseAction(FILE* fp, char* buf);

Actionset* parseActionset(FILE* fp, char* buf);

Rakefile* parseRakefile(char* filename);

#endif
