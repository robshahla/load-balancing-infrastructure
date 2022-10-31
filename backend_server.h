//
// Created by shahlarobert on 03/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_BACKEND_SERVER_H
#define SCHEME_IMPLEMENTATION_BACKEND_SERVER_H

#include "middle_box.h"
#include "config.h"
#include "packet.h"
#include <map>

using std::map;

class BackendServer : public MiddleBox{
    IpAddr server_ip;
    int server_port;
    map<uint64_t , uint64_t> server_client_cid; // A map that saves a map from a server CID to the relevant client CID.
    int server_id;
    int nonce;
    int phase;
public:
    BackendServer();

    BackendServer(int server_id, IpAddr server_ip, int server_port);

    ~BackendServer() = default;

    /* init: Initialize the fields in this object*/
    void init(int id, IpAddr ip, int port);

    /*process_respond_packet: A method that processes one packet from the packet queue (if it's not empty), and
     * generates a response packet, and returns it. if the queue is empty then `nullptr` is returned.*/
    shared_ptr<Packet> process_respond_packet();

    /*long_header_response: Generate a long header packet for responding to an incoming long header packet,
     * assuming that the long header packet received from the client is an initial packet.*/
    shared_ptr<Packet> long_header_response(shared_ptr<Packet> processed_packet);

    /*short_header_response: Generate a short header packet for responding to an incoming short header packet.*/
    shared_ptr<Packet> short_header_response(shared_ptr<Packet> processed_packet);

    /*get_new_cid: Allocate a new cid for a new client.*/
    uint64_t allocate_new_cid();

};


#endif //SCHEME_IMPLEMENTATION_BACKEND_SERVER_H
