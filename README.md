# QUIC Load Balancing Prototype
This project is an implementation of layer4 load balancing infrastructure used as a base for a load balancing prototype. It's main goal to provide a prototype for QUIC load balancing using routable CIDs as discussed in [QUIC-LB](https://datatracker.ietf.org/doc/html/draft-ietf-quic-load-balancers).


## Overview
The implementation uses multi-threading, and consists of clients, a router, load balancers, and backend servers. The clients send QUIC packets to the router, which forwards them to the load balancers. The load balancers then forward the packets to the backend servers. The backend servers respond to the clients via direct server return (DSR) mode.

A single thread is used for the clients, and another thread is used for the router. A thread for each load balancer and backend server is also used. The load balancing policy used for now is a four-tuple hash-based policy. Each of the router, load balancers, and backend servers has a packet queue to store received unprocessed packets.
An additional thread is used for monitoring the whole system. The monitoring thread is responsible for probing router, load balancers, and backend servers and log their queue sizes.

In the payload of each packet, a `task_duration` field is included. This field is used to simulate the processing time of the backend server. When the backend server receives a packet, it sleeps for the duration specified in the `task_duration` field before responding to the client.

## Development
Clone the repository:
```bash
git clone git@github.com:robshahla/load-balancing-infrastructure.git
```

Modify the `config.h` file to set the configuration paramaters such as the number of load balancers, backend servers, queue sizes, etc.


