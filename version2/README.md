### Project Objective:

This project aims to detect and locate compression links within a network simulation environment.

###  Requirements:

This code leverages the ns-3 network simulator, specifically the 3.38 branch. To set up the environment, follow these steps:

1. Clone ns-3: Obtain a local copy of the ns-3 3.38 branch.
1. Copy Modules: Transfer the modules located in the ./contrib directory to the corresponding contrib directory within the ns-3 3.38 installation.
1. Copy Scratch Files: Copy the files from the scratch directory to their designated locations within the ns-3 3.38 environment.
1. Copy Remaining Files: Place any remaining project files in the root directory of your ns-3 3.38 installation.
1. Build Modules: After copying the files, build the two custom modules included in this project.

### Running the Simulation:

Once the setup is complete, you can execute the simulation program using a command like this:

```./ns3 run compression-exp -- --filename=myconfig.txt --packetNumber=6000 --compLinkCap=10Mbps --payload=1100 --entropy=h```

### Functionality:

./scratch/compression-link/compression-exp.cc: This module determines if a compression link exists within the simulated network.
./scratch/compression-ttl/compression-ttl.cc: This module identifies the specific hop (network node) where the compression link is introduced.

### Batch Testing:

The project includes four shell scripts (.sh files) that facilitate batch testing, allowing you to run multiple simulations efficiently.

### Complete Code:

For a comprehensive view of the codebase, including its integration with the ns-3 3.38 branch, please refer to this repository: [\[link to repository\]](https://github.com/xiuhuiwang/ns-3-dev/tree/ns-3.38-branch)