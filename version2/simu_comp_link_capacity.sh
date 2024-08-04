
#!/bin/bash

for capacity in 1 5 10 20 30 40 50 60 70 80 90 100; do
  for entropy in l h; do
    ./ns3 run compression-exp -- --filename=myconfig.txt --packetNumber=6000 --compLinkCap=${capacity}Mbps --payload=1100 --entropy=$entropy
  done
done
