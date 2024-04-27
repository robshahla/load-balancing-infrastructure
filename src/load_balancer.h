//
// Created by shahlarobert on 03/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_LOAD_BALANCER_H
#define SCHEME_IMPLEMENTATION_LOAD_BALANCER_H

#include "packet_queue.h"
#include "middle_box.h"

class LoadBalancer : public MiddleBox{
    int load_balancer_id;
public:
    LoadBalancer();

    /* routing_decision: Pops a packet from the packet queue, and makes the routing decision considering if the packet
     * is a long or a short header packet. the method returns the server ID for which the packet should be routed, or
     * returns `DROP_PACKET` if the packet should be dropped and returns EMPTY_QUEUE id the queue is empty.
     * The routed packet is returned in the argument of the method.*/
    int routing_decision(shared_ptr<Packet>* packet);

    void init(int id);
};


#endif //SCHEME_IMPLEMENTATION_LOAD_BALANCER_H
