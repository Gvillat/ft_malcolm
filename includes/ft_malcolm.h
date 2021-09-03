#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

#include <signal.h>
#include <netdb.h> //gethostbyname
#include <stdio.h>
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h> //getifadddr freeifaddr
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

/*

      printf("htype [%0X]\n\
ptype [%0X]\n\
hlen [%0X]\n\
plen [%0X]\n\
opcode [%0X]\n\
source_mac [%s]\n\
source_ip [%s]\n\
target_mac [%s]\n\
target_ip [%s]\n",
arp_resp->htype,
arp_resp->ptype,
arp_resp->hlen,
arp_resp->plen,
arp_resp->opcode,
(char*)arp_resp->source_mac,
arp_resp->source_ip,
(char*)arp_resp->target_mac,
arp_resp->target_ip);

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

extern volatile sig_atomic_t g_sigint;

typedef struct s_arp_hdr {
  unsigned short    htype;
  unsigned short    ptype;
  unsigned char     hlen;
  unsigned char     plen;
  unsigned short    opcode;
  unsigned char     source_mac[6];
  unsigned char     source_ip[4];
  unsigned char     target_mac[6];
  unsigned char     target_ip[4];
}   t_myarp;

typedef struct s_malcolm
{
	char                     *ip_saddr;
	in_addr_t                ips;
	char                     *mac_saddr;
	char                     *ip_daddr;
	in_addr_t 		         ipd;
	char                     *mac_daddr;
    uint8_t                  mac_laddr[6];
    uint8_t                  ip_laddr[4];
	int                      socket;
	struct hostent           hostname;
	struct ifaddrs           *ifap;
  t_myarp                    arphdr;
}				t_malcolm;


// int check_ipv4_format(char *arg);
// int check_macaddr_format(char *arg);
void sig_handler(int sig);
void init_n_fill_mlcml(const char **av, t_malcolm *mlcml);
void malcolm_usage(char *error, char *str);
void letsgo_mitm(t_malcolm *mlcml, struct sockaddr_ll *s);

#endif