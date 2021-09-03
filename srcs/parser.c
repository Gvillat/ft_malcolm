#include "ft_malcolm.h"

static int check_ipv4_format(char *arg, uint8_t ip[4])
{
	int j = 0;
	int tmp = 0;
	char **tab = NULL;

	if (!arg)
		return (0);
	if (!(tab = ft_strsplit(arg, ".")))
		return (0);
	if (ft_tablen(tab) != 4)
		return (0);
	while (tab[j])
	{
		for (int i = 0; tab[j][i]; ++i)
		{
			if (i >= 3 || !ft_isdigit(tab[j][i]))
      			malcolm_usage(tab[j], "j> 3 & !ft_isdigit");
		}
    	tmp = (uint8_t)ft_atoi(tab[j]);
    	if (tmp > 255)
        	malcolm_usage(tab[j], "> 255");
    	ip[j] = tmp;
    	j++;
	}
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


static int is_macaddr_byte(char *str)
{
	char a, b;

	if (!str || !str[0] || !str[1] || ft_strlen(str) != 2 )
		return (0);
	a = str[0];
	b = str[1];
	if (is_macaddr_char(a) && is_macaddr_char(b))
		return (1);
	return (0);
}

static int check_macaddr_format(char *arg, uint8_t mac[6])
{
	int i = 0;
	char **tab = NULL;

	if (!arg)
		return (0);
	if (!(tab = ft_strsplit(arg, ":")))
		return (0);
	if (ft_tablen(tab) != 6)
	{
		ft_tabfree(tab);
		return (0);
	}
	while (tab[i])
	{
		if (is_macaddr_byte(tab[i]))
			mac[i] = ft_atoi_base(tab[i], "0123456789abcdef");
		else
		{
			ft_tabfree(tab);
			malcolm_usage(arg, "bad mac @");
		}
		i++;
	}
	ft_tabfree(tab);
	return (1);
}

void init_n_fill_mlcml(const char **av, t_malcolm *mlcml)
{
	ft_memset(mlcml, 0, sizeof(t_malcolm));
	g_sigint = 0;
    signal(SIGINT, &sig_handler);
	if (av[1] && check_ipv4_format((char*)av[1], mlcml->arphdr.source_ip))
		mlcml->ip_saddr = (char*)av[1];
	if (av[2] && check_macaddr_format((char*)av[2], mlcml->arphdr.source_mac))
		mlcml->mac_saddr = (char*)av[2];
	if (av[3] && check_ipv4_format((char*)av[3], mlcml->arphdr.target_ip))
		mlcml->ip_daddr = (char*)av[3];
	if (av[4] && check_macaddr_format((char*)av[4], mlcml->arphdr.target_mac))
		mlcml->mac_daddr = (char*)av[4];
	if ((mlcml->ips = inet_addr(av[1])) == INADDR_NONE)
		malcolm_usage((char*)av[1], "bad ip @");
	if ((mlcml->ipd = inet_addr(av[3])) == INADDR_NONE)
		malcolm_usage((char*)av[3], "bad ip @");
}
