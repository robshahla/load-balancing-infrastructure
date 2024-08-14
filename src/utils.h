//
// Created by shahlarobert on 31/10/2022.
//

#ifndef SCHEME_IMPLEMENTATION_UTILS_H
#define SCHEME_IMPLEMENTATION_UTILS_H

#include <iostream>
#include <cassert>
#include "config.h"

class Metrics {
    int iteration; //iteration of the logging
    string middlebox_name;
    int queue_size;
    int probe_rate_ms;
    int queue_max_size;
    int active_clients;

public:
    Metrics() = default;
    Metrics(int iteration, const string &middleboxName, int queueSize, int probe_rate_ms, int queue_max_size,
            int active_clients);
    string get_first_line_csv();
    string get_metrics_csv();
};

void log(string type, string process, int id, string message);

/* log_file: receives a set of metrics, and logs these metrics
 * to a file with name `output_file`. All is done in a csv format.
 * If `output_file` does not exist, then it will be created.*/
void log_file(string output_file, Metrics metrics);


#endif //SCHEME_IMPLEMENTATION_UTILS_H
