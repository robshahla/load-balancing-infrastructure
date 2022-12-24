//
// Created by shahlarobert on 03/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_MIDDLE_BOX_H
#define SCHEME_IMPLEMENTATION_MIDDLE_BOX_H

#include "packet_queue.h"
#include "packet.h"

using std::shared_ptr;

class MiddleBox {
    /*A queue that will contain the received packets from clients, and packets are forwarded from it.*/
    PacketQueue packet_queue;
    string name;

public:
    MiddleBox() = default;
    MiddleBox(int queue_size, const string& name);
    ~MiddleBox() = default;

    /* inserts the given packet to the queue. returns true if the packet was inserted, otherwise false
    * (if the packet was dropped because the queue is full).*/
    bool push_packet(shared_ptr<Packet> packet);
    bool is_queue_empty();
    shared_ptr<Packet> pop_packet();
    int get_queue_size();
    const string &getName() const;
    int get_queue_max_size();
};


#endif //SCHEME_IMPLEMENTATION_MIDDLE_BOX_H
