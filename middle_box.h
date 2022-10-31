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
public:
    MiddleBox() = default;
    explicit MiddleBox(int queue_size);
    ~MiddleBox() = default;
    void push_packet(shared_ptr<Packet> packet);
    bool is_queue_empty();
    shared_ptr<Packet> pop_packet();
};


#endif //SCHEME_IMPLEMENTATION_MIDDLE_BOX_H
