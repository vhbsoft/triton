CC    =    gcc
IDIR    =    ../include
CFLAGS    =    -O3 -Wall
CFLAGS    +=    -I$(IDIR)

HDR    =    unitExperiment.h
SRC    =    unitExperimentReceiver.c
SRC    +=    UDPTrainReceiver.c PostExperimentTCPReceiver.c PreExperimentTCPReceiver.c
OBJ     =    $(SRC:.c=.o)

#LIBS=-lm

%.o: %.c HDR
	$(CC) -c -o $@ $< $(CFLAGS)

unitExperimentReceiver: $(OBJ)
	$(CC)  -o $@ $^ $(CFLAGS) -lrt -lpthread

.PHONY:    clean

clean:
	rm -f $(OBJ) *~ core unitExperimentReceiver

