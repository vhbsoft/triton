// TODO add full description of usage here
// ./unitExperiment 1000 10 1500 ”~/log/experiment1” ”131.179.192.201” “131.179.192.202” 10 ‘L’

#include "unitExperiment.h" /*Contains all Experiment Functions*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

struct thread_data
{
  	int                probe_packet_length;
	char*		   send_buffer;
        int 		   send_buffer_length;
};

char send_buffer[150000];    //Hardcoded based on max number of probe packets in each experiment = 6000
int send_buffer_length = 0;

void w_UDPTrainGenerator(struct thread_data* td)
{
	if(UDPTrainReceiver(td->send_buffer, td->send_buffer_length, td->probe_packet_length) != 0){
    	exit(-1);
  	}

}


int main(int argc, char *argv[])
{

  int                num_of_packets;
  unsigned long      inter_packet_departure_spacing;
  int                probe_packet_length;

  /*Call initial TCP Connection to Set up receiver*/
  if(PreExperimentTCPReceiver(&num_of_packets, &inter_packet_departure_spacing, 
                              &probe_packet_length) != 0)
  {
  
    fprintf(stderr, "ERROR #%d: PreExperiment TCP Receiver Error", PRE_EXPERIMENT_TCP_RECEIVER_FAILED);
    return PRE_EXPERIMENT_TCP_RECEIVER_FAILED;                    
  
  }

	pthread_t t0;
	struct thread_data td;
	td.probe_packet_length = probe_packet_length;
	td.send_buffer = send_buffer;
	td.send_buffer_length = send_buffer_length;
	if (pthread_create(&t0, NULL, (void*)w_UDPTrainGenerator, (void*) &td) == -1)
		return -1;

  /*Call initial TCP Connection to Set up receiver*/
  if(PostExperimentTCPReceiver(send_buffer, send_buffer_length) != 0)
  {

    fprintf(stderr, "ERROR #%d: PostExperiment TCP Receiver Error", POST_EXPERIMENT_TCP_RECEIVER_FAILED);
    return POST_EXPERIMENT_TCP_RECEIVER_FAILED;

  }

	pthread_kill(t0,SIGINT);

	pthread_join(t0, NULL);
	return 0;
}


