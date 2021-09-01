#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

#include <signal.h>
#include <netdb.h> //gethostbyname
#include <stdio.h>
#include "libft.h"
#include <stdlib.h>
#include <sys/types.h>
#include <ifaddrs.h> //getifadddr freeifaddr
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
◦ sendto, recvfrom
◦ socket, setsockopt
◦ getuid, close, signal
◦ inet_addr
◦ gethostbyname, getifaddrs, freeifaddrs
◦ htons, ntohs`

Found available interface: eth0
An ARP request has been broadcast.
mac address of request: 10:dd:**:**:**:**
IP address of request: 10:12:12:07
Now sending an ARP reply to the target address with spoofed source, please wait...
Sent an ARP reply packet, you may now check the arp table on the target.
Exiting program...

sudo ./ft_malcolm 10.11.11.11 aa:bb:cc:dd:ee:ff 10.11.11.1111 aa:
bb:cc:dd:ee:ff
ft_malcolm: unknown host or invalid IP address: (10.11.11.1111).


struct hostent {
   char  *h_name;             official name of host 
   char **h_aliases;          alias list 
   int    h_addrtype;         host address type 
   int    h_length;           length of address 
   char **h_addr_list;        list of addresses 
}
*/



typedef struct s_malcolm
{
	char 				*ip_saddr;
	in_addr_t 		ips;
	char 				*mac_saddr;
	char 				*ip_daddr;
	in_addr_t 		ipd;
	char 				*mac_daddr;
	int 				socket;
	struct hostent hostname;
	struct ifaddrs *ifap;
  volatile sig_atomic_t sigint;
}				t_malcolm;

typedef struct s_arp_hdr {
  uint16_t htype;
  uint16_t ptype;
  uint8_t hlen;
  uint8_t plen;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
}		t_myarp;

// int check_ipv4_format(char *arg);
// int check_macaddr_format(char *arg);
void sig_handler(int sig);
void init_n_fill_mlcml(const char **av, t_malcolm *mlcml);
void malcolm_usage(char *error, char *str);

#endif