// Network Topology
//
//                               
// sender(p0) --- r1(p1) --- r2(p2) --- r3(p3) ---------------- r2(p4) --- receiver(p5)
//            p2p        p2p        p2p        compression link        p2p
//

#include "ns3/applications-module.h"
#include "ns3/compression-module.h"
#include "ns3/core-module.h"
#include "ns3/experiment-module.h"
#include "ns3/internet-module.h"
#include "ns3/log.h"
#include "ns3/network-module.h"
#include "ns3/pcap-file-wrapper.h"
#include "ns3/pcap-file.h"
#include "ns3/ping-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/vector.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Compression-Link-Detection");

struct CompConfig
{
    int UDP_source_port;
    int UDP_dest_port;
    int TCP_dest_port_headSyn; // For standalone application, this is the destination port for the
                               // first TCP packet
    int TCP_dest_port_tailSyn; // For standalone application, this is the destination port for the
                               // second TCP packet
    int TCP_dest_port_part1;
    int UDP_payload_size;
    int UDP_packet_number;
    int UDP_packet_TTL;
    std::string compression_link_capacity;
    uint8_t entropy;
    std::string output_file;
};

std::string trim(const std::string& str);
bool readConfigFile(const std::string& filename, CompConfig& config);

int
main(int argc, char* argv[])
{
    CommandLine cmd;

    std::string filename;
    std::string ttlString;
    std::string entropyString;

    cmd.AddValue("filename", "Name of the config file", filename);
    cmd.AddValue("ttl", "Time to live of the UDP packets", ttlString);
    cmd.AddValue("entropy", "Entropy of the UDP packets", entropyString);
    cmd.Parse(argc, argv);

    if (filename.empty())
    {
        std::cout << "Please provide a config file" << std::endl;
        return 0;
    }

    // Set the TTL value
    u_int8_t ttl = 225;
    int ttl_int = std::stoi(ttlString);
    if (ttl_int >= 0 && ttl_int <= 255)
    {
        ttl = static_cast<u_int8_t>(ttl_int);
    }
    else
    {
        std::cout << "Invalid TTL value. Using default value of 225" << std::endl;
    }

    // Set the entropy value
    uint8_t entropy = 'h';
    if (entropyString == "l" || entropyString == "h")
    {
        entropy = entropyString[0];
    }
    else
    {
        std::cout << "Invalid entropy value. Using default value of 'h'" << std::endl;
    }

    double_t send_interval = 0.00000001;
    std::string outerLinkCapacity =
        "100Mbps";                 // default capacity of the outer link (non compression link)
    bool enableCompression = true; // default value for compression
    const int NODE_SIZE = 6;  // number of nodes in the network

    // Read the config file
    CompConfig config;
    if (!readConfigFile(filename, config))
    {
        std::cerr << "Error reading config file" << std::endl;
        return 1;
    }

    //
    // CREATE THE NETWORK TOPOLOGY
    //

    NodeContainer nodes;
    nodes.Create(NODE_SIZE);

    // Create an array of NdoeContainer
    NodeContainer nodeContainerArray[NODE_SIZE -1];
    for (int i = 0; i < NODE_SIZE -1 ; i++)
    {
        nodeContainerArray[i] = NodeContainer(nodes.Get(i), nodes.Get(i + 1));
    }

    // NodeContainer s0p0 = NodeContainer(nodes.Get(0), nodes.Get(1)); // sender to router1
    // NodeContainer p0p1 = NodeContainer(nodes.Get(1), nodes.Get(2)); // router1 to router2
    // NodeContainer p1r0 = NodeContainer(nodes.Get(2), nodes.Get(3)); // router2 to receiver

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue(outerLinkCapacity));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));
    p2p.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue("655350000p"));
    NetDeviceContainer netDeviceContainerArray[NODE_SIZE - 1];
    for (int i = 0; i < NODE_SIZE - 1; i++)
    {   
        if (i == 3) {continue;}  // skip the compression link
        netDeviceContainerArray[i] = p2p.Install(nodeContainerArray[i]);
    }
    // NetDeviceContainer s0p0_device = p2p.Install(s0p0); // set the link properties of sender to router1
    // NetDeviceContainer p1r0_device = p2p.Install(p1r0); // set the link properties of router2 to receiver

    CompressionHelper compHepler;
    compHepler.SetDeviceAttribute("DataRate", StringValue(config.compression_link_capacity));
    compHepler.SetChannelAttribute("Delay", StringValue("2ms"));
    NetDeviceContainer compression_device = compHepler.Install(nodeContainerArray[3]); // set the link properties of compression link between p3 and p4

    // Set p3 to be a compression device
    Ptr<CompressionNetDevice> p3_device = DynamicCast<CompressionNetDevice>(compression_device.Get(0));
    p3_device->SetEnableCompression(enableCompression);
    p3_device->GetQueue()->SetAttribute("MaxSize", StringValue("655350000p"));
    // Set p4 to be a compression device
    Ptr<CompressionNetDevice> p4_device = DynamicCast<CompressionNetDevice>(compression_device.Get(1));
    p4_device->SetEnableDecompression(enableCompression);



    // Create the Internet stacks
    InternetStackHelper stack;
    stack.Install(nodes);

    // Create addresses
    Ipv4AddressHelper address;
    Ipv4InterfaceContainer interface_container[NODE_SIZE];
    address.SetBase("10.1.1.0", "255.255.255.0");
    interface_container[0] = address.Assign(netDeviceContainerArray[0]);
    address.SetBase("10.1.2.0", "255.255.255.0");
    interface_container[1] = address.Assign(netDeviceContainerArray[1]);
    address.SetBase("10.1.3.0", "255.255.255.0");
    interface_container[2] = address.Assign(netDeviceContainerArray[2]);
    address.SetBase("10.1.4.0", "255.255.255.0");
    interface_container[3] = address.Assign(compression_device);
    address.SetBase("10.1.5.0", "255.255.255.0");
    interface_container[4] = address.Assign(netDeviceContainerArray[4]);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Create a sender application
    CompressionSenderHelper senderHelper(interface_container[4].GetAddress(1),
                                         config.UDP_dest_port); // set destination address and port
    senderHelper.SetAttribute("MaxPackets", UintegerValue(config.UDP_packet_number));
    senderHelper.SetAttribute("Interval", TimeValue(Seconds(send_interval)));
    senderHelper.SetAttribute("PacketSize", UintegerValue(config.UDP_payload_size));
    senderHelper.SetAttribute("TTL", UintegerValue(ttl));

    ApplicationContainer senderApp = senderHelper.Install(nodes.Get(0));
    senderHelper.GetSender()->SetEntropy(entropy);
    senderHelper.GetSender()->SetTcpPort(config.TCP_dest_port_headSyn,
                                         config.TCP_dest_port_tailSyn);
    senderApp.Start(Seconds(1.0));
    senderApp.Stop(Seconds(10.0));

    // Create a receiver application
    CompressionReceiverHelper receiverHelper(config.UDP_dest_port);
    receiverHelper.SetAttribute("NumPackets", UintegerValue(config.UDP_packet_number));
    receiverHelper.SetAttribute("Interval", TimeValue(Seconds(send_interval)));
    receiverHelper.SetAttribute("PacketSize", UintegerValue(config.UDP_payload_size));

    ApplicationContainer receiverApp = receiverHelper.Install(nodes.Get(NODE_SIZE - 1));
    receiverHelper.GetReceiver()->SetLogFileName(config.output_file);
    receiverApp.Start(Seconds(1.0));
    receiverApp.Stop(Seconds(10.0));

    // Enable pcap
    // p2p.EnablePcapAll("compression", true);
    std::string pcapFileName = "ttl_" + ttlString + "_entropy_"+ entropyString;
    p2p.EnablePcap(pcapFileName, netDeviceContainerArray[0].Get(0), true);
    compHepler.EnablePcapAll("compressor_node", true);

    Simulator::Run();
    receiverHelper.GetReceiver()->Process(); // Output the results
    Simulator::Destroy();
    return 0;
}

