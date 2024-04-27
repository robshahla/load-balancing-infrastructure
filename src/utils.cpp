//
// Created by shahlarobert on 31/10/2022.
//

#include <fstream>
#include "utils.h"
#include "config.h"

using std::ofstream;
using std::filesystem::exists;



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

void log_file(string output_file, Metrics metrics) {
    bool log_first_line = false;
    if(!exists(output_file))
        // in this case the first line of the csv should be inserted
        log_first_line = true;

    ofstream out_file;
    out_file.open(output_file, std::ios_base::app);
    if(log_first_line) {
        Metrics metrics;
        string first_line = metrics.get_first_line_csv();
        out_file << first_line << std::endl;
    }
    out_file << metrics.get_metrics_csv() << std::endl;
    out_file.close();
}

Metrics::Metrics(int iteration, const string &middleboxName, int queueSize, int probe_rate_ms, int queue_max_size,
                 int active_clients) : iteration(iteration),
                                                                               middlebox_name(middleboxName),
                                                                               queue_size(queueSize),
                                                                               probe_rate_ms(probe_rate_ms),
                                                                               queue_max_size(queue_max_size),
                                                                               active_clients(active_clients){}

string Metrics::get_first_line_csv() {
    return "iteration, middlebox_name, get_queue_size (packets), probe_rate_ms, queue_max_size (packets), active_clients";
}

string Metrics::get_metrics_csv() {
    return std::to_string(iteration) + ", " +
            middlebox_name + ", " +
            std::to_string(queue_size) + ", " +
            std::to_string(probe_rate_ms) + ", " +
            std::to_string(queue_max_size) + ", " +
            std::to_string(active_clients);
}
