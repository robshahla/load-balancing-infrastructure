//
// Created by shahlarobert on 02/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_ROUTER_H
#define SCHEME_IMPLEMENTATION_ROUTER_H

#include <thread>
#include "packet_queue.h"
#include "config.h"
#include "middle_box.h"

using std::shared_ptr;
using std::pair;

class Router : public MiddleBox {
public:
     /* An array that will contain the responses from the servers to the clients. each client has an
      * index in the array, which contains a pair of the CID that the server allocated, along with the
      * sequence number of the message. <CID, sequence_number>*/
//    std::pair<long, int> client_messages[NUM_OF_CLIENTS];

    explicit Router(int packet_queue_size);


    /*routing_decision: Pops a packet from the packet queue, and makes the routing decision based on
     * the source IP address. The method returns the LB ID for which the packet should be routed, or
     * returns `DROP_PACKET` if the packet should be dropped and returns EMPTY_QUEUE id the queue is empty.
     * The routed packet is returned in the argument of the method.*/
    int routing_decision(shared_ptr<Packet>* packet);

    /* clients_send_packets: A function that inserts packets to the packet queue of the router, `input_rate_ms` is the rate in which packets
     * are inserted. The insertion from different clients is choosing a random client each time and inserting
     * a packet form that client to the queue*/
    void clients_send_packets(int input_rate_ms);

    /* A function that given a client index, IP and port, constructs a packet based on the latest response of the
     * server for the given client. If this is the first packet sent from this client, then the constructed packet
     * will be long header packet. Otherwise, it will be a short header packet.*/
    shared_ptr<Packet> construct_packet(int current_client_index, pair<IpAddr, int> ip_port);
};


#endif //SCHEME_IMPLEMENTATION_ROUTER_H
