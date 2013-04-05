
CC	=	gcc
IDIR	=	../include
CFLAGS	=	-O3 -Wall
CFLAGS	+=	-I$(IDIR)

HDR	=	unitExperiment.h
SRC	=	decompression_node_unit_experiment.c
OBJ 	=	$(SRC:.c=.o)

LIBS	=	-lz

%.o: %.c HDR
	$(CC) -c -o $@ $< $(CFLAGS)

decompression_node: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY:	clean

clean:
	rm -f $(OBJ) *~ core decompression_node

