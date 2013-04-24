/*
Example of usage:
$./compression_node 1 “131.179.192.201”
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include "zlib.h"

#include "unitExperiment.h"

#define UDP_PROBE_PORT_NUMBER "9876"
#define MAX_PACKET_SIZE 10000

enum error_t CompressionNodeUnitExperiment(bool apply_compression,
			      char* decompression_node_addr)
{

  // Set up recv_socket 
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

  // Set up send_socket
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  struct addrinfo* dest_addr_info;
  status = getaddrinfo(decompression_node_addr, UDP_PROBE_PORT_NUMBER, &hints, &dest_addr_info);
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

  // Main forwarding loop
  char packet_buffer[MAX_PACKET_SIZE];
  char compression_buffer[MAX_PACKET_SIZE];
  while (true)
    {
      int packet_size = recvfrom(recv_socket, packet_buffer, MAX_PACKET_SIZE-1, 0, NULL, NULL);

      if (apply_compression)
	{
          // Zlib Compression
      	  z_stream strm;
      	  strm.zalloc = Z_NULL;
      	  strm.zfree = Z_NULL;
	  strm.opaque = Z_NULL;
      	  strm.next_in = (uint8_t*) packet_buffer;
     	  strm.avail_in = packet_size;
    	  strm.next_out = (uint8_t*) compression_buffer;
          strm.avail_out = MAX_PACKET_SIZE;
	  if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK)
	    {
	      fprintf(stderr, "Error #%d: deflateInit compression error", COMPRESSION_ERROR);
	      return COMPRESSION_ERROR;
	    }
	  if (deflateBound(&strm, packet_size) > MAX_PACKET_SIZE)
	    {
	      fprintf(stderr, "Error #%d: deflateBound compression error", COMPRESSION_ERROR);
	      return COMPRESSION_ERROR;
	    }
	  if (deflate(&strm, Z_FINISH) != Z_STREAM_END)
	    {
	      fprintf(stderr, "ERROR #%d: deflate compression Error", COMPRESSION_ERROR);			
	      return COMPRESSION_ERROR;
	    }
          int compressed_packet_size = strm.total_out;
          deflateEnd(&strm);

          // Send compressed packet
          sendto(send_socket, compression_buffer, compressed_packet_size, 0, dest_addr_info->ai_addr, dest_addr_info->ai_addrlen);
	  memset(compression_buffer, 0, MAX_PACKET_SIZE);	
	}
      else
	{
          sendto(send_socket, packet_buffer, packet_size, 0, dest_addr_info->ai_addr, dest_addr_info->ai_addrlen);
	}
      memset(packet_buffer, 0, MAX_PACKET_SIZE);
    }

  freeaddrinfo (dest_addr_info);
  close (recv_socket);
  close (send_socket);
  return SUCCESS;

}

int main (int argc, char *argv[])
{
  int toCompress = atoi(argv[1]);
  return (CompressionNodeUnitExperiment (toCompress, argv[2]) == SUCCESS);
}




