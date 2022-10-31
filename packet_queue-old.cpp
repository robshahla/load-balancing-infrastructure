//
// Created by shahlarobert on 02/06/2022.
//

#include "packet_queue.h"

static int incrementCyclicValue(int index, int size);

PacketQueue::PacketQueue(int size): size(size), enqueue_index(0), dequeue_index(0), is_empty(true) {
    queue = new std::shared_ptr<Packet>[this->size];
}

PacketQueue::~PacketQueue() {
    delete[] queue;
}

void PacketQueue::enqueue(std::shared_ptr<Packet>& packet) {
    if(incrementCyclicValue(this->enqueue_index, this->size) == this->dequeue_index) {
        return;
    }
    this->queue[this->enqueue_index] = packet;
    this->is_empty = false;
    this->enqueue_index = incrementCyclicValue(this->enqueue_index, this->size);
}

std::shared_ptr<Packet> PacketQueue::dequeue() {
    if(this->enqueue_index == this->dequeue_index) {
        this->is_empty = true;
        return nullptr;
    }
    std::shared_ptr<Packet> returned_packet = this->queue[this->dequeue_index];
    this->dequeue_index = incrementCyclicValue(this->dequeue_index, this->size);
    return returned_packet;
}

bool PacketQueue::isEmpty() {
    return this->is_empty;
}

static int incrementCyclicValue(int index, int size) {
    return (index + 1) % size;
}