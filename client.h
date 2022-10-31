//
// Created by shahlarobert on 04/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_CLIENT_H
#define SCHEME_IMPLEMENTATION_CLIENT_H

#include "packet.h"
#include "config.h"

class Client {
    int client_id;
    IpAddr client_ip;
    int client_port;
    uint64_t scid;
    uint64_t dcid;
    shared_ptr<Packet> last_received_packet;
    bool sent_initial;
    int sequence_number;
    bool last_packet_acked;

public:
    Client() = default;
    Client(int client_id, IpAddr client_ip, int client_port);
    shared_ptr<Packet> generate_send_packet();
    void receive_packet(shared_ptr<Packet> received_packet);
};


#endif //SCHEME_IMPLEMENTATION_CLIENT_H
