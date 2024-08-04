#include "stdio.h"      /* for printf() and fprintf() */
#include "sys/socket.h" /* for socket(), bind(), connect(), send(), recv() */
#include "arpa/inet.h"  /* for sockaddr_in, inet_addr(), inet_ntoa() */
#include "stdlib.h"     /* for atoi() and exit() */
#include "string.h"     /* for memset() */
#include "unistd.h"     /* for close() */

#include "unitExperiment.h" /*Contains all Experiment Functions*/

enum error_t PreExperimentTCPReceiver(int* num_of_packets,
                      unsigned long* inter_packet_departure_spacing,
                      int* probe_packet_length){

  /*Declertion of Constants*/
  const size_t INTEGER_SIZE=sizeof(int);       /*Length of integer*/
  const size_t LONG_SIZE=sizeof(unsigned long);/*Length of Long*/

  /* Length of send buffer holding int, unsigned long, int */
  const unsigned int BUFFER_REC_LENGTH=INTEGER_SIZE*2+LONG_SIZE+100;

  /*Variable Creation and Initializaiton*/
  int servSock;                      /* Socket descriptor for Server */
  int clntSock;                      /* Socket descriptor for Client */
  struct sockaddr_in servAddr;       /* Server address */
  struct sockaddr_in clntAddr;       /* Client address */
  char recBuffer[BUFFER_REC_LENGTH]; /* Buffer for receiving packets */
  size_t clntLen = sizeof(clntAddr); /* Set the size of the in-out parameter */

  /*Initiate a TCP connection*/
  /* Create a reliable, stream socket using TCP */
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
    fprintf(stderr, "ERROR #%d: Socket Setup Error", SOCKET_SETUP_ERROR);
    return SOCKET_SETUP_ERROR;
  }
  
  /* Construct the server address structure */
  memset(&servAddr, 0, sizeof(servAddr));         /* Zero out structure */
  servAddr.sin_family      = AF_INET;             /* Internet address family*/
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* Server IP address */
  servAddr.sin_port        = htons(PRE_TCP_SERVER_PORT);     /* Server port */
//Added By Mahdi Start
	int reuse = 1;
        if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
                printf("Can't set the reuse option on the socket");
  //Added By Mahdi End
  /* Bind to the local address */
  if (bind(servSock,(struct sockaddr *)&servAddr,sizeof(servAddr)) < 0)
  {
    fprintf(stderr, "ERROR #%d: Binding Error", BIND_ERROR);
    return BIND_ERROR;
  }
  
  /* Mark the socket so it will listen for incoming connections */
  if (listen(servSock, 500) < 0)
  {
    fprintf(stderr, "ERROR #%d: Port Listening Error", LISTEN_ERROR);
    return LISTEN_ERROR;
  }
  
  /* Wait for a client to connect */
  if ((clntSock = accept(servSock,(struct sockaddr*)&clntAddr,&clntLen)) < 0)
  {
    fprintf(stderr, "ERROR #%d: Accept Client Error", ACCEPT_CLIENT_ERROR);
    return ACCEPT_CLIENT_ERROR;
  }
  
  /* Receive Data */
  if (recv(clntSock, recBuffer, BUFFER_REC_LENGTH , 0) <= 0)
  {
    fprintf(stderr, "ERROR #%d: Receive Data Error", RECEIVE_ERROR);
    return RECEIVE_ERROR;
  }
  
 /*Extract Data from recBuffer and put into appropriate variables*/                                    
  memcpy(num_of_packets, (recBuffer), INTEGER_SIZE);
  memcpy(inter_packet_departure_spacing, (recBuffer+INTEGER_SIZE), LONG_SIZE);
  memcpy(probe_packet_length, (recBuffer+INTEGER_SIZE+LONG_SIZE), INTEGER_SIZE);

  /*Close Socket*/
  close(servSock);

  return 0;

}


