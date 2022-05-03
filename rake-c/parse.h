#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>

#include "rakefile.h"

#define MAX_LINE_LEN 512

void read_line(FILE* fp, char* buf);

Action* parseAction(FILE* fp, char* buf);

Actionset* parseActionset(FILE* fp, char* buf);

Rakefile* parseRakefile(FILE* fp, char* buf);

#endif