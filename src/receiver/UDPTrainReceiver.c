#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>

#include "unitExperiment.h"

#define UDP_PROBE_PORT_NUMBER "200"

struct timespec diff(struct timespec start, struct timespec end)
{
        struct timespec temp;
        if ((end.tv_nsec-start.tv_nsec)<0) {
                temp.tv_sec = end.tv_sec-start.tv_sec-1;
                temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
        } else {
                temp.tv_sec = end.tv_sec-start.tv_sec;
                temp.tv_nsec = end.tv_nsec-start.tv_nsec;
        }
        return temp;
}
int recv_socket;
void handle_shutdown(int sig)
{
        if (recv_socket)
        	close(recv_socket);
        return;

}


enum error_t UDPTrainReceiver (char* buffer, unsigned long* buffer_length, int probe_packet_length)
{

  // Set up recv_socket
	 if (signal(SIGINT, handle_shutdown) == -1)
                printf("Can’t set the interrupt handler");

  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo* my_addr_info;
  int status = getaddrinfo(NULL, UDP_PROBE_PORT_NUMBER, &hints, &my_addr_info);
  if (status != 0)
    {
	fprintf(stderr, "ERROR #%d: Address Information Error", ADDRINFO_ERROR);
	return ADDRINFO_ERROR;
    }

  int recv_socket = socket(my_addr_info->ai_family, my_addr_info->ai_socktype, my_addr_info->ai_protocol);
  if (recv_socket == -1)
    {
	fprintf(stderr, "ERROR #%d: Socket Setup Error", SOCKET_SETUP_ERROR);
	return SOCKET_SETUP_ERROR;
    }

  status = bind(recv_socket, my_addr_info->ai_addr, my_addr_info->ai_addrlen);
  if (status == -1)
    {
	fprintf(stderr, "ERROR #%d: Binding Error", BIND_ERROR);
	close (recv_socket);
	return BIND_ERROR;
    }
  freeaddrinfo(my_addr_info);

  // Main receive loop

  int current_seq_id;
  int last_seq_id = -1;

//  time_t start_time = time(NULL);

  struct timespec startTime, endTime;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
  char temp[100];
  char packet_buffer[probe_packet_length];

  int log_size;
  int i;

  while (true) // This loop must be interrupted by the TCP connection
    {
      
	if(!recv_socket)
		return SUCCESS;

	//getting and storing the packet sequence ID
	recvfrom(recv_socket, packet_buffer, probe_packet_length-1, 0, NULL, NULL);
	current_seq_id = *((int*) packet_buffer);

	//If there is no missing packets
	if (current_seq_id - last_seq_id == 1)
    	{

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
		log_size = sprintf(temp, "%d\t%ld\n",current_seq_id, diff(startTime, endTime).tv_nsec);
		strcat(buffer,temp);

		buffer_length += log_size;
   
		last_seq_id = current_seq_id;

    	}
    	else if (current_seq_id - last_seq_id > 1)  	//If there are some missing packets
    	{

      		
		for (i=last_seq_id+1; i<current_seq_id; i++)
        	{

			log_size = sprintf(temp, "\t-1\n");
        	        strcat(buffer,temp);
        		buffer_length += log_size;

        	}

                clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
                log_size = sprintf(temp, "%d\t%ld\n",current_seq_id, diff(startTime, endTime).tv_nsec);
                strcat(buffer,temp);
		buffer_length += log_size;

		last_seq_id = current_seq_id;

    	} // current_seq_id - last_seq_id < 0 should never happen
    }

  close (recv_socket);
  return SUCCESS;

}




