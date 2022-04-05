#Ho Seok Lee, 1004112177, hoseok.lee@mail.utoronto.ca

Part 1:
- command used: 
    dig utoronto.ca (NS/MX) for name or mail exchange server
- follow up dig commands for mail exchange server to retrieve with IPv4 addresses
- command for digging local BIND server: 
    dig @127.0.0.1 -p <NAMED port number> utoronto.ca (NS/MX)
- the only difference between normal dig and dig with the local BIND server is the
  base server IPv4 address for utoronto.ca (base server address becomes the address
  of the BIND server), but other addresses such as name and mail exchange servers
  remain the same