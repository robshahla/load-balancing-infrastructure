//
// Created by shahlarobert on 04/06/2022.
//

#include "client.h"


Client::Client(int client_id, IpAddr client_ip, int client_port): client_id(client_id), client_ip(client_ip), client_port(client_port),
scid(CLIENT_CID), last_received_packet(nullptr), sent_initial(false), sequence_number(INITIAL_SEQUENCE_NUMBER),
last_packet_acked(true) {
    std::srand(RANDOM_SEED);
    dcid = uint64_t(std::rand());
}

shared_ptr<Packet> Client::generate_send_packet() {
    //In this case the client is waiting for a response from the server.
    if(!last_packet_acked)
        return nullptr;

    int task_duration = get_task_duration();
    Payload payload(
            sequence_number,
            task_duration
            );
//    string payload = std::to_string(sequence_number);
    log("client", SEND, client_id, "sequence number: " + payload.serialize());
    if(!sent_initial) {
        shared_ptr<Packet> long_header_packet(new LongHeaderPacket(client_ip,
                                                                   VIP,
                                                                   client_port,
                                                                   VPORT,
                                                                   scid,
                                                                   dcid,
                                                                   payload));
        sent_initial = true;
        last_packet_acked = false;
//        sequence_number++;
        return long_header_packet;
    }

    shared_ptr<Packet> short_header_packet(new ShortHeaderPacket(client_ip,
                                                                 VIP,
                                                                 client_port,
                                                                 VPORT,
                                                                 dcid,
                                                                 payload));
    last_packet_acked = false;
//    sequence_number++;
    return short_header_packet;
}

void Client::receive_packet(shared_ptr<Packet> received_packet) {
    assert(received_packet);
    last_received_packet = received_packet;
    if(!received_packet->is_short_header()) {
//        std::cout << "Client::receive_packet: " << received_packet->get_scid() << std::endl; // TODO: remove
        dcid = received_packet->get_scid();
    }
    sequence_number++;
    last_packet_acked = true;
    log("client", RECEIVE, client_id, "sequence number: " + received_packet->get_payload().serialize());
}

int Client::get_task_duration() {
    return 5000; // TODO: make this generate a random number depending on seed
}