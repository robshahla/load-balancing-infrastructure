//
// Created by shahlarobert on 02/06/2022.
//

#include "packet.h"
#include <string>
#include <iostream>

IpAddr::IpAddr(int b1, int b2, int b3, int b4): b1(b1), b2(b2), b3(b3), b4(b4) {}

string IpAddr::to_string() const {
    return std::to_string(b1) + std::to_string(b2) + std::to_string(b3) + std::to_string(b4);
}


Packet::Packet(const IpAddr source_ip, const IpAddr destination_ip, const int source_port, const int destination_port,
       bool header_form, const string& payload): source_ip(source_ip), destination_ip(destination_ip),
                                                 source_port(source_port), destination_port(destination_port), header_form(header_form), payload(payload) {}

bool Packet::is_short_header() {
    return this->header_form == SHORT_HEADER;
}

uint64_t Packet::get_dcid() {
    return 0;
}

uint64_t Packet::get_scid() {
    return 0;
}

const IpAddr &Packet::get_source_ip() const {
    return source_ip;
}

const IpAddr &Packet::get_destination_ip() const {
    return destination_ip;
}

int Packet::get_source_port() const {
    return source_port;
}

int Packet::get_destination_port() const {
    return destination_port;
}

const string &Packet::get_payload() const {
    return payload;
}

void Packet::setPayload(const string &payload) {
    Packet::payload = payload;
}

LongHeaderPacket::LongHeaderPacket(const IpAddr source_ip, const IpAddr destination_ip, const int source_port,
                 const int destination_port, const uint64_t source_cid, const uint64_t destination_cid,
                 const string& payload):
        Packet(source_ip, destination_ip, source_port, destination_port, LONG_HEADER, payload),
        source_cid(source_cid), destination_cid(destination_cid){}


uint64_t LongHeaderPacket::get_dcid() {
//    std::cout << "LongHeaderPacket::get_dcid: " << this->destination_cid << std::endl; // TODO: remove
    return this->destination_cid;
}

uint64_t LongHeaderPacket::get_scid() {
    return this->source_cid;
}

ShortHeaderPacket::ShortHeaderPacket(const IpAddr source_ip, const IpAddr destination_ip, const int source_port,
                  const int destination_port, const uint64_t destination_cid, const string& payload):
        Packet(source_ip, destination_ip, source_port, destination_port, SHORT_HEADER, payload),
        destination_cid(destination_cid){}

uint64_t ShortHeaderPacket::get_dcid() {
//    std::cout << "ShortHeaderPacket::get_dcid: " << this->destination_cid << std::endl; // TODO: remove
    return this->destination_cid;
}

uint64_t ShortHeaderPacket::get_scid() {
    return 0;
}


CID::CID(int server_id, int nonce, int phase): server_id(server_id), nonce(nonce), phase(phase) {}

CID::CID(uint64_t encrypted_cid) {
    uint64_t decrypted_value = encrypted_cid ^ encryption_private_key;
//    std::cout << "decrypt" << std::endl; // TODO: remove
//    std::cout << decrypted_value << std::endl; // TODO: remove
    server_id = ((decrypted_value >> 56));
    nonce = ((decrypted_value >> 24) & 0xffffffff);
    phase = (decrypted_value & 0xffffff);
    std::cout << "decrypt" << std::endl; // TODO: remove
    std::cout << server_id << "|" << nonce << "|" << phase; // TODO: remove
}

uint64_t CID::encrypt_cid() {
    std::cout << "encrypt" << std::endl; // TODO: remove
    std::cout << server_id << "|" << nonce << "|" << phase << std::endl; // TODO: remove
    uint64_t value = server_id;
    value = (value << 32) | nonce;
    value = (value << 24) | phase;
//    std::cout << "encrypt" << std::endl; // TODO: remove
//    std::cout << (value ^ encryption_private_key) << std::endl; // TODO: remove
    return value ^ encryption_private_key;
}

int CID::get_server_id() const {
    return server_id;
}

int CID::get_nonce() const {
    return nonce;
}

int CID::get_phase() const {
    return phase;
}

Payload::Payload(int sequenceNumber, int taskDuration) : sequence_number(sequenceNumber), task_duration(taskDuration) {}

int Payload::getSequenceNumber() const {
    return sequence_number;
}

int Payload::getTaskDuration() const {
    return task_duration;
}
