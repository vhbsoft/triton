#include "stdio.h"      /* for printf() and fprintf() */
#include "sys/socket.h" /* for socket(), bind(), connect(), send(), recv() */
#include "arpa/inet.h"  /* for sockaddr_in, inet_addr(), inet_ntoa() */
#include "stdlib.h"     /* for atoi() and exit() */
#include "string.h"     /* for memset() */
#include "unistd.h"     /* for close() */

#include "unitExperiment.h" /*Contains all Experiment Functions*/

/*SERVER PORT*/
#define POST_TCP_SERVER_PORT 26400

enum error_t PostExperimentTCPReceiver(char* send_buffer, int send_buffer_length)
{

  /*Declertion of Constants*/
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
  {
    fprintf(stderr, "ERROR #%d: Socket Setup Error", SOCKET_SETUP_ERROR);
    return SOCKET_SETUP_ERROR;
  }
  
  /* Construct the server address structure */
  memset(&servAddr, 0, sizeof(servAddr));         /* Zero out structure */
  servAddr.sin_family      = AF_INET;             /* Internet address family*/
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* Server IP address */
  servAddr.sin_port        = htons(POST_TCP_SERVER_PORT);     /* Server port */
  

  /* Bind to the local address */
	int reuse = 1;
        if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
                printf("Can't set the reuse option on the socket");

  if (bind(servSock,(struct sockaddr *)&servAddr,sizeof(servAddr)) < 0)
  {
    fprintf(stderr, "ERROR #%d: Binding Error", BIND_ERROR);
    return BIND_ERROR;
  }
  
  /* Mark the socket so it will listen for incoming connections */
  if (listen(servSock, 5) < 0)
  {
    fprintf(stderr, "ERROR #%d: Port Listening Error", LISTEN_ERROR);
    return LISTEN_ERROR;
  }
  
  /* Set the size of the in-out parameter */
  clntLen = sizeof(clntAddr);
  
  /* Wait for a client to connect */
  if ((clntSock = accept(servSock,(struct sockaddr*)&clntAddr,&clntLen)) < 0)
  {
    fprintf(stderr, "ERROR #%d: Accept Client Error", ACCEPT_CLIENT_ERROR);
    return ACCEPT_CLIENT_ERROR;
  } 
  
  /* Send the length of the data to the server */
  int size_of_buffer = strlen(sendBuffer);
 
  printf("%d %s \n", size_of_buffer, sendBuffer);
 
  if (send(clntSock, &size_of_buffer, INTEGER_SIZE, 0) != INTEGER_SIZE)
  {
    fprintf(stderr, "ERROR #%d: Send Data Error", SEND_ERROR);
    return SEND_ERROR;
  }
  
  /* Send the send_buffer data to client */
  if (send(clntSock, sendBuffer, size_of_buffer, 0) != size_of_buffer)
  {
    fprintf(stderr, "ERROR #%d: Send Data Error", SEND_ERROR);
    return SEND_ERROR;
  }

  /*Close Socket*/
  close(servSock);

  return 0;

}


