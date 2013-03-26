#
# Python script to run both low and high entropy and 
# write the data results into a file at the sender
#
# How to run this script:
# python ~/triton/experimentRun [dest_addr] [compression_node_addr]
#

import subprocess
import sys
import datetime
import time

udp_session_timeout = 3000 # 3sec constant for now
num_of_packets = 3000 #constant for now
inter_packet_departure_spacing = 0 #constant for now
probe_packet_length = 1480 # 20 bytes for compression header (assumption: 1500 is MTU). maybe variable later
dest_addr = sys.argv[1] #from command-line: First Argument
compression_node_addr = sys.argv[2] #from command-line: Second Argument 

#results_data_file_path = "~/triton/results_data/"
#log_file_path = "~/triton/results_data/"  #for errors
results_data_file_path = "results_data/"
log_file_path = "results_data/"  #for errors

#set time stamp
current_time = datetime.datetime.now() #formatted time from python
current_timestamp_posix = time.mktime(current_time.timetuple()) #formats it to unix standard timestamp

# Run for low entropy
entropy = 'L'
results_data_file = results_data_file_path + str(int(current_timestamp_posix)) + '_L.dat'
log_data_file = log_file_path + str(int(current_timestamp_posix)) + '_L.dat'

#open log file to write to
log_file = open(log_data_file, 'w+')

#arguments to be given to subprocess
args = ["./unitExperimentSender",num_of_packets,inter_packet_departure_spacing,probe_packet_length, results_data_file, dest_addr,compression_node_addr, udp_session_timeout,"L"]
str_args = [ str(x) for x in args ] #convert args to string

# Execute the following command in terminal
#./unitExperimentSender num_of_packets inter_packet_departure_spacing probe_packet_length "results_data_file" "dest_addr" "compression_node_addr" timeout 'L' > log_data_file
runExperiment = subprocess.Popen(str_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

stdout_value, stderr_value = runExperiment.communicate('through stdin to stdout')
log_file.write(repr(stdout_value))
log_file.write(repr(stderr_value))
log_file.close()


# Wait 1 minute after first unitExperiment is done
time.sleep(60)

#reset time stamp
current_time = datetime.datetime.now() #formatted time from python
current_timestamp_posix = time.mktime(current_time.timetuple()) #formats it to unix standard timestamp

# Run for high entropy
entropy = 'H'
results_data_file = results_data_file_path + str(int(current_timestamp_posix)) + '_H.dat'
log_data_file = log_file_path + str(int(current_timestamp_posix)) + '_H.dat'

#open log file to write to
log_file = open(log_data_file, 'w+')

#arguments to be given to subprocess
args = ["./unitExperimentSender",num_of_packets,inter_packet_departure_spacing,probe_packet_length, results_data_file, dest_addr,compression_node_addr, udp_session_timeout,"H"]
str_args = [ str(x) for x in args ] #convert args to string

# Execute the following command in terminal
#./unitExperimentSender num_of_packets inter_packet_departure_spacing probe_packet_length "results_data_file" "dest_addr" "compression_node_addr" timeout 'H' > log_data_file
runExperiment = subprocess.Popen(str_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

stdout_value, stderr_value = runExperiment.communicate('through stdin to stdout')
log_file.write(repr(stdout_value))
log_file.write(repr(stderr_value))
log_file.close()







