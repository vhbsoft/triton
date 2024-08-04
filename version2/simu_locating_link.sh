
#!/bin/bash

for ttl in {1..7}; do
  for entropy in l h; do
    ./ns3 run compression-ttl-none -- --filename=myconfig.txt --ttl=$ttl --entropy=$entropy
  done
done
