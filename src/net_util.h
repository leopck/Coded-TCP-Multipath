/*
 * net_util.h 
 * 
 * Contains various network related utilities.
 */

#ifndef NET_UTIL_H
#define NET_UTIL_H

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <string.h>
#include <stdint.h>

typedef int socket_t;

#define NETIFDOWN	0
#define NETIFUP		1

// We have to take into account the IP+UDP header
// Max IP header is 24B, UDP header is 8B 
#define IPUDP_HEADER_SIZE 32 


struct addr_list
{
	int active;
	char if_name[10];
	struct sockaddr_in addr;
	struct addr_list * next;
	int sk;
};


typedef struct{
  double tstamp;
  flag_t flag;
  uint32_t  seqno; // Sequence # of the coded packet sent
  uint32_t  blockno; // Base of the current block
  uint8_t start_packet; // The # of the first packet that is mixed
  uint8_t  num_packets; // The number of packets that are mixed
  uint8_t packet_coeff; // Seed for coefficients of the mixed packets
  //  unsigned char checksum[CHECKSUM_SIZE];  // MD5 checksum
  char payload[PAYLOAD_SIZE];  
} Data_Pckt;

typedef struct{
  double tstamp;
  flag_t flag;
  uint32_t  ackno; // The sequence # that is being acked --> this is to make it Reno-like
  uint32_t  blockno; // Base of the current block
  uint8_t dof_rec;  // Number of dofs left from the block
  //  unsigned char checksum[CHECKSUM_SIZE];  // MD5 checksum
} Ack_Pckt;

// used for zero copy 
typedef union {
  Data_Pckt msg;
  Ack_Pckt ack;
  char buff[MSS];
} Msgbuf;

// used to provide pool of packets for mem management
typedef struct Skb {
  Msgbuf msgbuf;
  struct Skb* next;
  bool used;
} Skb;

typedef struct{
  uint16_t payload_size;
  char* payload;
} Bare_Pckt; // This is the datastructure for holding packets before encoding


/* Function declarations */
void htonpData(Data_Pckt *msg);
void htonpAck(Ack_Pckt *msg);
void ntohpData(Data_Pckt *msg);
void ntohpAck(Ack_Pckt *msg);
void prettyPrint(char** coeffs, int window);

#endif
