#Ho Seok Lee, 1004112177, hoseok.lee@mail.utoronto.ca

Part 1:

    dig utoronto.ca (NS/MX)

- follow up dig commands for mail exchange server to retrieve with IPv4 addresses
  by putting NS or MX
- command for digging BIND server: 

    dig @127.0.0.1 -p <NAMED port number> utoronto.ca (NS/MX)

- the only difference between normal dig and dig with the local BIND server is the
  base server IPv4 address for utoronto.ca (base server address becomes the address
  of the BIND server), but other addresses such as name and mail exchange servers
  remain the same

Part 2:

    python2 dnsproxy_starter.py --port <NAMED port number> --dns_port <QUERY port number>
    ./run_bind.sh
    dig @127.0.0.1 -p <NAMED port number> utoronto.ca (NS/MX)

- after binding with the server at 127.0.0.1, the socket fowards received packets 
  from the DNS server to the BIND server, then forward the reply from the BIND
  server back to the DNS, using the scapy.DNS() function

Part 3:

    python2 dnsproxy_starter.py --port <NAMED port number> --dns_port <QUERY port number> --spoof_response
    ./run_bind.sh
    dig @127.0.0.1 -p <NAMED port number> utoronto.ca (NS/MX)

- after retrieving the responses from the DNS server, spoof the response with scapy
  such that the IPv4 address is "1.2.3.4" and the two name servers are "ns.dnslabattacker.net"