//
// Created by shahlarobert on 04/06/2022.
//

#include "client.h"
#include "utils.h"


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

    shared_ptr<Packet> returned_packet = nullptr;
    log("client", SEND, client_id, "sequence number: " + payload.serialize());
    if(!sent_initial) {
        returned_packet = shared_ptr<Packet>(new LongHeaderPacket(client_ip,
                                                                   VIP,
                                                                   client_port,
                                                                   VPORT,
                                                                   scid,
                                                                   dcid,
                                                                   payload));
        sent_initial = true;
    } else {
        returned_packet = shared_ptr<Packet>(new ShortHeaderPacket(client_ip,
                                                                     VIP,
                                                                     client_port,
                                                                     VPORT,
                                                                     dcid,
                                                                     payload));

    }

    last_packet_acked = false;
    return returned_packet;
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
//    std::srand(RANDOM_SEED);
    return (std::rand() % 10 + 1) * 1000 ; // TODO: make this generate a random number depending on seed
}