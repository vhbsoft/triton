
#initalConfigurePLNodes.py
#Version 1.0

#PlanetLab Nodes Initial Configuration Script
#This script will prepare the PlanetLab nodes for the experiment
#by installing necessary packages and tools, and moving and compiling the experiment code 

#Argument: a file consist of PlanetLab nodes (all in domain name format) one at each line 

################################
#Install versioning system
################################
#setup_version = 1.0
#skip for now

#Commands can be issued remotely using ssh in the following format:
#ssh ucla_triton@[planetlab_hostname] "[list of commands]" (e.g.,"ls -l; ps -aux; whoami")

################################
#Install necessary packages
################################
#zlib-devel.i386 is for compression modules
sudo yum groupinstall "Development Tools"    
sudo yum -y install man gzip less gcc flex bison bc rpm-build make iperf zlib-devel.i386 git 

#Install pathrate (skip for now)

###########################################
#Create directory structure on the node
###########################################


#mkdir ~/triton_1_0
#mkdir ~/triton_results
#mkdir ~/triton_logs


###########################################
#Copy executables or source files
###########################################
#use the github repository
#git clone inside ~/triton_1_0
#config files are also assumed to be included in the git repository

###########################################
#Compile the source code
###########################################
#run 'make' in every single directory (sender/receiver/compression/decompression)




