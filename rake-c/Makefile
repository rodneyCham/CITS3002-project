
CC              = cc
CFLAGS          = -std=c99 -O -Wall -Werror -pedantic

OBJ				      = strsplit.o

build: rakefile.c $(OBJ)
	$(CC) $(CFLAGS) -o rf rakefile.c $(OBJ)

%.o:	%.c
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f tester $(OBJ)
