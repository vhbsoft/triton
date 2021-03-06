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
#include <pthread.h>
#include "unitExperiment.h"


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
        pthread_exit(NULL);

}


enum error_t UDPTrainReceiver (char* buffer, unsigned long* buffer_length, int probe_packet_length)
{
//printf("INSIDE UDP TRAIN GENERATOR\n");
  // Set up recv_socket
	 if (signal(SIGINT, handle_shutdown) == SIG_ERR)
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
//printf("Receive Socket Start\n");
  int recv_socket = socket(my_addr_info->ai_family, my_addr_info->ai_socktype, my_addr_info->ai_protocol);
  if (recv_socket == -1)
    {
	fprintf(stderr, "ERROR #%d: Socket Setup Error", SOCKET_SETUP_ERROR);
	return SOCKET_SETUP_ERROR;
    }
	int reuse = 1;
        if (setsockopt(recv_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
                printf("Can't set the reuse option on the socket");

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
 // char packet_buffer[probe_packet_length];
  char packet_buffer[2000];
  int log_size;
  int i;

  while (true) // This loop must be interrupted by the TCP connection
    {
      //printf("INSIDE LOOP\n");
	if(!recv_socket){
		//printf("recv_socket ended loop\n");		
		return SUCCESS;
	}
	//getting and storing the packet sequence ID
	//int recvCount = 
	//printf("sending receivefrom pakets \n");
	recvfrom(recv_socket, packet_buffer, probe_packet_length-1, 0, NULL, NULL);
	//printf("finished sending receivefrom pakets \n");
	current_seq_id = *((int*) packet_buffer);
	//printf("Data = %s \n",packet_buffer);
	//If there is no missing packets
	if (current_seq_id - last_seq_id == 1)
    	{

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
                log_size = sprintf(temp, "%d\t%ld\n",current_seq_id, diff(startTime, endTime).tv_nsec);
		//log_size = sprintf(temp, "%d ",current_seq_id);
		strcat(buffer,temp);
		//printf("Data = %s\n", buffer);
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




