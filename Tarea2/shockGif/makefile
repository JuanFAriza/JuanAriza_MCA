CC = gcc
CC_OPTIONS =   -O2 -Wall 
EXEC = euler.x
LIBS = -lm 

OBJS = estructuras.o io.o main.o lax_wendoff.o
INCL = estructuras.h io.h lax_wendoff.h inc.h
INTER = shock.dat
GRAPH = plotShock.py
OUTS = u.png

all: $(OBJS)


.c.o:
	$(CC) $(CC_OPTIONS) -c $<

plotshock:
	python $(GRAPH)

shock:
	./$(EXEC)

exec: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(EXEC)

clean:
	rm -f $(OBJS) *~ core* ${EXEC} ${INTER}
