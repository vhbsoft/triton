#include "stdio.h"      /* for printf() and fprintf() */
#include "sys/socket.h" /* for socket(), bind(), connect(), send(), recv() */
#include "arpa/inet.h"  /* for sockaddr_in, inet_addr(), inet_ntoa() */
#include "stdlib.h"     /* for atoi() and exit() */
#include "string.h"     /* for memset() */
#include "unistd.h"     /* for close() */

#include "unitExperiment.h" /*Contains all Experiment Functions*/

/*SERVER PORT*/
#define SERVER_PORT 16401

enum error_t PostExperimentTCPConnection(char*  experiment_run_log_absolute_filename,
                              char* dest_addr){

  /*Declertion of Constants*/
  const int IP_ADDRESS_BUFFER_LENGTH = 16;   /*length of buffer for IP Address*/
  const size_t INTEGER_SIZE=sizeof(int);     /*Length of integer*/

  /*Variable Creation and Initializaiton*/
  int sock;                                  /* Socket descriptor */
  struct sockaddr_in servAddr;               /* Server address */
  unsigned int BUFFER_REC_LENGTH;            /*Receive Buffer Length*/
  char* recBuffer;                           /* Buffer for sending packets */
  char servIP[IP_ADDRESS_BUFFER_LENGTH];     /* Server IP address (dotted quad) */
  strcpy( servIP, dest_addr);                /*Initialize the servIP with Destination Address*/

  /*Initiate a TCP connection*/
  /* Create a reliable, stream socket using TCP */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    fprintf(stderr, "ERROR #%d: Socket Setup Error", SOCKET_SETUP_ERROR);
    return SOCKET_SETUP_ERROR;
  }

  /* Construct the server address structure */
  memset(&servAddr, 0, sizeof(servAddr));         /* Zero out structure */
  servAddr.sin_family      = AF_INET;             /* Internet address family*/
  servAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
  servAddr.sin_port        = htons(SERVER_PORT);     /* Server port */

  /* Establish the connection to the echo server */
  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
    fprintf(stderr, "ERROR #%d: Connection Setup Error", CONNECT_ERROR);
    return CONNECT_ERROR;
  }

  /*Recieve Length of Data that Will Be Passed*/
  if (recv(sock, &BUFFER_REC_LENGTH, INTEGER_SIZE, 0) <= 0){
    fprintf(stderr, "ERROR #%d: Receive Data Error", RECEIVE_ERROR);
    return RECEIVE_ERROR;
  }

  /*Initialize Receive Buffer with Buffer_Receive_Length*/
  recBuffer = (char*)(malloc(BUFFER_REC_LENGTH));

  /* Receive Data */
  if (recv(sock, recBuffer, BUFFER_REC_LENGTH , 0) <= 0){
    fprintf(stderr, "ERROR #%d: Receive Data Error", RECEIVE_ERROR);
    return RECEIVE_ERROR;
  }

  /*WRITE recBuffer TO FILE*/
  FILE *file;

  /*apend file (add text to a file or create a file if it does not exist.*/
  file = fopen(experiment_run_log_absolute_filename,"a+");

  /*writes recBuffer to the file*/
  fwrite((void*)recBuffer,sizeof(char),BUFFER_REC_LENGTH,file);

  /*closes the file*/ 
  fclose(file); 

  /*Close Socket*/
  close(sock);

  return 0;

}



