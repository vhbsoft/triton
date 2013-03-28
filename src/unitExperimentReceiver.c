// ./newmain 1000 10 1500 ”~/log/experiment1” ”131.179.192.201” “131.179.192.202” 10 ‘L’
#include "unitExperiment.h" /*Contains all Experiment Functions*/
#include "stdio.h"
#include "stdlib.h"
int main(int argc, char *argv[]){
  int                num_of_packets;
  unsigned long      inter_packet_departure_spacing;
  int                probe_packet_length;
  char*              send_buffer;
  int                send_buffer_length;
  /*Call initial TCP Connection to Set up receiver*/
  if(PreExperimentTCPReceiver(&num_of_packets, &inter_packet_departure_spacing, 
                              &probe_packet_length) != 0){
    fprintf(stderr, "ERROR #%d: PreExperiment TCP Receiver Error", PRE_EXPERIMENT_TCP_RECEIVER_FAILED);
    return PRE_EXPERIMENT_TCP_RECEIVER_FAILED;                    
  }
  /*Call UDP Connection to Receive Data from Client
  if(UDPTrainGenerator(num_of_packets,inter_packet_departure_spacing,  
                      probe_packet_length,entropy,compression_node_addr) != 0){
    return -1;
  }*/
  /*Call Final TCP Connection to receive packets*/
  if(PostExperimentTCPReceiver(send_buffer, send_buffer_length) != 0){
    fprintf(stderr, "ERROR #%d: PostExperiment TCP Receiver Error", POST_EXPERIMENT_TCP_RECEIVER_FAILED);
    return POST_EXPERIMENT_TCP_RECEIVER_FAILED;
  }
}
