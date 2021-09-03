#include "ft_malcolm.h"


volatile sig_atomic_t g_sigint;


void malcolm_usage(char *error, char *str)
{
	dprintf(2,"ft_malcolm: %s : %s\n", error, str);
	exit(42);
}

void debug_malcolm(t_malcolm *m)
{
	printf("\nSource  IP {%s}\nSource  MAC {%s}\nDest    IP {%s}\nDest   MAC {%s}\nLocal    IP {%d.%d.%d.%d}\nLocal   MAC {%0x:%0x:%0x:%0x:%0x:%0x}\n", m->ip_saddr, m->mac_saddr, m->ip_daddr, m->mac_daddr, m->ip_laddr[0], m->ip_laddr[1], m->ip_laddr[2], m->ip_laddr[3], m->mac_laddr[0], m->mac_laddr[1], m->mac_laddr[2], m->mac_laddr[3], m->mac_laddr[4], m->mac_laddr[5]);
	printf("ARP HEADER\nsource mac [%0x:%0x:%0x:%0x:%0x:%0x]\nSource ip [%d.%d.%d.%d]\ntarget mac [%0x:%0x:%0x:%0x:%0x:%0x]\ntarget ip [%d.%d.%d.%d]"
	,m->arphdr.source_mac[0]
	,m->arphdr.source_mac[1]
	,m->arphdr.source_mac[2]
	,m->arphdr.source_mac[3]
	,m->arphdr.source_mac[4]
	,m->arphdr.source_mac[5]
	,m->arphdr.source_ip[0]
	,m->arphdr.source_ip[1]
	,m->arphdr.source_ip[2]
	,m->arphdr.source_ip[3]
	,m->arphdr.target_mac[0]
	,m->arphdr.target_mac[1]
	,m->arphdr.target_mac[2]
	,m->arphdr.target_mac[3]
	,m->arphdr.target_mac[4]
	,m->arphdr.target_mac[5]
	,m->arphdr.target_ip[0]
	,m->arphdr.target_ip[1]
	,m->arphdr.target_ip[2]
	,m->arphdr.target_ip[3]);
};


void print_pkt(uint8_t *pkt, int len, int fd)
{
    int i = 0;
    int j = 0;

    dprintf(fd , "Paquet %d\n", len);
    while (i <= len)
    {
       dprintf(fd , "%02x ", pkt[i]);
        if(j == 3)
            write(fd, " ", 1);
        else if (j == 7)
        {
            j = -1;
            write(fd, "\n", 1);
        }
       i++;
       j++;
    }
    dprintf(fd , "\n\n");
}

int cmpuchar(uint8_t a[4], uint8_t b[4], size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		// printf("%0x   %0x\n", a[i], b[i]);
		if (a[i] != b[i])
			return (0);
	}
	return (1);
}

void interface_found(t_malcolm *mlcml, struct sockaddr_ll *s)
{
	uint8_t buff[255];
	int len = -1;
	struct ethhdr *rcv_resp;
	t_myarp *arp;

	if ((mlcml->socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) <= 0)
	{
		dprintf(2, "error socket\n");
        exit(3);
	}
	while (!g_sigint 
		&&(len = recvfrom(mlcml->socket, buff, 255, 0, NULL, NULL)) >= 0)
	{
	    rcv_resp = (struct ethhdr *) buff;
	    arp = (t_myarp *) (buff + 14);
	    if (ntohs(rcv_resp->h_proto) != ETH_P_ARP)
	    {
			printf("Not an ARP packet\n");
			continue;
	    }
	    if (ntohs(arp->opcode) != ARPOP_REQUEST){
			printf("Not an ARP REQUEST\n");
			continue;
	    }
	    // printf("[%u.%u.%u.%u]\n", arp->target_ip[0], arp->target_ip[1], arp->target_ip[2], arp->target_ip[3]);
   		if (cmpuchar(arp->source_ip, mlcml->arphdr.source_ip, 4)
   			&& cmpuchar(arp->source_mac, mlcml->arphdr.source_mac, 6)
   			&& cmpuchar(arp->target_ip, mlcml->arphdr.target_ip, 4))
   		{
	   		print_pkt(buff, len, 2);
	   		letsgo_mitm(mlcml, s);
	   		break ;
	   	}

	}
	// mlcml->arphdr = create_arp_pkt(mlcml, );
	// while (!g_sigint)
	// {
	// 	int retb = 0;

	// 	retb = recvfrom()
	// 	wiat for arp request
	// }
}


void get_ifaddr(t_malcolm *mlcml)
{
	struct ifaddrs *ifa;
	struct sockaddr_ll *s = NULL;
	ifa = NULL;
	// tmp = NULL;
	(void)mlcml;
	if (getifaddrs(&ifa) == -1)
	{
		printf("error get_ifaddr\n");
		exit(1);
	}
	// tmp = &ifa;
	while (ifa != NULL)
	{
		if (ifa->ifa_addr && ft_strncmp(ifa->ifa_name, "lo", 2) && ft_strncmp(ifa->ifa_name, "wlo", 3))
		{
			// if (ifa->ifa_addr->sa_family == AF_INET)
			// {
			// 	char **tab = NULL;
			// 	tab = ft_strsplit(inet_ntoa(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr), ".");
			// 	if (ft_tablen(tab) != 4)
			// 		printf("tailee pas bomnee\n");
			// 	for (int i = 0; i < 4; ++i)
   //            		mlcml->ip_laddr[i] = ft_atoi(tab[i]);
			// }
			// else 
			if (ifa->ifa_addr->sa_family == AF_PACKET)
			{
				s = (struct sockaddr_ll*)ifa->ifa_addr;
              	for (int i=0; i <s->sll_halen; i++)
              	{
					// char *tnmp = (char)s->sll_addr[i];
              		mlcml->mac_laddr[i] = s->sll_addr[i];//ft_atoi_base(tnmp, "0123456789abcdef");

              		// break;
              	}
            }
		}
		ifa = ifa->ifa_next;
	}
	interface_found(mlcml, s);
	// freeifaddrs(*tmp);
}

void    sig_handler(int sig)
{
    if (sig == SIGINT)
        g_sigint = 1;
}

int main(int argc, char const *argv[])
{
	t_malcolm mlcml;
	
	if (argc != 5)
		malcolm_usage("Error", "Invalid number of args");
	init_n_fill_mlcml(argv, &mlcml);
	get_ifaddr(&mlcml);
	debug_malcolm(&mlcml);
	return 0;
}

// struct in_addr in