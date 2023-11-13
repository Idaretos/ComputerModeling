/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int main (int argc, char *argv[]) {
     uint32_t nPackets = 1;
    //  std::string animFile = "myfirst-animation.xml";

    CommandLine cmd (__FILE__);
    cmd.AddValue("nPackets", "Number of packets to echo", nPackets);
    cmd.Parse (argc, argv);
    
    Time::SetResolution (Time::NS);

    /*
    The logs will be shown in the console output of the program. The log levels are used to control the verbosity of the logs. There are several log levels available in NS-3, including:

        LOG_LEVEL_ALL: Used to enable all log levels.
        LOG_LEVEL_DEBUG: The most verbose log level, used for debugging purposes.
        LOG_LEVEL_INFO: Used to provide general information about the program's execution.
        LOG_LEVEL_WARN: Used to indicate a potential issue or problem that may need attention.
        LOG_LEVEL_ERROR: Used to indicate an error that has occurred.
        LOG_LEVEL_FATAL: Used to indicate a fatal error that will cause the program to terminate.
        LOG_LEVEL_NONE: No logs will be shown.
    
    By default, NS-3 logs at the INFO level and above. You can change the log level for a specific component using the LogComponentEnable function.
    For example, 
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    will enable logging for the UdpEchoClientApplication component at the INFO level.
    */
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NS_LOG_INFO("Creating Topology");
    NodeContainer nodes;
    nodes.Create (2);

    PointToPointHelper pointToPoint; // create a point to point helper
    /* comment the following two lines to set the attributes from command line arguments */ 
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps")); // set the data rate of the net devices
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms")); // set the delay of the channel

    NetDeviceContainer devices;
    devices = pointToPoint.Install (nodes); // install the net devices on the nodes of the container

    InternetStackHelper stack;
    stack.Install (nodes); // install the internet stack on the nodes of the container (internet stack = IP, TCP, UDP, etc.)

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0"); // set the base address and mask

    Ipv4InterfaceContainer interfaces = address.Assign (devices); // assign the addresses to the net devices

    UdpEchoServerHelper echoServer (9); // create a UDP echo server on port 9

    ApplicationContainer serverApps = echoServer.Install (nodes.Get (1)); // install the server application on node 1
    serverApps.Start (Seconds (1.0)); // start the server application after 1 second
    serverApps.Stop (Seconds (10.0)); // stop the server application after 10 seconds

    UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9); // create a UDP echo client on node 0, port 9
    // echoClient.SetAttribute ("MaxPackets", UintegerValue (1)); // set the number of packets to echo to 1
    echoClient.SetAttribute ("MaxPackets", UintegerValue (nPackets));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0))); // set the interval between packets to 1 second
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024)); // set the packet size to 1024 bytes

    ApplicationContainer clientApps = echoClient.Install (nodes.Get (0)); // install the client application on node 0
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));


    AsciiTraceHelper ascii; // create an AsciiTraceHelper object
    pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("myfirst.tr")); // enable ascii tracing on all point to point devices and create a file named myfirst.tr
    pointToPoint.EnablePcapAll ("myfirst"); // enable pcap tracing on all point to point devices and create a file named myfirst.pcap
    AnimationInterface anim (animFile); // create an AnimationInterface object and create a file named myfirst-animation.xml
    // The tracing files will be created in the directory where the program is executed. (ex. ~/ns-allinone-3.36.1/ns-3.36.1/myfirst-0-0.pcap)

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}
