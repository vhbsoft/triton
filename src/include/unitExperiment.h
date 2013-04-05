#ifndef UNITEXPERIMENT_H
#define UNITEXPERIMENT_H

enum error_t {
 SUCCESS                              = 0,
 FAIL                                 = 1,
 PRE_EXPERIMENT_TCP_SENDER_FAILED     = 2,
 PRE_EXPERIMENT_TCP_RECEIVER_FAILED   = 3,
 POST_EXPERIMENT_TCP_SENDER_FAILED    = 4,
 POST_EXPERIMENT_TCP_RECEIVER_FAILED  = 5,
 UDP_TRAIN_GENERATOR_FAILED           = 6,
 UDP_TRAIN_RECEIVER_FAILED            = 7,
 SOCKET_SETUP_ERROR                   = 101,
 CONNECT_ERROR                        = 102,
 SEND_ERROR                           = 103,
 RECEIVE_ERROR                        = 104,
 BIND_ERROR                           = 105,
 LISTEN_ERROR                         = 106,
 ACCEPT_CLIENT_ERROR                  = 107,
 ADDRINFO_ERROR                       = 108,
 DEVRANDOM_ERROR                      = 201,
 ENTROPY_PARAM_ERROR                  = 202,
 COMPRESSION_ERROR                    = 203,
 DECOMPRESSION_ERROR                  = 204,
 PTHREAD_ERROR                        = 301
};


enum error_t PreExperimentTCPConnection(
        int num_of_packets,
        unsigned long inter_packet_departure_spacing,
        int probe_packet_length,
        int* timeout_estimation,
        char* dest_addr
        );

enum error_t PreExperimentTCPReceiver(
        int* num_of_packets, //Will be initialized and used to pass to UDP
        unsigned long* inter_packet_departure_spacing, //Initialize and Pass to UDP
        int* probe_packet_length //Will be initialized and used to pass to UDP
        );

enum error_t UDPTrainGenerator(
	int num_of_packets,
	unsigned long inter_packet_departure_spacing,
	int probe_packet_length,
	char entropy,
	char* compression_node_addr
	);

enum error_t UDPTrainReceiver(
	char* buffer, 
	unsigned long* buffer_length, 
	int probe_packet_length
);

enum error_t PostExperimentTCPConnection(
        char*  experiment_run_log_absolute_filename, //Log File Name to Be Created
        char* dest_addr //Address of Destination Server
        );

enum error_t PostExperimentTCPReceiver(
        char* send_buffer, //The Data that was received during UDP connection
        int send_buffer_length //Length of Buffer received during UDP connection
        );

enum error_t CompressionNodeUnitExperiment(bool apply_compression,
			      char* decompression_node_addr);

enum error_t DecompressionNodeUnitExperiment(bool applied_compression, 
				char* dest_addr);

#endif
