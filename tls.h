#ifndef TLS_H
#define TLS_H

#include <stdint.h>

typedef struct
{
	uint8_t* buffer;
	uint32_t available;
	uint32_t i;
} NetMsg;

NetMsg* NetMsg_new();
void NetMsg_push8(NetMsg* msg, uint8_t v);
void NetMsg_push16(NetMsg* msg, uint16_t v);
void NetMsg_push32(NetMsg* msg, uint32_t v);
void NetMsg_send(NetMsg* msg, int sock, uint8_t type0, uint8_t type1);

void send_clientHello(int sock);
void get_clientHello(int sock);

#endif
