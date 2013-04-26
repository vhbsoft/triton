#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>

#include "unitExperiment.h"

enum error_t UDPTrainGenerator (int num_of_packets,
                       unsigned long inter_packet_departure_spacing,
                       int probe_packet_length,  
                       char entropy,
                       char* compression_node_addr
)
{

  // Set up send_socket
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  struct addrinfo* dest_addr_info;
  int status = getaddrinfo(compression_node_addr, UDP_PROBE_PORT_NUMBER, &hints, &dest_addr_info);
  if (status != 0)
    {
	fprintf(stderr, "ERROR #%d: Address Information Error", ADDRINFO_ERROR);
	return ADDRINFO_ERROR;
    }

  int send_socket = socket(dest_addr_info->ai_family, dest_addr_info->ai_socktype, dest_addr_info->ai_protocol);
  if (send_socket == -1)
    {
	fprintf(stderr, "ERROR #%d: Socket Setup Error", SOCKET_SETUP_ERROR);
	return SOCKET_SETUP_ERROR;
    }

  // Set up packet_data
  uint8_t* packet_data;
  if (entropy == 'L')
    {
      packet_data = (uint8_t*) calloc (probe_packet_length, 1);
    }
  else if (entropy == 'H')
    {
      packet_data = (uint8_t*) malloc (probe_packet_length);
      FILE* urandom;
      urandom = fopen("/dev/urandom", "r");
      if (urandom == NULL)
	{
		fprintf(stderr, "ERROR #%d: Reading From DevRandom Error", DEVRANDOM_ERROR);	
		return DEVRANDOM_ERROR;
	}
      if( fread(packet_data, probe_packet_length, 1, urandom) != 1)
        fprintf(stderr, "ERROR #%d: FREAD_ERROR", ENTROPY_PARAM_ERROR);	
        
      fclose(urandom);
    }
  else
    { //Should not happen
	fprintf(stderr, "ERROR #%d: Invalild Entropy Type (H/L) Error", ENTROPY_PARAM_ERROR);	
	return ENTROPY_PARAM_ERROR;
    }
  
  int packet_seq_id = 0;
  *((int*) packet_data) = packet_seq_id;
  //time_t last_send = time(NULL);

  //struct timespec inter_packet_departure_spacing_struct;
  //inter_packet_departure_spacing_struct.tv_sec = 0;
  //inter_packet_departure_spacing_struct.tv_nsec = inter_packet_departure_spacing;

  //struct timespec remaining_departure_spacing;

  while (packet_seq_id < num_of_packets)
    {

	  sendto(send_socket, packet_data, probe_packet_length, 0, dest_addr_info->ai_addr, dest_addr_info->ai_addrlen); 
	  packet_seq_id++;
	  *((int*) packet_data) = packet_seq_id;

          //to incorperate inter packet departure spacing
          //nanosleep(&inter_packet_departure_spacing_struct, &remaining_departure_spacing);
          //usleep(inter_packet_departure_spacing);
          int sleep_time;
          for(sleep_time=0;sleep_time < inter_packet_departure_spacing;sleep_time++){}
    }

  freeaddrinfo (dest_addr_info);
  free (packet_data);
  close (send_socket);

  return SUCCESS;
}



