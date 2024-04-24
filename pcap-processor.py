import pyshark
import json
import sys
import csv
from ast import literal_eval as make_tuple


# returns two options of 4-tuple of the given packet, the two options differ in the source and destination order
def get_four_tuple(packet):
    src_tuple = get_tuple_string(packet.ip.src, packet.tcp.srcport)
    dst_tuple = get_tuple_string(packet.ip.dst, packet.tcp.dstport)
    return (src_tuple, dst_tuple), (dst_tuple, src_tuple)


def get_tuple_string(ip, port):
    return str(ip) + ":" + str(port)


# appends an entry to the given dictionary, with the given parameters
def append_to_dict(connections: dict, src_dst_tuple, dst_src_tuple, timestamp, token, initial):
    if (src_dst_tuple, dst_src_tuple) in connections.keys():
        connections[(src_dst_tuple, dst_src_tuple)] += [",".join([timestamp, str(src_dst_tuple), token, initial])]
    elif (dst_src_tuple, src_dst_tuple) in connections.keys():
        connections[(dst_src_tuple, src_dst_tuple)] += [",".join([timestamp, str(src_dst_tuple), token, initial])]
    else:
        connections[(src_dst_tuple, dst_src_tuple)] = [",".join([timestamp, str(src_dst_tuple), token, initial])]


# build a dictionary that contains as keys the IP + Port tuple of both endpoints of a connection.
# the values are timestamp, source IP + Port, destination IP + Port, bool=true if token is available,
def quic_clients_dict(pcap_data, out_file):
    connections = {}
    for packet in pcap_data:
        layers = [layer.layer_name for layer in packet]
        if "quic" in layers and "udp" in layers:
            timestamp = packet.sniff_timestamp
            src_dst_tuple, dst_src_tuple = get_four_tuple(packet)
            initial = "initial" if int(packet.quic.header_form) == 1 and int(packet.quic.long_packet_type) == 0 else "non_initial"
            token = "token_available" if initial == "initial" and int(packet.quic.token_length) != 0 else "no_token_available"
            append_to_dict(connections,
                           src_dst_tuple,
                           dst_src_tuple,
                           timestamp,
                           token,
                           initial)

    with open(out_file, 'w') as f:
        json.dump({str(k): v for k, v in connections.items()}, f, indent=4)


# build a dictionary that contains as keys the IP + Port tuple of both endpoints of a connection.
# the values are timestamp, source IP + Port, destination IP + Port, bool=true if token is available,
def tcp_clients_dict(pcap_data, out_file):
    connections = {}
    count = 0
    for packet in pcap_data:
        layers = [layer.layer_name for layer in packet]
        if "tcp" in layers and not "ipv6" in layers:
            timestamp = packet.sniff_timestamp
            src_dst_tuple, dst_src_tuple = get_four_tuple(packet)
            syn = "syn" if int(packet.tcp.flags_syn) == 1 else "non_syn"
            ack = "ack" if int(packet.tcp.flags_ack) == 1 else "non_ack"
            append_to_dict(connections,
                           src_dst_tuple,
                           dst_src_tuple,
                           timestamp,
                           syn,
                           ack)
        count += 1
        if count % 1000 == 0:
            print(count)

    with open(out_file, 'w') as f:
        json.dump({str(k): v for k, v in connections.items()}, f, indent=4)


# TODO: continue using scapy instead of pyshark
# def tcp_clients_dict_scapy(pcap_flow, out_file):
#
#     sessions = pcap_flow.sessions()
#     for session in sessions:
#         for packet in sessions[session]:
#             try:
#                 if packet[TCP].dport == 80:
#                     payload = bytes(packet[TCP].payload)
#                     url = get_url_from_payload(payload)
#                     urls_output.write(url.encode())
#             except Exception as e:
#                 pass
#     urls_output.close()


def key_in_aggregated(aggregate_dict, key_tuple):
    """checks whether the given key_tuple is in the aggregate_dict, if so,
    the key present in the aggregate_dict will be returned, otherwise `None` will
    be returned"""
    if str(key_tuple) in aggregate_dict:
        return str(key_tuple)

    if str((key_tuple[1], key_tuple[0])) in aggregate_dict:
        return str(key_tuple[1], key_tuple[0])

    return None


