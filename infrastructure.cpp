//
// Created by shahlarobert on 03/06/2022.
//

#include "infrastructure.h"
#include <thread>

using std::thread;

/*get_ip_address: Given a base IP address and an index, the function allocates a new IP address and
 * returns it. (max supported 255 IP addresses).*/
static IpAddr get_ip_address(IpAddr base_ip, int index) {
    IpAddr current_ip = base_ip;
    current_ip.b4 += index;
    return current_ip;
}

static int get_backend_server_port() {
    return BASE_BACKEND_SERVER_PORT;
}


static int get_client_port() {
    return BASE_CLIENT_PORT;
}

/* get_client_from_ip: Given an IP address of a client, returns the index of the client.*/
static int get_client_from_ip(IpAddr client_ip) {
    return client_ip.b4 - BASE_CLIENT_IP.b4;
}

Infrastructure::Infrastructure(): router(ROUTER_QUEUE_SIZE), load_balancers(NUM_OF_LBS),
backend_servers(NUM_OF_BACKEND_SERVERS), clients(NUM_OF_CLIENTS){

    for(int i = 0; i < NUM_OF_LBS; ++i) {
        load_balancers[i].init(i);
    }

    for(int i = 0; i < NUM_OF_BACKEND_SERVERS; ++i) {
        backend_servers[i].init(i, get_ip_address(BASE_BACKEND_SERVER_IP, i), get_backend_server_port());
    }

    for(int i = 0; i < NUM_OF_CLIENTS; ++i) {
        clients[i] = Client(i, get_ip_address(BASE_CLIENT_IP, i), get_client_port());
    }
}


void Infrastructure::clients_send_packets() {
    srand(RANDOM_SEED);
    int counter = 0;
    while(counter < NUM_OF_CLIENT_PACKETS) {
        int current_client = rand() % NUM_OF_CLIENTS;
        shared_ptr<Packet> packet_to_send = clients[current_client].generate_send_packet();
        if(packet_to_send == nullptr) {
            std::this_thread::sleep_for(std::chrono::milliseconds(CLIENT_RESPONSE_WAIT_MS));
            continue;
        }
        router.push_packet(packet_to_send);
        counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(ROUTER_INPUT_RATE_MS));
    }
}

void Infrastructure::route_to_lbs() {
    while(1) {
        if(router.is_queue_empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(ROUTER_OUTPUT_RATE_MS));
            continue;
        }
        shared_ptr<Packet> packet;
        int lb_index = router.routing_decision(&packet);
        assert(lb_index != EMPTY_QUEUE);
        load_balancers[lb_index].push_packet(packet);
    }
}

void Infrastructure::lb_process(int index) {
    while(1) {
        if(load_balancers[index].is_queue_empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(LB_OUTPUT_RATE_MS));
            continue;
        }
        shared_ptr<Packet> packet;
        int backend_server_index = load_balancers[index].routing_decision(&packet);
        if(backend_server_index == DROP_PACKET)
            continue;
        assert(backend_server_index != EMPTY_QUEUE);
        backend_servers[backend_server_index].push_packet(packet);
    }
}

void Infrastructure::backend_server_process(int index) {
    while(1) {
        if(backend_servers[index].is_queue_empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(BACKEND_SERVER_OUTPUT_RATE_MS));
            continue;
        }

        shared_ptr<Packet> response = backend_servers[index].process_respond_packet();
        assert(response != nullptr);
        int client_index = get_client_from_ip(response->get_destination_ip());
        clients[client_index].receive_packet(response);
    }
}

void Infrastructure::run() {
    thread clients_thread(&Infrastructure::clients_send_packets, this);
    thread router_thread(&Infrastructure::route_to_lbs, this);
    thread load_balancers_threads[NUM_OF_LBS];
    thread backend_servers_threads[NUM_OF_BACKEND_SERVERS];

    // starts LB threads:
    for(int i = 0; i < NUM_OF_LBS; ++i) {
        load_balancers_threads[i] = thread(&Infrastructure::lb_process, this, i);
    }

    // starts backend servers threads:
    for(int i = 0; i < NUM_OF_BACKEND_SERVERS; ++i) {
        backend_servers_threads[i] = thread(&Infrastructure::backend_server_process, this, i);
    }

    clients_thread.join();
    router_thread.join();

    // join LB threads
    for(int i = 0; i < NUM_OF_LBS; ++i) {
        load_balancers_threads[i].join();
    }

    // join backend servers threads
    for(int i = 0; i < NUM_OF_BACKEND_SERVERS; ++i) {
        load_balancers_threads[i].join();
    }
}


