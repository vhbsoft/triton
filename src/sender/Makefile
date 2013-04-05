
CC	=	gcc
IDIR	=	../include
CFLAGS	=	-O3 -Wall
CFLAGS	+=	-I$(IDIR)

HDR	=	unitExperiment.h
SRC	=	unitExperimentSender.c 
SRC	+=	UDPTrainGenerator.c PreExperimentTCPConnection.c PostExperimentTCPConnection.c
OBJ 	=	$(SRC:.c=.o)

#LIBS=-lm

%.o: %.c HDR
	$(CC) -c -o $@ $< $(CFLAGS)

unitExperimentSender: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY:	clean

clean:
	rm -f $(OBJ) *~ core unitExperimentSender


