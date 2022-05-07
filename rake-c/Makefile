
CC              = cc
CFLAGS          = -std=c99 -O -Wall -Werror -pedantic

OBJ				= strsplit.o rakefile.o parse.o exec.o

build: rake-c.c $(OBJ)
	$(CC) $(CFLAGS) -o rake-c rake-c.c $(OBJ)

%.o:	%.c
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(OBJ)

test:
	$(CC) $(CFLAGS) -o test_command test_command.c
