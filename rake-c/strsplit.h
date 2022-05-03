#ifndef STRSPLIT_H
#define STRSPLIT_H

char** strsplit(const char* line, int* nwords);

void free_words(char** words);

#endif
