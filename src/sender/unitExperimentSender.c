/*
Usage:
./unitExperimentSender --numPackets1000 --interPacketDepartureSpacing10 --probePacketLength1500 --logFile”~/log/experiment1” --destAddr”131.179.192.201” --compressionNodeAddr”131.179.192.202” --timeout10 --entropy’L’

Example:
./unitExperimentSender 1000 10 1500 ”~/log/experiment1” ”131.179.192.201” “131.179.192.202” 10 ‘L’
*/


#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>

#include "unitExperiment.h" /*Contains all Experiment Functions*/

int main(int argc, char *argv[])
{

  int                num_of_packets = atoi(argv[1]);           //Number of Packets [1,60000]
  unsigned long      inter_packet_departure_spacing = strtoul(argv[2],0,0);  //[0,10^9]    in nano-seconds
  int                probe_packet_length = atoi(argv[3]);   //[0,1500]    in bytes
  char*              experiment_run_log_absolute_filename = argv[4];    //FILENAME_SIZE = 255
  char*              dest_addr = argv[5];
  char*              compression_node_addr = argv[6];
  unsigned long      packet_train_timeout = strtoul(argv[7],0,0);     //[0,10^6]     in msec
  char*              entropy = argv[8]; 
 //char*              source_addr = argv[9];
  char*              source_addr = "127.000.000.001";
  int timeout_estimation = 3;

  /*Call initial TCP Connection to Set up receiver
  if(PreExperimentTCPConnection(num_of_packets,inter_packet_departure_spacing,
                      probe_packet_length,&timeout_estimation,dest_addr) != 0)
  {
    fprintf(stderr, "ERROR #%d: PreExperiment TCP Sender Error", PRE_EXPERIMENT_TCP_SENDER_FAILED);
    return PRE_EXPERIMENT_TCP_SENDER_FAILED;                    
  }*/

  /*Call UDP Connection to Send Data to Receiver*/
  if(UDPTrainGenerator(num_of_packets,inter_packet_departure_spacing,  
                      probe_packet_length,entropy[0],compression_node_addr, source_addr) != 0)
  {
    fprintf(stderr, "ERROR #%d: UDP Train Generator Error", UDP_TRAIN_GENERATOR_FAILED);
    return UDP_TRAIN_GENERATOR_FAILED;                    
  }

  /*CREATE WAIT TIMEOUT FUNCTION*/
  sleep(packet_train_timeout);

  /*Call Final TCP Connection to receive packets
  if(PostExperimentTCPConnection(experiment_run_log_absolute_filename, dest_addr) != 0)
  {
    fprintf(stderr, "ERROR #%d: PostExperiment TCP Sender Error", POST_EXPERIMENT_TCP_SENDER_FAILED);
    return POST_EXPERIMENT_TCP_SENDER_FAILED;                    
  }*/

  return SUCCESS;
}


