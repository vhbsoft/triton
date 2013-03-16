#
# Python script to run both low and high entropy and 
# write the data results into a file at the sender
#
# How to run this script:
# python ~/triton/experimentRun [dest_addr] [compression_node_addr]
#

udp_session_timeout = 3000 # 3sec constant for now
num_of_packets = 3000 #constant for now
inter_packet_departure_spacing = 0 #constant for now
probe_packet_length = 1480 # 20 bytes for compression header (assumption: 1500 is MTU). maybe variable later
dest_addr = [dest_addr] #from command-line
compression_node_addr = [compression_node_addr] #from command-line

results_data_file_path = "~/triton/results_data/"
log_file_path = "~/triton/results_data/"  #for errors


# Run for low entropy
entropy = 'L'
results_data_file = results_data_file_path + current_timestamp_posix() + '_L.dat'
log_data_file = log_file_path + current_timestamp_posix() + '_L.dat'

# Execute the following command in terminal
#./unitExperimentSender num_of_packets inter_packet_departure_spacing probe_packet_length "results_data_file" "dest_addr" "compression_node_addr" timeout 'L' > log_data_file


# Wait 1 minute after first unitExperiment is done


# Run for high entropy
entropy = 'H'
results_data_file = results_data_file_path + current_timestamp_posix() + '_H.dat'
log_data_file = log_file_path + current_timestamp_posix() + '_H.dat'

# Execute the following command in terminal
#./unitExperimentSender num_of_packets inter_packet_departure_spacing probe_packet_length "results_data_file" "dest_addr" "compression_node_addr" timeout 'H' > log_data_file








