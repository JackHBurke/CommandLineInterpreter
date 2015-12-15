CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -g
OBJECTS = cmi.o cmiBody.o tokenizer.o cmiPipe.o cmiCbreak.o ttymode.o
EXES = cmi calc lcount

all:       cmi calc lcount

cmi:	   $(OBJECTS)
	$(CC) -o $@ $(LDFLAGS) $(OBJECTS)
 
calc:      calc.o 
	$(CC) -o $@ $(LDFLAGS) calc.o

lcount:    lcount.o
	$(CC) -o $@ $(LDFLAGS) lcount.o	

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECS)
