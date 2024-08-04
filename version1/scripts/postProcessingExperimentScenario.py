#Post Experiment Data Processing Script for A Single Scenario:  

#Name: postProcessingExperimentScenario.py

#This script reads its arguments from a config file "postProcessingExperimentScenario.config"
#So it takes only a single argument:  [path of the config file] (default path is current directory)

import subprocess
import os
import sys
import datetime
import time
import ConfigParser

#Open Config File to Extract Values
experiment_config_file_name = sys.argv[1]
config = ConfigParser.RawConfigParser()
config.readfp(open(experiment_config_file_name))

#content of the config file with the following parameters (i.e. values are set by the config file)
pl_node_address =  config.get('DEFAULT', 'pl_node_addr')#planetLab node address
pl_raw_data_path =  config.get('DEFAULT', 'pl_raw_data_path') #path to the dat files on the PlanetLab node (where copy from)
raw_data_local_path = config.get('DEFAULT', 'raw_data_local_path') #local path to copy the files into
num_of_packets = config.getint('DEFAULT', 'num_of_packets') #number of packets used in each unit experiment, perhaps from .meta file
packet_length = config.getint('DEFAULT', 'packet_length') #perhaps from .meta file
inter_packet_departure_spacing = config.getint('DEFAULT', 'inter_packet_departure_spacing') #perhaps from .meta file
log_file_path = config.get('DEFAULT', 'log_file_path')  #for errors only for this script
experiment_scenario_id = config.getint('DEFAULT', 'experiment_scenario_id') 
intermediate_files_local_path = config.getint('DEFAULT', 'intermediate_files_local_path') #path of intermediate files created (e.g. aggregate delta file)


################################
# COPYING RAW EXPERIMENT DATA
################################

#print "Copying experiment data..."
print "Copying Experiment data..."

#mkdir where the raw experiment data will be copied to
#mkdir raw_data_local_path/experiment_scenario_id
os.mkdir(raw_data_local_path/experiment_scenario_id)

#copy the files from the planetlab node to the local machine (using scp) 
#from pl_node_address's pl_raw_data_path to raw_data_local_path/experiment_scenario_id




################################
# VALIDATING RAW EXPERIMENT DATA
################################

#print "Validating raw experiment data..."
#make sure all errors outputed in this section also includes the human readable format of 
#the experiment timestamps
#also for errors that result in ignoring a particular data file start the error message like this:
#Ignoring xxxxx: (reason for ignoring/error message)

#check that for every _L file there is exactly an _H file and vice versa if not output the file name
#e.g."Missing xxxx_H". In this case, the data will be ignored for intermediate file generation process

#check that for every pair of _L and _H there is a corresponding .meta file
#if not print "Missing xxx.meta" but in this case data will NOT be ignored

#check for number of packets (number of lines)
#each _L and _H file must have exactly num_of_packets lines
#if so output error and ignore this file for intermediate file generation process

#for both _L and _H files check if for first and N-th packet timestamp is not -1.
#for now we are going to ignore this data if either 1st of Nth packets are lost.
#output an error message saying which packet (1st or Nth) in which files were lost

#look in the .meta file and see if exit status = 0 (success) if not ignore the data for 
#intermediate file and output it as error


################################
# GENERATING INTERMEDIATE FILES
################################


#create delta file with the following naming convention in intermediate_files_local_path directory:
#aggregate_delta_(experiment_scenario_id)_(first recorded experiment run timestamp)_to
#_(first recorded experiment run timestamp).dat 
#(timestamps in some human readable format with hours granuality e.g., MMDDYYYY_HH 
#or something like that)

#note that the raw experiment files are in the following format:
#(packet_squence_id)\t(timestamp)\n
#an example of the generated timestamp by unitExperiment: 13612079
#extract timestamp from first and last line for every experiment file pair i
#delta_i = subtract first from last line's timestamp
#delta file is a file with delta_i in every line in chronological order (experiment timestamp)


################################
# GENERATING REPORT FILE
################################

#create an aggregate experiment report file with the following name in the 
#intermediate_files_local_path directory:
#(first recorded experiment run timestamp)_(first recorded experiment run timestamp).report 
#(timestamps in some human readable format with hours granuality e.g., MMDDYYYY_HH 
#or something like that)

#aggregate experiment report file includes:
Number of packets in unit experiment = #num_of_packets
Total number of valid measurements = #validated and used in the generating intermediate files
Total number of corrupted experiment files = #total number of lines didn't match and missing 1 or Nth
Total number of inconsistent experiment files = #total number of corrupted files (missing files)
Total number of failed measurements = #exited with != success
Total number of measurements = #all pairs of _L and _H regardless of being invalid

