//
// Created by shahlarobert on 02/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_PACKET_QUEUE_H
#define SCHEME_IMPLEMENTATION_PACKET_QUEUE_H

#include "packet.h"
#include "config.h"
#include <mutex>
#include <queue>
#include <semaphore>

using std::mutex;
using std::queue;
using std::binary_semaphore;

/*
 * A queue that is implemented to support concurrent dequeue and enqueue without using locks.
 * The queue is of fixed size. if it overflows, then packets start to be dropped.
 * The implementation is using a cyclic array.
 * */
class PacketQueue {
    queue<shared_ptr<Packet>> packet_queue;
    mutex changing_lock;
    int max_size;
public:
    PacketQueue() = default;
    explicit PacketQueue(int size);
    ~PacketQueue() = default;

    /* inserts the given packet to the queue. returns true if the packet was inserted, otherwise false
     * (if the packet was dropped because the queue is full).*/
    bool enqueue(std::shared_ptr<Packet>& packet);
    std::shared_ptr<Packet> dequeue();
    bool isEmpty();
    int get_size();
    int get_max_size() const;
};


#endif //SCHEME_IMPLEMENTATION_PACKET_QUEUE_H
