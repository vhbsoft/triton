// ./newmain 1000 10 1500 ~/log/experiment1 131.179.192.201 131.179.192.202 10 L
#include "unitExperiment.h" /*Contains all Experiment Functions*/
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
int main(int argc, char *argv[]){
  int                num_of_packets = atoi(argv[1]);           //Number of Packets [1,60000]
  unsigned long      inter_packet_departure_spacing = strtoul(argv[2],0,0);  //[0,10^9]    in nano-seconds
  int                probe_packet_length = atoi(argv[3]);   //[0,1500]    in bytes
  char*              experiment_run_log_absolute_filename = argv[4];    //FILENAME_SIZE = 255
  char*              dest_addr = argv[5];
  char*              compression_node_addr = argv[6];
  unsigned long      packet_train_timeout = strtoul(argv[7],0,0);     //[0,10^6]     in msec
  char*              entropy = argv[8]; 
  int* timeout_estimation;
  /*Call initial TCP Connection to Set up receiver*/
  if(PreExperimentTCPConnection(num_of_packets,inter_packet_departure_spacing,
                      probe_packet_length,timeout_estimation,dest_addr) != 0){
    return -1;                    
  }
  /*Call UDP Connection to Send Data to Receiver
  if(UDPTrainGenerator(num_of_packets,inter_packet_departure_spacing,  
                      probe_packet_length,entropy,compression_node_addr) != 0){
    return -1;
  }*/
  /*CREATE WAIT TIMEOUT FUNCTION*/
  sleep(*timeout_estimation);
  /*Call Final TCP Connection to receive packets*/
  if(PostExperimentTCPConnection(experiment_run_log_absolute_filename, dest_addr) != 0){
    return -1;
  }
}
