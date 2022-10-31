//
// Created by shahlarobert on 03/06/2022.
//

#include "middle_box.h"
#include "config.h"


MiddleBox::MiddleBox(int queue_size): packet_queue(queue_size) {}

void MiddleBox::push_packet(shared_ptr<Packet> packet) {
    this->packet_queue.enqueue(packet);
}

bool MiddleBox::is_queue_empty() {
    return this->packet_queue.isEmpty();
}

shared_ptr<Packet> MiddleBox::pop_packet() {
    return this->packet_queue.dequeue();
}