#include "ft_malcolm.h"

static int check_ipv4_format(char *arg)
{
	int j = 0, dot = 0;
	char byte[4];

	if (!arg)
		return (0);
	ft_bzero(byte, 4);
	for (int i = 0; arg[i]; i++)
	{
		if (j < 3 && ft_isdigit(arg[i]))
		{
			byte[j] = arg[i];
			j++;
		}
		else if (dot < 3 && arg[i] == '.')
		{
			dot++;
			if (ft_atoi(byte) > 255)
				malcolm_usage(byte, "> 255");
			ft_bzero(byte, 4);
			j = 0;
		}
		else
			malcolm_usage(byte, "j> 3 & != .");
	}
	if (ft_atoi(byte) > 255)
		malcolm_usage(byte, "> 255");
	return 1;
}

static int is_macaddr_char(char c)
{
	if (c == '*')
		return (1);
	else if (c >= 'a' && c <= 'f')
		return (1);
	else if (c >= 'A' && c <= 'F')
		return (1);
	else if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int check_macaddr_format(char *arg)
{
	int i = 0, j = 0, deuxpts = 0;
	if (!arg)
		return (0);
	while (arg[i])
	{
		if (is_macaddr_char(arg[i]))
			j++;
		else if (arg[i] == ':')
		{
			if (deuxpts > 4 || j % 2 != 0)
				malcolm_usage(&arg[i], "mauvais : nbr");
			deuxpts++;
		}
		else
			malcolm_usage(&arg[i], "bad mac @");
		i++;
	}
	return (1);
}

void init_n_fill_mlcml(const char **av, t_malcolm *mlcml)
{
	ft_memset(mlcml, 0, sizeof(t_malcolm));
	if (av[1] && check_ipv4_format((char*)av[1]))
		mlcml->ip_saddr = (char*)av[1];
	if (av[2] && check_macaddr_format((char*)av[2]))
		mlcml->mac_saddr = (char*)av[2];
	if (av[3] && check_ipv4_format((char*)av[3]))
		mlcml->ip_daddr = (char*)av[3];
	if (av[4] && check_macaddr_format((char*)av[4]))
		mlcml->mac_daddr = (char*)av[4];
	if ((mlcml->ips = inet_addr(av[1])) == INADDR_NONE)
		malcolm_usage((char*)av[1], "bad ip @");
	if ((mlcml->ipd = inet_addr(av[3])) == INADDR_NONE)
		malcolm_usage((char*)av[3], "bad ip @");
}
