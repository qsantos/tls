#include "tls.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

NetMsg* NetMsg_new()
{
	NetMsg* ret = malloc(sizeof(NetMsg));
	ret->i = 9;
	ret->available = ret->i;
	ret->buffer = malloc(ret->available);
	return ret;
}

void NetMsg_push8(NetMsg* msg, uint8_t v)
{
	if (msg->i >= msg->available)
	{
		msg->available = msg->available ? msg->available * 2 : 0;
		msg->buffer = realloc(msg->buffer, msg->available);
	}
	msg->buffer[msg->i++] = v;
}

void NetMsg_push16(NetMsg* msg, uint16_t v)
{
	NetMsg_push8(msg, (v >> 8) & 0xFF);
	NetMsg_push8(msg, (v >> 0) & 0xFF);
}

void NetMsg_push32(NetMsg* msg, uint32_t v)
{
	NetMsg_push8(msg, (v >> 24) & 0xFF);
	NetMsg_push8(msg, (v >> 16) & 0xFF);
	NetMsg_push8(msg, (v >>  8) & 0xFF);
	NetMsg_push8(msg, (v >>  0) & 0xFF);
}

void NetMsg_send(NetMsg* msg, int sock, uint8_t type0, uint8_t type1)
{
	// record
	msg->buffer[0] = type0;
	msg->buffer[1] = 0x03; // major version number
	msg->buffer[2] = 0x03; // minor version number
	uint16_t recLen = msg->i - 5;
	msg->buffer[3] = (recLen >> 8) & 0xFF;
	msg->buffer[4] = (recLen >> 0) & 0xFF;
	
	// ???
	msg->buffer[5] = type1;
	uint16_t unkLen = msg->i - 9;
	msg->buffer[6] = (unkLen >> 16) & 0xFF;
	msg->buffer[7] = (unkLen >>  8) & 0xFF;
	msg->buffer[8] = (unkLen >>  0) & 0xFF;
	
	write(sock, msg->buffer, msg->i);
	free(msg->buffer);
	free(msg);
}

#define N_MYCIPHERSUITES 36
static const uint16_t mycipherSuites[N_MYCIPHERSUITES] =
{
	0x00ff, 0xc00a, 0xc014, 0x0088,
	0x0087, 0x0039, 0x0038, 0xc00f,
	0xc005, 0x0084, 0x0035, 0xc007,
	0xc009, 0xc011, 0xc013, 0x0045,
	0x0044, 0x0033, 0x0032, 0xc00c,
	0xc00e, 0xc002, 0xc004, 0x0096,
	0x0041, 0x0004, 0x0005, 0x002f,
	0xc008, 0xc012, 0x0016, 0x0013,
	0xc00d, 0xc003, 0xfeff, 0x000a
};

void send_clientHello(int sock)
{
	NetMsg* msg = NetMsg_new();
	
	NetMsg_push8(msg, 0x03);
	NetMsg_push8(msg, 0x03); // version
	
	// random
	NetMsg_push32(msg, time(NULL));
	for (uint8_t i = 0; i < 28; i++)
		NetMsg_push8(msg, rand());
	
	// session id
	NetMsg_push8(msg, 0);
	
	NetMsg_push16(msg, sizeof(mycipherSuites));
	for (uint8_t i = 0; i < N_MYCIPHERSUITES; i++)
		NetMsg_push16(msg, mycipherSuites[i]);
	
	uint8_t compressionMethods[] = { 0x00 };
	NetMsg_push8(msg, sizeof(compressionMethods));
	for (uint8_t i = 0; i < sizeof(compressionMethods); i++)
		NetMsg_push8(msg, compressionMethods[i]);
	
	NetMsg_send(msg, sock, 0x16, 0x01); // handshake / client_hello
}

typedef struct
{
	uint8_t type0;
	uint8_t version[2];
	uint8_t length0[2];
	uint8_t type1;
	uint8_t length1[3];
} recordHeader;

void get_clientHello(int sock)
{
	recordHeader head;
	read(sock, &head, sizeof(head));
	uint32_t len = (head.length1[0] << 16) | (head.length1[1] << 8) | (head.length1[2] << 0);
	uint8_t* data = malloc(len);
	read(sock, data, len);
	
	uint8_t* version = data;
	uint8_t* random  = version + 2;
	
	uint8_t* sessionId = random + 32;
	uint8_t sessionId_size = *sessionId;
	sessionId += 1;
	
	uint8_t* cipherSuites = sessionId + sessionId_size;
	uint16_t cipherSuites_size = (*cipherSuites << 8) | *(cipherSuites + 1);
	cipherSuites += 2;
	uint16_t cipherSuite = 0;
	for (uint8_t i = 0; !cipherSuite && i < N_MYCIPHERSUITES; i++)
		for (uint8_t j = 0; j < cipherSuites_size; j+=2)
			if ( ((cipherSuites[j] << 8) | cipherSuites[j+1]) == mycipherSuites[i])
			{
				cipherSuite = mycipherSuites[i];
				break;
			}
	
	uint8_t* compressionMethods = cipherSuites + cipherSuites_size;
	uint8_t compressionMethods_size = *compressionMethods;
	compressionMethods += 1;
	
	uint8_t* extensions = compressionMethods + compressionMethods_size;
	if (data + len > extensions)
	{
		uint16_t extensions_size = (*extensions << 8) | *(extensions + 1);
		extensions += 2;
		printf("extensions_size = %d\n", extensions_size);
	}
	
	
	puts("client_hello got");
	
	
	NetMsg* msg = NetMsg_new();
	NetMsg_push8(msg, 0x03);
	NetMsg_push8(msg, 0x03); // version
	// random
	NetMsg_push32(msg, time(NULL));
	for (uint8_t i = 0; i < 28; i++)
		NetMsg_push8(msg, rand());
	NetMsg_push8(msg, 0); // session id
	NetMsg_push16(msg, cipherSuite); // cipher suite
	NetMsg_push8(msg, 0); // compression method
	NetMsg_send(msg, sock, 0x16, 0x02); // handshake / server_hello
	
	puts("server_hello sent");
	
	free(data);
	//NetMsg* msg = NetMsg_new();
	//NetMsg_send(msg, sock, 0x16, 0x0b); // handshake / certificate
}