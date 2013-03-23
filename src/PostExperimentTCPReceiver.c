#include "unitExperiment.h" /*Contains all Experiment Functions*/
#include "stdio.h"      /* for printf() and fprintf() */
#include "sys/socket.h" /* for socket(), bind(), connect(), send(), recv() */
#include "arpa/inet.h"  /* for sockaddr_in, inet_addr(), inet_ntoa() */
#include "stdlib.h"     /* for atoi() and exit() */
#include "string.h"     /* for memset() */
#include "unistd.h"     /* for close() */

enum error_t PostExperimentTCPReceiver(char* send_buffer, int send_buffer_length){
  /*Declertion of Constants*/
  const unsigned short SERVER_PORT = 16401;  /* Server port */
  const size_t INTEGER_SIZE=sizeof(int);     /*Length of integer*/
  const unsigned int BUFFER_SEND_LENGTH=send_buffer_length;  /* Length of send buffer */
  const int IP_ADDRESS_BUFFER_LENGTH = 16;   /*length of buffer for IP Address*/
  /*Variable Creation and Initializaiton*/
  int servSock;                      /* Socket descriptor for Server */
  int clntSock;                      /* Socket descriptor for Client */
  struct sockaddr_in servAddr;       /* Server address */
  struct sockaddr_in clntAddr;       /* Client address */
  char* sendBuffer = send_buffer;    /* Data filled buffer to Send to Client*/
  size_t clntLen;
  /*Initiate a TCP connection*/
  /* Create a reliable, stream socket using TCP */
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    return -1;
  /* Construct the server address structure */
  memset(&servAddr, 0, sizeof(servAddr));         /* Zero out structure */
  servAddr.sin_family      = AF_INET;             /* Internet address family*/
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* Server IP address */
  servAddr.sin_port        = htons(SERVER_PORT);     /* Server port */
  /* Bind to the local address */
  if (bind(servSock,(struct sockaddr *)&servAddr,sizeof(servAddr)) < 0)
    return -1;
  /* Mark the socket so it will listen for incoming connections */
  if (listen(servSock, 5) < 0)
    return -1;
  /* Set the size of the in-out parameter */
  clntLen = sizeof(clntAddr);
  /* Wait for a client to connect */
  if ((clntSock = accept(servSock,(struct sockaddr*)&clntAddr,&clntLen)) < 0)
    return -1;
  /* Send the length of the data to the server */
  if (send(clntSock, &BUFFER_SEND_LENGTH, INTEGER_SIZE, 0) != BUFFER_SEND_LENGTH)
    return -1;
  /* Send the data to the server */
  if (send(clntSock, sendBuffer, BUFFER_SEND_LENGTH, 0) != BUFFER_SEND_LENGTH)
    return -1;
  /*Close Socket*/
  close(servSock);
  return 0;
}
