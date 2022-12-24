//
// Created by shahlarobert on 03/06/2022.
//

#include "middle_box.h"
#include "config.h"


MiddleBox::MiddleBox(int queue_size, const string& name): packet_queue(queue_size), name(name) {}

bool MiddleBox::push_packet(shared_ptr<Packet> packet) {
    return this->packet_queue.enqueue(packet);
}

bool MiddleBox::is_queue_empty() {
    return this->packet_queue.isEmpty();
}

shared_ptr<Packet> MiddleBox::pop_packet() {
    return this->packet_queue.dequeue();
}

int MiddleBox::get_queue_size() {
    return this->packet_queue.get_size();
}

const string &MiddleBox::getName() const {
    return name;
}

int MiddleBox::get_queue_max_size() {
    return this->packet_queue.get_max_size();
}
