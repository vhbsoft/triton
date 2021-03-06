/*
Usage: 
./unitExperimentReceiver
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#include "unitExperiment.h" /*Contains all Experiment Functions*/

struct thread_data
{
  	int                probe_packet_length;
  	char*		           send_buffer;
  	long unsigned int  send_buffer_length;
};

char send_buffer[MAX_SEND_BUFFER_SIZE]; //Hardcoded based on max number of probe packets in each experiment = 6000
unsigned long send_buffer_length = 0;

void w_UDPTrainGenerator(struct thread_data* td)
{
	if(UDPTrainReceiver(td->send_buffer, &(td->send_buffer_length), td->probe_packet_length) != 0)
	{
		fprintf(stderr, "ERROR #%d: UDP Train Receiver Error", UDP_TRAIN_RECEIVER_FAILED);
    		//return UDP_TRAIN_RECEIVER_FAILED;  	
    		return;
	}

}


int main(int argc, char *argv[])
{

  int                num_of_packets;
  unsigned long      inter_packet_departure_spacing;
  int                probe_packet_length;

//must loop twice for both low and high entropy  
  //int experiment_iterator;
  //for(experiment_iterator=0;experiment_iterator<2;experiment_iterator++)
  
  //Infinite loop for receiver
  while(1)
  {
  /* Initialize send_buffer to null char*/
  int i;
  for(i=0; i<MAX_SEND_BUFFER_SIZE;i++){
    send_buffer[i] = 0;
  }
  send_buffer_length = 0;
  
  /*Call initial TCP Connection to Set up receiver*/
  if(PreExperimentTCPReceiver(&num_of_packets, &inter_packet_departure_spacing, 
                              &probe_packet_length) != 0)
  {
    fprintf(stderr, "ERROR #%d: PreExperiment TCP Receiver Error", PRE_EXPERIMENT_TCP_RECEIVER_FAILED);
    return PRE_EXPERIMENT_TCP_RECEIVER_FAILED;                    
  }
	printf("Pre is done\n");
	pthread_t t0;
	struct thread_data td;
	td.probe_packet_length = probe_packet_length;
	td.send_buffer = send_buffer;
	td.send_buffer_length = send_buffer_length;
	if (pthread_create(&t0, NULL, (void*)w_UDPTrainGenerator, (void*) &td) == -1)
		return -1;
	printf("UDP is done\n");
  
  /*Call initial TCP Connection to Set up receiver*/
  if(PostExperimentTCPReceiver( send_buffer, MAX_SEND_BUFFER_SIZE) != 0)
  {
    fprintf(stderr, "ERROR #%d: PostExperiment TCP Receiver Error", POST_EXPERIMENT_TCP_RECEIVER_FAILED);
    return POST_EXPERIMENT_TCP_RECEIVER_FAILED;
  }	
	printf("Post is done\n");

	pthread_kill(t0,SIGINT);
	pthread_join(t0, NULL);
  }
	return 0;

}


