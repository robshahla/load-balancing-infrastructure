//
// Created by shahlarobert on 03/06/2022.
//

#include "backend_server.h"

static string construct_response_payload(const string& payload) {
    return payload;
}
BackendServer::BackendServer(): MiddleBox(SERVER_QUEUE_SIZE), server_id(0), nonce(0), phase(0) {}

BackendServer::BackendServer(int server_id, IpAddr server_ip, int server_port): MiddleBox(SERVER_QUEUE_SIZE), server_ip(server_ip),
server_port(server_port), server_id(server_id), nonce(INITIAL_NONCE), phase(INITIAL_PHASE) {}

void BackendServer::init(int id, IpAddr ip, int port) {
    server_id = id;
    server_ip = ip;
    server_port = port;
}

uint64_t BackendServer::allocate_new_cid() {
    CID new_cid(this->server_id, this->nonce, this->phase);
    this->nonce += 1;
    return new_cid.encrypt_cid();
}

shared_ptr<Packet> BackendServer::long_header_response(shared_ptr<Packet> processed_packet) {
    uint64_t response_scid = this->allocate_new_cid();
//    std::cout << "BackendServer::long_header_response: " << response_scid << std::endl; // TODO: remove
    string response_payload = construct_response_payload(processed_packet->get_payload());
    this->server_client_cid[response_scid] = processed_packet->get_scid();
    shared_ptr<Packet> returned_packet(new LongHeaderPacket(this->server_ip,
                                                            processed_packet->get_source_ip(),
                                                            this->server_port,
                                                            processed_packet->get_source_port(),
                                                            response_scid,
                                                            processed_packet->get_scid(),
                                                            response_payload));
//    std::cout << "BackendServer::long_header_response: " << returned_packet->get_scid() << std::endl; // TODO: remove
    return returned_packet;
}

shared_ptr<Packet> BackendServer::short_header_response(shared_ptr<Packet> processed_packet) {
    uint64_t response_dcid = this->server_client_cid[processed_packet->get_dcid()];
    string response_payload = construct_response_payload(processed_packet->get_payload());
    shared_ptr<Packet> returned_packet(new ShortHeaderPacket(this->server_ip,
                                                            processed_packet->get_source_ip(),
                                                            this->server_port,
                                                            processed_packet->get_source_port(),
                                                            response_dcid,
                                                            response_payload));
    return returned_packet;
}


shared_ptr<Packet> BackendServer::process_respond_packet() {
    if(this->is_queue_empty())
        return nullptr;

    shared_ptr<Packet> processed_packet = this->pop_packet();
    shared_ptr<Packet> returned_packet = nullptr;
    assert(processed_packet != nullptr);

    log("backend_server", RECEIVE, this->server_id, "sequence number: " + processed_packet->get_payload() + " client_ip: " + processed_packet->get_source_ip().to_string());

    if(!processed_packet->is_short_header()) {
        returned_packet = this->long_header_response(processed_packet);
    } else {
        returned_packet = this->short_header_response(processed_packet);
    }

    log("backend_server", SEND, this->server_id, "sequence number: " + processed_packet->get_payload() + " client_ip: " + processed_packet->get_source_ip().to_string());
    return returned_packet;
}