//
// Created by shahlarobert on 02/06/2022.
//

#include "router.h"
#include "packet.h"
#include "utils.h"
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

    for(int i = 0; i < MAX_NUM_OF_CLIENTS; ++i) {
        IpAddr current_ip = base_ip;
        current_ip.b4 += i;
        int current_port = base_port;

        clients_ips_ports[i].first = current_ip;
        clients_ips_ports[i].second = current_port;
    }
}

Router::Router(int packet_queue_size): MiddleBox(packet_queue_size, "router") {
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
    log("router", SEND, current_router_id, "processing payload: " + processed_packet->get_payload().serialize());

    return route_policy(processed_packet);
}
