//
// Created by shahlarobert on 02/06/2022.
//

#include "router.h"
#include "packet.h"
#include <string>

using std::rand;
using std::srand;
using std::shared_ptr;
using std::pair;

/* allocate_ips_ports: Given an array of size `NUM_OF_CLIENTS`, allocates an IP address + Port for each client*/
static void allocate_ips_ports(pair<IpAddr, int>* clients_ips_ports) {
    assert(clients_ips_ports);
    IpAddr base_ip = BASE_CLIENT_IP;
    int base_port = BASE_CLIENT_PORT;

    for(int i = 0; i < NUM_OF_CLIENTS; ++i) {
        IpAddr current_ip = base_ip;
        current_ip.b4 += i;
        int current_port = base_port;

        clients_ips_ports[i].first = current_ip;
        clients_ips_ports[i].second = current_port;
    }
}

Router::Router(int packet_queue_size): MiddleBox(packet_queue_size) {
//    std::srand(RANDOM_SEED);
//    for(int i = 0; i < NUM_OF_CLIENTS; ++i) {
//        client_messages[i].first = long(std::rand());
//
//        client_messages[i].second = INITIAL_SEQUENCE_NUMBER;
//    }
}

/*route_policy: Returns the chosen LB for the given packet, using the 4-th byte
 * of the source IP address modulo the number of LBs.*/
static int route_policy(shared_ptr<Packet> processed_packet) {
    return processed_packet->get_source_ip().b4 % NUM_OF_LBS;
}

int Router::routing_decision(shared_ptr<Packet>* packet) {
    if(this->is_queue_empty()) {
        return EMPTY_QUEUE;
    }
    shared_ptr<Packet> processed_packet = this->pop_packet();
    assert(processed_packet != nullptr);
    *packet = processed_packet;

    // this is hardcoded for now, we assume that we have only one router
    const int current_router_id = 0;
    log("router", SEND, current_router_id, "processing payload: " + processed_packet->get_payload());

    return route_policy(processed_packet);
}

//shared_ptr<Packet> Router::construct_packet(int current_client_index, pair<IpAddr, int> client_ip_port) {
//    long destination_cid = this->client_messages[current_client_index].first;
//    int sequence_number = this->client_messages[current_client_index].second;
//
//    if(sequence_number == INITIAL_SEQUENCE_NUMBER) {
//        shared_ptr<Packet> long_header_packet(new LongHeaderPacket(client_ip_port.first,
//                                                                   VIP,
//                                                                   client_ip_port.second,
//                                                                   VPORT,
//                                                                   CLIENT_CID,
//                                                                   destination_cid,
//                                                                   std::to_string(sequence_number)));
//        return long_header_packet;
//    }
//    shared_ptr<Packet> short_header_packet(new ShortHeaderPacket(client_ip_port.first,
//                                                                 VIP,
//                                                                 client_ip_port.second,
//                                                                 VPORT,
//                                                                 destination_cid,
//                                                                 std::to_string(sequence_number)));
//    return short_header_packet;
//}
//
//void Router::clients_send_packets(int input_rate_ms) {
//    pair<IpAddr, int> clients_ips_ports[NUM_OF_CLIENTS]; // An array of pairs of client IP+Port. [<ip, port>]
//    allocate_ips_ports(clients_ips_ports);
//    srand(RANDOM_SEED);
//
//    int counter = 0;
//    while(counter < NUM_OF_CLIENT_PACKETS) {
//        int current_client = rand() % NUM_OF_CLIENTS;
//        shared_ptr<Packet> packet_to_send = construct_packet(current_client, clients_ips_ports[current_client]);
//        this->push_packet(packet_to_send);
//        counter++;
//        std::this_thread::sleep_for(std::chrono::milliseconds(input_rate_ms));
//    }
//}