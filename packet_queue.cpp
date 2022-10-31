//
// Created by shahlarobert on 02/06/2022.
//

#include "packet_queue.h"

PacketQueue::PacketQueue(int size): max_size(size) {}


void PacketQueue::enqueue(std::shared_ptr<Packet>& packet) {
    if(packet_queue.size() == max_size)
        return;
    changing_lock.lock();
    packet_queue.push(packet);
    changing_lock.unlock();
}

shared_ptr<Packet> PacketQueue::dequeue() {
    if(packet_queue.empty())
        return nullptr;
    changing_lock.lock();
    shared_ptr<Packet> returned_packet = packet_queue.front();
    packet_queue.pop();
    changing_lock.unlock();
    return returned_packet;
}

bool PacketQueue::isEmpty() {
    return packet_queue.empty();
}
