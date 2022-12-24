import pyshark


def main():
    filename = "../pcap-data/DATA/ALLOT/YouTube/1667395874.0_video_qoe_1_R9JNA0DNB9J_2022-11-02_15-31-14_00000.pcap"
    # filename = "../pcap-data/201805161400.pcap"

    pcap_data = pyshark.FileCapture(filename)
    count = 0
    for packet in pcap_data:
        for layer in packet:
            if layer.layer_name == 'quic':
                if int(layer.header_form) == 1 and int(layer.long_packet_type) == 0:
                    print(layer.token_length)

    print(count)


if __name__ == "__main__":
    main()
