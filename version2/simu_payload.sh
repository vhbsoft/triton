
#!/bin/bash

for payload in 50 100 500 600 700 800 900 1000 1100; do
  for entropy in l h; do
    ./ns3 run compression-exp -- --filename=myconfig.txt --packetNumber=6000 --compLinkCap=10Mbps --payload=$payload --entropy=$entropy
  done
done
