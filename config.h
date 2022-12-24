//
// Created by shahlarobert on 03/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_CONFIG_H
#define SCHEME_IMPLEMENTATION_CONFIG_H

#include "packet.h"
#include <iostream>
#include <filesystem>

using std::shared_ptr;

#define DROP_PACKET (-2)
#define EMPTY_QUEUE (-1)

const int INITIAL_NUM_OF_CLIENTS = 100;
const int MAX_NUM_OF_CLIENTS = 1000;
const int NUM_OF_LBS = 1;
const int NUM_OF_BACKEND_SERVERS = 2;


const int ROUTER_INPUT_RATE_MS = 200;
const int ROUTER_OUTPUT_RATE_MS = 500;
const int LB_OUTPUT_RATE_MS = 500;
const int BACKEND_SERVER_OUTPUT_RATE_MS = 500;
const int CLIENT_RESPONSE_WAIT_MS = 1000; // number of ms a client would wait before checking if a response was received from the server

const int ROUTER_QUEUE_SIZE = 1000;
const int LB_QUEUE_SIZE = 1000;
const int SERVER_QUEUE_SIZE = 1000;

const int RANDOM_SEED = 0;
const int INITIAL_SEQUENCE_NUMBER = 0;
const int INITIAL_NONCE = 0;
const int INITIAL_PHASE = 0;
const long CLIENT_CID = 1;
const int NUM_OF_CLIENT_PACKETS = 1000;

const IpAddr BASE_CLIENT_IP = {1,1,1,1};
const IpAddr BASE_BACKEND_SERVER_IP = {100,100,100,1};
const int BASE_CLIENT_PORT = 80;
const int BASE_BACKEND_SERVER_PORT = 80;
const IpAddr VIP = {8,8,8,8}; // Virtual IP
const int VPORT = 80; // Virtual Port

const bool logging = true;

const string RESULTS_OUTPUT_FILE = "./results.csv";
const int PROBE_RATE_MS = 2500;

// strings that explain the process that is being done by a middle box
const string SEND = "SEND";
const string RECEIVE = "RECEIVE";
const string DROP = "DROP";



#endif //SCHEME_IMPLEMENTATION_CONFIG_H
