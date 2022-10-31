//
// Created by shahlarobert on 02/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_PACKET_QUEUE_H
#define SCHEME_IMPLEMENTATION_PACKET_QUEUE_H

#include "packet.h"
#include "config.h"

/*
 * A queue that is implemented to support concurrent dequeue and enqueue without using locks.
 * The queue is of fixed size. if it overflows, then packets start to be dropped.
 * The implementation is using a cyclic array.
 * */
class PacketQueue {
    int size;
    std::shared_ptr<Packet>* queue;
    int enqueue_index, dequeue_index;
    bool is_empty;
public:
    PacketQueue() = default;
    explicit PacketQueue(int size);
    ~PacketQueue();
    void enqueue(std::shared_ptr<Packet>& packet);
    std::shared_ptr<Packet> dequeue();
    bool isEmpty();
};


#endif //SCHEME_IMPLEMENTATION_PACKET_QUEUE_H