def add_to_aggregated_dict(aggregate_dict: dict, connections_dict: dict):
    """adds the connection in `connections_dict` to `aggregate_dict`"""
    counter = 0
    for key, value in connections_dict.items():
        key_tuple = make_tuple(key)
        key_in = key_in_aggregated(aggregate_dict, key_tuple)
        if key_in is not None:
            aggregate_dict[key_in] += value

        else:
            aggregate_dict[str(key_tuple)] = value
        counter += 1
        if counter % 1000 == 0:
            print(f"number of analyzed keys: {counter}")


def aggregate_json_files(file_base_name, input_files):
    """receives a list of files and aggregates their contents into one large json file"""
    aggregate_dict = {}

    for file in input_files:
        with open(file, 'r') as f:
            print(f"working on file: {file}")
            connections_dict = json.load(f)
            add_to_aggregated_dict(aggregate_dict, connections_dict)

    out_file = file_base_name + "aggregated.json"
    with open(out_file, 'w') as f:
        json.dump({str(k): v for k, v in aggregate_dict.items()}, f, indent=4)


def pcap_to_json(filename):
    """receives a pcap file, and builds a relevant json from it"""
    pcap_data = pyshark.FileCapture(filename)
    outfile = "tcp-processed-data-" + filename.split(".")[4] + ".json"
    print(outfile)
    tcp_clients_dict(pcap_data, outfile)


def calculate_statistics(filename):
    """calculate some statistics on the given file"""
    with open(filename, 'r') as f:
        connections_dict = json.load(f)
        print("number of keys: ", len(connections_dict.keys()))
        ip_port_set = set()
        for k in connections_dict.keys():
            ip_port_set.add((make_tuple(k)[0])[0])
            ip_port_set.add((make_tuple(k)[0])[1])
        print("len(ip_port_set): ", len(ip_port_set))
        # print(ip_port_set)
        ip_set = set()
        for p in ip_port_set:
            ip_set.add(p.split(':')[0])
        print("len(ip_set): ", len(ip_set))
        # print(ip_set)


def to_per_client_dict(per_client_dict: dict, connections_dict: dict):
    """builds the per_client_dict from the connections_dict"""

    counter = 0
    for key, value in connections_dict.items():
        first_key_tuple = make_tuple(key)[0]
        ip_addresses = tuple(sorted((first_key_tuple[0].split(":")[0], first_key_tuple[1].split(":")[0])))
        if ip_addresses in per_client_dict:
            per_client_dict[ip_addresses] += value
        else:
            per_client_dict[ip_addresses] = value
        counter += 1
        if counter % 1000 == 0:
            print(f"number of connections processed: {counter}")


def per_client_connections_file(filename, out_file):
    """given a json file, create a new json file, that the keys are only IP addresses
    without the ports, the ports however are written in values."""
    per_client_dict = {}
    with open(filename, 'r') as f:
        print(f"working on file: {filename}")
        connections_dict = json.load(f)
        to_per_client_dict(per_client_dict, connections_dict)

    with open(out_file, 'w') as f:
        json.dump({str(k): v for k, v in per_client_dict.items()}, f, indent=4)


def extract_values(row: str):
    """receives a string row, and returns a list that includes the fields in the row"""
    comma_seperated = row.split(",")
    timestamp = comma_seperated[0]
    src_ip = comma_seperated[1].split(":")[0][2:]
    src_port = comma_seperated[1].split(":")[1][:-1]
    dest_ip = comma_seperated[2].split(":")[0][2:]
    dest_port = comma_seperated[2].split(":")[1][:-2]
    syn = comma_seperated[3]
    ack = comma_seperated[4]
    return [timestamp, src_ip, src_port, dest_ip, dest_port, syn, ack]


def construct_csv_line(packet_fields: list, additional_fields: list):
    """returns a list that is the concatenation of both given lists"""
    return packet_fields + additional_fields


