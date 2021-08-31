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

void get_ifaddr(t_malcolm *mlcml)
{
	char buf[INET_ADDRSTRLEN];
	struct hostent* host;

	if (getifaddrs(&(mlcml->ifap)) == -1)
	{
		printf("error get_ifaddr\n");
		exit(1);
	}
	while (mlcml->ifap)
	{
		if (mlcml->ifap->ifa_addr->sa_family == AF_INET)
		{
			host = gethostbyname(mlcml->ifap->ifa_name);
			// inet_addr()
			inet_ntop(AF_INET, mlcml->ifap->ifa_addr->sa_data, (void*)buf, INET_ADDRSTRLEN);
			printf("%s\n", mlcml->ifap->ifa_name);
		}
		mlcml->ifap = mlcml->ifap->ifa_next;
	}
}

int main(int argc, char const *argv[])
{
	t_malcolm mlcml;
	// struct hostent* localHost;


	if (argc != 5)
		malcolm_usage("Error", "Invalid number of args");
	// localHost = gethostbyname("");
	// inet_addr(localIP);
	// for (int i = 0; localHost->h_addr_list[i]; ++i)
	// {
		// printf("%s\n", localHost->h_addr);
	// }
	init_n_fill_mlcml(argv, &mlcml);
	get_ifaddr(&mlcml);
	freeifaddrs(mlcml.ifap);
	debug_malcolm(&mlcml);
	return 0;
}