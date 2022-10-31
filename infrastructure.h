//
// Created by shahlarobert on 03/06/2022.
//

#ifndef SCHEME_IMPLEMENTATION_INFRASTRUCTURE_H
#define SCHEME_IMPLEMENTATION_INFRASTRUCTURE_H

#include "router.h"
#include "load_balancer.h"
#include "backend_server.h"
#include "client.h"
#include <vector>

using std::vector;

/* The infrastructure class implements the infrastructure in the datacenter,
 * which includes a router as a gateway, a layer of load balancers that receive
 * the packets from the gateway router, and a layer of backend servers that receive
 * the packets from the load balancers. the backend servers return the responses to
 * the clients through the router, bypassing the load balancers layer. The payload of
 * the returned packets include the incremented sequence number that was received in
 * the packet from the client.*/
class Infrastructure {
    Router router;
    vector<LoadBalancer> load_balancers;
    vector<BackendServer> backend_servers;
    vector<Client> clients;
public:
    Infrastructure();

    /* run: A method that runs all of the threads that will
     * be working in the infrastructure.*/
    void run();

    /* clients_send_packets: Send packets from the clients
     * by randomly selecting a client each time interval and inserting the packet in the
     * router queue.*/
    void clients_send_packets();

    /* route_to_lbs: Route packets from the router packet queue into one of the LBs*/
    void route_to_lbs();

    /* lb_process: Processes a packet from LB in the given index and forward
     * it to the correct backend server.*/
    void lb_process(int index);

    /* backend_server_process: Processes a packet from the backend server
     * in the given index and responds with the relevant response to the
     * client.*/
    void backend_server_process(int index);
};


#endif //SCHEME_IMPLEMENTATION_INFRASTRUCTURE_H
