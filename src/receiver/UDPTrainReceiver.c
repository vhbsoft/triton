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
#include <fcntl.h>
#include <errno.h>

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


enum error_t UDPTrainReceiver (char* buffer, unsigned long* buffer_length, int probe_packet_length, unsigned long initial_experiment_run_time, unsigned long later_experiment_run_time)
{
//printf("INSIDE UDP TRAIN GENERATOR\n");
  // Set up recv_socket
	 if (signal(SIGINT, handle_shutdown) == SIG_ERR)
	   printf("Can’t set the interrupt handler");
  
  int check_ip_address = 1;
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
  fcntl(recv_socket, F_SETFL, O_NONBLOCK);
  
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
  unsigned long experiment_run_time = initial_experiment_run_time;
  struct timespec startTime, endTime;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
  
  struct timespec experiment_startTime, experiment_endTime;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &experiment_startTime);
  
  char temp[100];
 // char packet_buffer[probe_packet_length];
  char packet_buffer[2000];
  char ip_string[IP_ADDRESS_LENGTH+1];
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
	int x = recvfrom(recv_socket, packet_buffer, /*IP_ADDRESS_LENGTH+*/probe_packet_length-1, 0, NULL, NULL);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &experiment_endTime);
	if( diff(experiment_startTime, experiment_endTime).tv_sec >=  experiment_run_time)
    {
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &experiment_startTime);
      experiment_run_time = later_experiment_run_time;
      struct tm file_time;
      tzset();
      //printf("setting time struct \n");
      localtime_r(&(experiment_startTime.tv_sec), &file_time);
      char file_name[36];
      //printf("string copy happening \n");
      strcpy(file_name, ip_string);
      //printf("getting local time \n");
      strftime (file_name+15, 21, "_%Y-%m-%d_%H:%M:%S", &file_time);
      check_ip_address = 1;
	    FILE *file;
	    //printf("opening file \n");
      file = fopen(file_name,"a+");
      //printf("writing to file \n");
      fwrite((void*)buffer,sizeof(char),*buffer_length,file);
      fclose(file); 
    }
  if (x > 0)
  {
	  //printf("done receiving\n");
	
	  /*Check IP Address of First Packet*/
	  if(check_ip_address)
	  {
	    memcpy(ip_string, packet_buffer, IP_ADDRESS_LENGTH);
	    ip_string[IP_ADDRESS_LENGTH]='\0';	    
	    check_ip_address = 0;
	  } 
	  /*End Check IP Address of First Packet*/
	
  	/*Check Timer to Write to Buffer*/

    //printf("diff(experiment_startTime, experiment_endTime).tv_sec = %d", diff(experiment_startTime, experiment_endTime).tv_sec);
   /* if( diff(experiment_startTime, experiment_endTime).tv_sec >= 60 )
    {
      check_ip_address = 1;
	    printf("creating file \n");
	    //Creating a File
	    FILE *file;

      //char file_name[15] = ip_string;
      //apend file (add text to a file or create a file if it does not exist.
      file = fopen(ip_string,"a+");
      printf("file is open: name is %s\n", ip_string);
      //writes recBuffer to the file
      int x_err = fwrite((void*)buffer,sizeof(char),*buffer_length,file);
      printf("fwrite completed with %d\n", x_err);
      //closes the file
      fclose(file); 
      exit(1);
    }*/
	  /*End Check Timer*/
	
	
	  //printf("finished sending receivefrom pakets \n");

  
	  current_seq_id = *((int*) (packet_buffer+IP_ADDRESS_LENGTH));
	  //printf("current_seq_id = %u \n", current_seq_id);
	  //If there is no missing packets
	  if (current_seq_id - last_seq_id == 1)
    {
  
	  	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
      log_size = sprintf(temp, "%d\t%ld\n",current_seq_id, diff(startTime, endTime).tv_nsec);
	  	//log_size = sprintf(temp, "%d ",current_seq_id);
	  	strcat(buffer,temp);
	  	//printf("Data = %s\n", buffer);
	  	*buffer_length += log_size;
   
	  	last_seq_id = current_seq_id;

    }
    else if (current_seq_id - last_seq_id > 1)  	//If there are some missing packets
    {

      		
	  	for (i=last_seq_id+1; i<current_seq_id; i++)
       	{
  
  	  		log_size = sprintf(temp, "\t-1\n");
          strcat(buffer,temp);
       		*buffer_length += log_size;
  
      	}

      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
      log_size = sprintf(temp, "%d\t%ld\n",current_seq_id, diff(startTime, endTime).tv_nsec);
      strcat(buffer,temp);
	  	*buffer_length += log_size;

      last_seq_id = current_seq_id;

    }
    else if (current_seq_id < 10)
    {
	  	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
      log_size = sprintf(temp, "%d\t%ld\n",current_seq_id, diff(startTime, endTime).tv_nsec);
	  	//log_size = sprintf(temp, "%d ",current_seq_id);
	  	strcat(buffer,temp);
	  	//printf("Data = %s\n", buffer);
	  	*buffer_length += log_size;
   
	  	last_seq_id = current_seq_id;    
    }
  }
  }
  close (recv_socket);
  return SUCCESS;

}




