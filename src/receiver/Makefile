
CC	=	gcc
IDIR	=	../include
CFLAGS	=	-O3 -Wall
CFLAGS	+=	-I$(IDIR)

HDR	=	unitExperiment.h
SRC	=	UDPTrainReceiver.c PreExperimentTCPReceiver.c PostExperimentTCPReceiver.c
OBJ 	=	$(SRC:.c=.o)

#LIBS=-lm

%.o: %.c HDR
	$(CC) -c -o $@ $< $(CFLAGS)

unitExperimentReceiver: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY:	clean

clean:
	rm -f $(OBJ) *~ core unitExperimentReceiver