// Helper function to remove leading/trailing whitespace
std::string
trim(const std::string& str)
{
    size_t start = str.find_first_not_of(' ');
    return (start == std::string::npos) ? ""
                                        : str.substr(start, str.find_last_not_of(' ') + 1 - start);
}

bool
readConfigFile(const std::string& filename, CompConfig& config)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening config file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (getline(file, line))
    {
        // Remove comments
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
        {
            line = line.substr(0, commentPos);
        }

        line = trim(line);
        if (line.empty())
        {
            continue;
        }

        size_t separatorPos = line.find(':');
        if (separatorPos == std::string::npos)
        {
            std::cerr << "Invalid config line: " << line << std::endl;
            continue;
        }

        std::string key = trim(line.substr(0, separatorPos));
        std::string value = trim(line.substr(separatorPos + 1));

        if (key == "UDP_source_port")
        {
            config.UDP_source_port = std::stoi(value);
        }
        else if (key == "UDP_dest_port")
        {
            config.UDP_dest_port = std::stoi(value);
        }
        else if (key == "TCP_dest_port_headSyn")
        {
            config.TCP_dest_port_headSyn = std::stoi(value);
        }
        else if (key == "TCP_dest_port_tailSyn")
        {
            config.TCP_dest_port_tailSyn = std::stoi(value);
        }
        else if (key == "TCP_dest_port_part1")
        {
            config.TCP_dest_port_part1 = std::stoi(value);
        }
        else if (key == "UDP_payload_size")
        {
            config.UDP_payload_size = std::stoi(value);
        }
        else if (key == "UDP_packet_number")
        {
            config.UDP_packet_number = std::stoi(value);
        }
        else if (key == "UDP_packet_TTL")
        {
            config.UDP_packet_TTL = std::stoi(value);
        }
        else if (key == "compression_link_capacity")
        {
            config.compression_link_capacity = value;
        }
        else if (key == "entropy")
        {
            config.entropy = value[0];
        }
        else if (key == "output_file")
        {
            config.output_file = value;
        }
        else
        {
            std::cerr << "Unknown config key: " << key << std::endl;
        }
    }

    return true;
}