def get_connection_mean_serve_time(packets: list):
    """given the packets list of a connection, calculate the
    expected request time. This is a hard task, as in
    our trace for each connection we only have the
    packets in a single direction.
    Our heuristic for the serve time is to take the average
    of the difference between two consecutive packets."""
    if len(packets) == 1:
        return 0

    timestamps = [float(extract_values(packet)[0]) for packet in packets]
    differences = [p2 - p1 for p1, p2 in zip(timestamps[:-1], timestamps[1:])]
    return sum(differences) / len(differences)


def json_file_to_csv(json_file, csv_file):
    """convert a given json file to csv,
    timestamps will be sorted in ascending order"""
    header = ['timestamp', 'src_ip', 'src_port', 'dest_ip', 'dest_port', 'syn/fin', 'ack', 'serve_time']
    data = []
    counter = 0
    with open(json_file, 'r') as f:
        print(f"working on file: {json_file}")
        connections_dict = json.load(f)
        for value in connections_dict.values():

            serve_time = get_connection_mean_serve_time(value)

            # we take the first request and make it a 'syn' packet if it's not,
            # because our pcap includes connections that were opened before the sniff start
            first_packet = construct_csv_line(extract_values(value[0]), [serve_time])
            first_packet[5] = 'syn'
            data += [first_packet]

            data += [construct_csv_line(extract_values(v), [serve_time]) for v in value[1:-1]]

            # if the connection contains at least
            # 2 packets, then the last packet of
            # the connection would be the FIN packet
            if len(value) >= 2:
                last_packet = construct_csv_line(extract_values(value[-1]), [serve_time])
                last_packet[5] = 'fin'
                data += [last_packet]

            counter += 1
            if counter % 1000 == 0:
                print(f"counter: {counter}")

    data.sort(key=lambda x: x[0])
    print("writing to csv")
    with open(csv_file, 'w', encoding='UTF8', newline='') as f:
        writer = csv.writer(f)

        # write the header
        writer.writerow(header)

        # write multiple rows
        writer.writerows(data)


def check_number_of_resumption_per_connection(json_file):
    with open(json_file, 'r') as f:
        print(f"working on file: {json_file}")
        connections_dict = json.load(f)
        for value in connections_dict.values():
            data = []

            first_packet_syn = 'syn'
            data += [first_packet_syn]

            data += [extract_values(v)[5] for v in value[1:]]
            count = data.count('syn')
            if count > 1:
                print(count, extract_values(value[0]))


def check_ascending_timestamps(filename):
    """check if the timestamps in the given file are in ascending order"""
    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        prev_timestamp = 0
        port_set = set()
        for row in csv_reader:
            if line_count == 0:
                print(f'Column names are {", ".join(row)}')
                line_count += 1
                continue
            elif line_count == 1:
                prev_timestamp = float(row[0])

            else:
                if float(row[0]) < prev_timestamp:
                    print(f"error with timestamps: {row[0]} and {prev_timestamp}")
                    return
                prev_timestamp = float(row[0])

            line_count += 1
            port_set.add(int(row[2]))

            print(line_count)
        return port_set


def list_src_ports(filename):
    """given a csv filename, return the list of the src ports used by packets in the file"""
    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        port_set = set()
        for row in csv_reader:
            if line_count == 0:
                print(f'Column names are {", ".join(row)}')
                line_count += 1
                continue

            line_count += 1
            port_set.add(int(row[2]))
        return port_set


def main():
    # given a pcap file, create a json file
    # pcap_filename = sys.argv[1]
    # pcap_to_json(pcap_filename)

    # given json files, create an aggregated json file
    # file_base_name = "tcp-processed-data-"
    # input_json_files = [f"{file_base_name}anon{i}.json" for i in range(1, 23)]
    # aggregate_json_files(file_base_name, input_json_files)

    # calculate some statistics
    # filename = "tcp-processed-data-aggregated.json"
    # calculate_statistics(filename)

    # filename = "tcp-processed-data-aggregated.json"
    # out_file = "tcp-processed-data-per-client.json"
    # per_client_connections_file(filename, out_file)

    json_file = "tcp-processed-data-aggregated.json"
    csv_file = "tcp-processed-data.csv"
    # json_file_to_csv(json_file, csv_file)

    # port_set = check_ascending_timestamps(csv_file)
    # print(sorted(list(port_set)))

    check_number_of_resumption_per_connection(json_file)


if __name__ == "__main__":
    main()


