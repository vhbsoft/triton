#ifndef UNITEXPERIMENT_H
#define UNITEXPERIMENT_H
enum error_t {
 SUCCESS                              = 0,
 FAIL                                 = 1,
 PRE_EXPERIMENT_TCP_SENDER_FAILED     = 2,
 PRE_EXPERIMENT_TCP_RECEIVER_FAILED   = 3,
 POST_EXPERIMENT_TCP_SENDER_FAILED    = 4,
 POST_EXPERIMENT_TCP_RECEIVER_FAILED  = 5,
 SOCKET_SETUP_ERROR                   = 102,
 CONNECT_ERROR                        = 103,
 SEND_ERROR                           = 104,
 RECEIVE_ERROR                        = 105,
 BIND_ERROR                           = 106,
 LISTEN_ERROR                         = 107,
 ACCEPT_CLIENT_ERROR                  = 108
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
enum error_t PostExperimentTCPConnection(
        char*  experiment_run_log_absolute_filename, //Log File Name to Be Created
        char* dest_addr //Address of Destination Server
        );
enum error_t PostExperimentTCPReceiver(
        char* send_buffer, //The Data that was received during UDP connection
        int send_buffer_length //Length of Buffer received during UDP connection
        );
#endif