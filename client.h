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

    /* Generates a packet for sending. If an initial packet was not yet sent by this client, then an initial packet
     * will be sent, otherwise a short header packet will be sent.*/
    shared_ptr<Packet> generate_send_packet();

    /* Receives a packet by the client, this method is called by whoever wants to send the client a packet.*/
    void receive_packet(shared_ptr<Packet> received_packet);

    /* Returns a task duration for the upcoming packet that will be sent.*/
    int get_task_duration();
};


#endif //SCHEME_IMPLEMENTATION_CLIENT_H
