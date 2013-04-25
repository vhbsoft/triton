
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
import paramiko
import cmd
import time
import sys

#Commands can be issued remotely using ssh in the following format:
#ssh ucla_triton@[planetlab_hostname] "[list of commands]" (e.g.,"ls -l; ps -aux; whoami")
planetlab_hostname = sys.argv[1]

#open file of planetlab host names
#planetlab_host_file = sys.argv[1]
#planetlab_host_file = open(planetlab_host_file, 'r')

#set global variables for username and password
planetlab_username = "ucla_triton"
planetlab_password = "uclaucla"

#for every_line_in_file set planetlab_hostname = this_line until end_of_file
#for planetlab_hostname in planetlab_host_file.readlines():
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.connect(planetlab_hostname, username=planetlab_username, password= planetlab_password)

################################
#Install necessary packages
################################
#zlib-devel.i386 is for compression modules
#sudo yum groupinstall "Development Tools"
stdin, stdout, stderr = ssh.exec_command("sudo yum -y install man gzip less gcc flex bison bc rpm-build make iperf zlib-devel.i386 git")
stdin.write(planetlab_password)
stdin.flush()
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line

#Install pathrate (skip for now)

###########################################
#Create directory structure on the node
###########################################


#mkdir ~/triton_1_0
#mkdir ~/triton_results
#mkdir ~/triton_logs
stdin,stdout,stderr = ssh.exec_command("mkdir ~/triton_1_0")
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line
stdin,stdout,stderr = ssh.exec_command("mkdir ~/triton_results")
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line
stdin,stdout,stderr = ssh.exec_command("mkdir ~/triton_logs")
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line

###########################################
#Copy executables or source files
###########################################
#use the github repository
#git clone inside ~/triton_1_0
#config files are also assumed to be included in the git repository

print "Initializing Git Folder\n"
stdin,stdout,stderr = ssh.exec_command("cd ~/triton_1_0/; git clone git://github.com/vhbsoft/triton.git; cd ~/ ;")
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line
time.sleep(1)

#if git folder already exists, then update it
print "Updating Git Folder\n"
stdin,stdout,stderr = ssh.exec_command("cd ~/triton_1_0/triton/; git pull origin master; cd ~/ ;")
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line
time.sleep(1)

###########################################
#Compile the source code
###########################################
#run 'make' in every single directory (sender/receiver/compression/decompression)
print "Compiling Sender \n"
stdin,stdout,stderr = ssh.exec_command("cd ~/triton_1_0/triton/src/sender/; sudo make;")
stdin.write(planetlab_password)
stdin.flush()
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line
#print "finished make1\n"
time.sleep(1)

print "Compiling Receiver \n"
stdin,stdout,stderr = ssh.exec_command("cd ~/triton_1_0/triton/src/receiver/; sudo make;")
stdin.write(planetlab_password)
stdin.flush()
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line
#print "finished make2\n"
time.sleep(1)

print "Compiling Compression \n"
stdin,stdout,stderr = ssh.exec_command("cd ~/triton_1_0/triton/src/compression/; sudo make;")
stdin.write(planetlab_password)
stdin.flush()
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line
#print "finished make3\n"
time.sleep(1)

print "Compiling Decompression \n"
stdin,stdout,stderr = ssh.exec_command("cd ~/triton_1_0/triton/src/decompression/; sudo make;")
stdin.write(planetlab_password)
stdin.flush()
for line in stdout.read().splitlines():
	print '%s' % line
for line in stderr.read().splitlines():
	print '%s' % line
time.sleep(1)

