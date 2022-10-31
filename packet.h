//
// Created by shahlarobert on 02/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_PACKET_H
#define SCHEME_IMPLEMENTATION_PACKET_H

#include <string>
#include <iostream>
#define LONG_HEADER 1
#define SHORT_HEADER 0

using std::string;

class IpAddr {
public:
    int b1, b2, b3, b4;
    IpAddr() = default;
    IpAddr(int b1, int b2, int b3, int b4);
    string to_string() const;
};

/* A class that represents the payload that is attached with each packet. */
class Payload {
    int sequence_number;
    int task_duration;

public:
    Payload() = default;
    Payload(int sequenceNumber, int taskDuration);
    int get_sequence_number() const;
    int get_task_duration() const;
    string serialize() const;
};


class Packet {
    IpAddr source_ip;
    IpAddr destination_ip;
    int source_port;
    int destination_port;
    bool header_form;
    Payload payload;
public:
    Packet() = default;
    Packet(const IpAddr source_ip, const IpAddr destination_ip, const int source_port, const int destination_port,
           bool header_form, Payload payload);
    bool is_short_header();
    virtual uint64_t get_dcid();
    virtual uint64_t get_scid();
    const IpAddr &get_source_ip() const;
    const IpAddr &get_destination_ip() const;
    int get_source_port() const;
    int get_destination_port() const;
    const Payload &get_payload() const;
    void setPayload(Payload payload);
};

class LongHeaderPacket : public Packet {
    uint64_t source_cid;
    uint64_t destination_cid;
public:
    LongHeaderPacket() = default;
    LongHeaderPacket(const IpAddr source_ip, const IpAddr destination_ip, const int source_port,
                     const int destination_port, const uint64_t source_cid, const uint64_t destination_cid,
                     Payload payload);

    uint64_t get_dcid() override;
    uint64_t get_scid() override;
};

class ShortHeaderPacket : public Packet {
    uint64_t destination_cid;
public:
    ShortHeaderPacket(const IpAddr source_ip, const IpAddr destination_ip, const int source_port,
                      const int destination_port, const uint64_t destination_cid, Payload payload);

    uint64_t get_dcid() override;
    uint64_t get_scid() override;
};

const uint64_t encryption_private_key = 0x0;

/* For now, we don't use encryption and decryption, we only use a XOR with a private key.
 * The long is an 8 byte number. we use 8 bits for server_id, 32 bits for nonce and 24 bits for phase. */
class CID {
    uint server_id;
    uint nonce;
    uint phase;

public:
    CID(int server_id, int nonce, int phase);

    // A constructor that build a CID from an encrypted CID of type long.
    CID(uint64_t encrypted_cid);
    uint64_t encrypt_cid();
    int get_server_id() const;
    int get_nonce() const;
    int get_phase() const;
};

#endif //SCHEME_IMPLEMENTATION_PACKET_H
