//
// Created by shahlarobert on 04/06/2022.
//
#include "config.h"

void log(string type, string process, int id, string message) {
    if(type == "router")
        std::cout << "\t\t\t\t\t\t\t\t\t";
    if(type == "LB")
        std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
    if(type == "backend_server")
        std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
    if(logging) {
        std::cout << type << " " << id << " - " << "process: " << process << " ----- " << message << std::endl;
    }
}