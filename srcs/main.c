#include "ft_malcolm.h"

void malcolm_usage(char *error, char *str)
{
	dprintf(2,"ft_malcolm: %s : %s\n", error, str);
	exit(42);
}

void debug_malcolm(t_malcolm *m)
{
	printf("\nSource  IP {%s}\nSource  MAC {%s}\nDest    IP {%s}\nDest   MAC {%s}\n", m->ip_saddr, m->mac_saddr, m->ip_daddr, m->mac_daddr);
	printf("ARP HEADER\nsource mac [%c%c:%c%c:%c%c:%c%c:%c%c:%c%c]\nsource ip [%d.%d.%d.%d]\ntarget mac [%c%c:%c%c:%c%c:%c%c:%c%c:%c%c]\ntarget ip [%d.%d.%d.%d]"
	,m->arphdr.source_mac[0][0]
	,m->arphdr.source_mac[0][1]
	,m->arphdr.source_mac[1][0]
	,m->arphdr.source_mac[1][1]
	,m->arphdr.source_mac[2][0]
	,m->arphdr.source_mac[2][1]
	,m->arphdr.source_mac[3][0]
	,m->arphdr.source_mac[3][1]
	,m->arphdr.source_mac[4][0]
	,m->arphdr.source_mac[4][1]
	,m->arphdr.source_mac[5][0]
	,m->arphdr.source_mac[5][1]
	,m->arphdr.source_ip[0]
	,m->arphdr.source_ip[1]
	,m->arphdr.source_ip[2]
	,m->arphdr.source_ip[3]
	,m->arphdr.target_mac[0][0]
	,m->arphdr.target_mac[0][1]
	,m->arphdr.target_mac[1][0]
	,m->arphdr.target_mac[1][1]
	,m->arphdr.target_mac[2][0]
	,m->arphdr.target_mac[2][1]
	,m->arphdr.target_mac[3][0]
	,m->arphdr.target_mac[3][1]
	,m->arphdr.target_mac[4][0]
	,m->arphdr.target_mac[4][1]
	,m->arphdr.target_mac[5][0]
	,m->arphdr.target_mac[5][1]
	,m->arphdr.target_ip[0]
	,m->arphdr.target_ip[1]
	,m->arphdr.target_ip[2]
	,m->arphdr.target_ip[3]);
};
// static void create_socket(t_malcolm *mlcml)
// {
//     int t;

//     t = 1;
//     if ((mlcml->socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) <= 0)
//         ft_perror("socket", "can't create socket", 32, 42);
 	// if (setsockopt(pg->socket, IPPROTO_IP, IP_HDRINCL, &t, sizeof(int)))
  //           ft_perror("setsockopt", "can't set IP_HDRINCL on socket", 32, 42);

  //   if (setsockopt(pg->socket, SOL_IP, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
  //       ft_perror("setsockopt", "can't set IP_HDRINCL on socket", 32, 42);
// }

int wait_arp_pkt(t_malcolm *mlcml)
{
	int len = -1;
	t_myarp *arp = NULL;
	while (!(len = recvfrom(mlcml->socket, &arp, sizeof(t_myarp), 0, NULL, NULL)))
	{
		if (len == -1)
			return (len);
	}
	printf("%s\n", );
}

void interface_found(struct ifaddrs *ifa, t_malcolm *mlcml)
{
	printf("\tFound available interface: <%s> @",ifa->ifa_name ); 
	printf(" Address: <%s>\n", inet_ntoa(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr));
	// create_socket(mlcml);
	char *buff[1024];
	int len = -1;
	struct ethhdr *rcv_resp;
	t_myarp *arp_resp;

	if ((mlcml->socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) <= 0)
	{
		dprintf(2, "error socket\n");
        exit(3);
	}
	while (1)
	{
		if (g_sigint)
	    	break ;
		if ((len = recvfrom(mlcml->socket, buff, 1024, 0, NULL, NULL) == -1))
			malcolm_usage("recvfrom", "pb");
	    rcv_resp = (struct ethhdr *) buff;
	    arp_resp = (t_myarp *) (buff + sizeof(struct ethhdr));
	    if (ntohs(rcv_resp->p_proto) != htons(ETH_P_ARP))
			malcolm_usage(NULL, "Not an ARP packet");
	    if (ntohs(arp_resp->opcode) != 0x001)
			malcolm_usage(NULL, "Not an ARP REQUEST");
	   	printf("received ARP len=%d", len);
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

	ifa = NULL;
	if (getifaddrs(&(ifa)) == -1)
	{
		printf("error get_ifaddr\n");
		exit(1);
	}
	while (ifa != NULL)
	{
		if (ifa->ifa_addr && (ifa->ifa_addr->sa_family == AF_INET) && ft_strncmp(ifa->ifa_name, "lo", 2))
		{
			interface_found(ifa, mlcml);
			break;
		}
		ifa = ifa->ifa_next;
	}
	freeifaddrs(ifa);
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



























	// localHost = gethostbyname("");
	// inet_addr(localIP);
	// for (int i = 0; localHost->h_addr_list[i]; ++i)
	// {
		// printf("%s\n", localHost->h_addr);
	// }
// if (!(host = gethostbyname(mlcml->ifap->ifa_name)))
				// printf("{%hu}\n", ntohs(mlcml->ifap->ifa_addr->sa_data));
			// printf("%s          [%s]\n"
// ,mlcml->ifap->ifa_name, host->h_name);
			// for (int i = 0; host->h_addr_list[i]; ++i)
			// {
				// printf("%s %s\n",host->h_addr_list[i], host->h_name);
			// }
			// local = gethostbyname("");
			// inet_addr()