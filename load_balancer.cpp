//
// Created by shahlarobert on 03/06/2022.
//

#include "load_balancer.h"
#include "config.h"
#include "packet.h"
#include "utils.h"

using std::hash;

/*route_cid: A function that returns the routing decision made by the load balancer based on the CID value in the
 * received packet.*/
int route_cid(shared_ptr<Packet> processed_packet) {
    uint64_t current_encrypted_cid = processed_packet->get_dcid();
//    std::cout << "route_cid: " << current_encrypted_cid << std::endl; // TODO: remove
    CID current_decrypted_cid(current_encrypted_cid);

    int server_id = current_decrypted_cid.get_server_id(); // server_id is the same as the server index
    return server_id;
}

/*route_policy: A function that returns the routing decision made by the load balancer based on the 4-tuple hash value
 * and in the future it might depend on the actual load of the servers too.*/
int route_policy(shared_ptr<Packet> processed_packet) {
    string four_tuple = processed_packet->get_source_ip().to_string() +
            std::to_string(processed_packet->get_source_port()) +
            processed_packet->get_destination_ip().to_string() +
            std::to_string(processed_packet->get_destination_port());
    return hash<string>{}(four_tuple) % NUM_OF_BACKEND_SERVERS;
}

LoadBalancer::LoadBalancer(int load_balancer_id): MiddleBox(LB_QUEUE_SIZE, "load-balancer-" + std::to_string(load_balancer_id)),
load_balancer_id(load_balancer_id) {}

void LoadBalancer::init(int id) {
    this->load_balancer_id = id;
}

int LoadBalancer::routing_decision(shared_ptr<Packet>* packet) {
    if(this->is_queue_empty())
        return EMPTY_QUEUE;

    shared_ptr<Packet> processed_packet = this->pop_packet();
    assert(processed_packet != nullptr);
    *packet = processed_packet;

    log("LB", SEND, this->load_balancer_id, "processing payload: " + processed_packet->get_payload().serialize());

    if(processed_packet->is_short_header()) {
        return route_cid(processed_packet);
    }

    return route_policy(processed_packet);
}