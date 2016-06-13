IDIR = ../include
CC = gcc
CFLAGS = -I$(IDIR) -Wall

ODIR = ../obj

_DEPS = Digraph.h manager.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Test_Instructions.o Digraph.o manager.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

digraph: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~