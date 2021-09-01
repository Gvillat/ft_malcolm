#include "ft_malcolm.h"

void malcolm_usage(char *error, char *str)
{
	dprintf(2,"ft_malcolm: %s : %s\n", error, str);
	exit(42);
}

void debug_malcolm(t_malcolm *m)
{
	printf("\nSource  IP {%s}\nSource  MAC {%s}\nDest    IP {%s}\nDest   MAC {%s}\n", m->ip_saddr, m->mac_saddr, m->ip_daddr, m->mac_daddr);
}

static void create_socket(t_malcolm *mlcml)
{
    int t;

    t = 1;
    if ((mlcml->socket = socket(PF_INET, SOCK_RAW, htons(ETH_P_ALL))) <= 0)
        ft_perror("socket", "can't create socket", 32, 42);
 	if (setsockopt(pg->socket, IPPROTO_IP, IP_HDRINCL, &t, sizeof(int)))
            ft_perror("setsockopt", "can't set IP_HDRINCL on socket", 32, 42);

    if (setsockopt(pg->socket, SOL_IP, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
        ft_perror("setsockopt", "can't set IP_HDRINCL on socket", 32, 42);
}

void interface_found(struct ifaddrs *ifa, t_malcolm *mlcml)
{
	printf("\tFound available interface: <%s> @",ifa->ifa_name ); 
	printf(" Address: <%s>\n", inet_ntoa(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr));
	create_socket(mlcml);
	while (!mlcml->sigint)
	{
		wiat for arp request
	}
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
			interface_found(ifa, mlclm);
			freeifaddrs(ifa);
			break;
		}
		ifa = ifa->ifa_next;
	}
	freeifaddrs(ifa);
}

void    sig_handler(int sig, t_malcolm *mlcml)
{
    if (sig == SIGINT)
        mlcml->sigint = 1;
}

int main(int argc, char const *argv[])
{
	t_malcolm mlcml;
	// struct hostent* localHost;


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
			// printf("%s          [%s]\n",mlcml->ifap->ifa_name, host->h_name);
			// for (int i = 0; host->h_addr_list[i]; ++i)
			// {
				// printf("%s %s\n",host->h_addr_list[i], host->h_name);
			// }
			// local = gethostbyname("");
			// inet_addr()