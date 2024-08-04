#include "stdio.h"      /* for printf() and fprintf() */
#include "sys/socket.h" /* for socket(), bind(), connect(), send(), recv() */
#include "arpa/inet.h"  /* for sockaddr_in, inet_addr(), inet_ntoa() */
#include "stdlib.h"     /* for atoi() and exit() */
#include "string.h"     /* for memset() */
#include "unistd.h"     /* for close() */

#include "unitExperiment.h" /*Contains all Experiment Functions*/

enum error_t PreExperimentTCPConnection(int num_of_packets,
                      unsigned long inter_packet_departure_spacing,
                      int probe_packet_length,
                      int* timeout_estimation,
                      char* dest_addr){

  /*Declertion of Constants*/
  const size_t INTEGER_SIZE=sizeof(int);     /*Length of integer */
  const size_t LONG_SIZE=sizeof(unsigned long);/*Length of Long*/

  /* Length of send buffer holding int, unsigned long, int */
  const unsigned int BUFFER_SEND_LENGTH=INTEGER_SIZE*2+LONG_SIZE;
  const int IP_ADDRESS_BUFFER_LENGTH = 16;   /*length of buffer for IP Address*/

  /*Variable Creation and Initializaiton*/
  int sock;                                  /* Socket descriptor */
  struct sockaddr_in servAddr;               /* Server address */
  char sendBuffer[BUFFER_SEND_LENGTH];       /* Buffer for sending packets */
  char servIP[IP_ADDRESS_BUFFER_LENGTH];     /* Server IP address (dotted quad) */
  strcpy( servIP, dest_addr);                /*Initialize the servIP with Destination Address*/

  /*fill sendBuffer with data*/                                    
  memcpy((sendBuffer), &num_of_packets, INTEGER_SIZE);
  memcpy((sendBuffer+INTEGER_SIZE), &inter_packet_departure_spacing, LONG_SIZE);
  memcpy((sendBuffer+INTEGER_SIZE+LONG_SIZE), &probe_packet_length, INTEGER_SIZE);

  /*Initiate a TCP connection*/
  /* Create a reliable, stream socket using TCP */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
    fprintf(stderr, "ERROR #%d: Socket Setup Error", SOCKET_SETUP_ERROR);
    return SOCKET_SETUP_ERROR;
  }
  
  /* Construct the server address structure */
  memset(&servAddr, 0, sizeof(servAddr));         /* Zero out structure */
  servAddr.sin_family      = AF_INET;             /* Internet address family*/
  //servAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
  	inet_pton(AF_INET, servIP, &servAddr.sin_addr);
	servAddr.sin_port        = htons(PRE_TCP_SERVER_PORT);     /* Server port */

  /* Establish the connection to the echo server */
  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
  {
    fprintf(stderr, "ERROR #%d: Connection Setup Error IP = %s", CONNECT_ERROR,servIP);
    return CONNECT_ERROR;
  }

  /* Send the data to the server */
  if (send(sock, sendBuffer, BUFFER_SEND_LENGTH, 0) != BUFFER_SEND_LENGTH){
    fprintf(stderr, "ERROR #%d: Send Data Error", SEND_ERROR);
    return SEND_ERROR;
  }

  /*Close Socket*/
  close(sock);
 
 return 0;

}


