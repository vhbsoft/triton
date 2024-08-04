
#!/bin/bash

for packetNum in 10 20 50 100 200 400 600 800 1000; do
  for entropy in l h; do
    ./ns3 run compression-exp -- --filename=myconfig.txt --packetNumber=$packetNum --compLinkCap=10Mbps --entropy=$entropy
  done
done
