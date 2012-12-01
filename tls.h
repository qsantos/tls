#ifndef TLS_H
#define TLS_H

#include <stdint.h>

typedef enum
{
	change_cipher_spec = 20,
	alert              = 21,
	handshake          = 22,
	application_data   = 23,
} ContentType;

typedef enum
{
	hello_request       = 0,
	client_hello        = 1,
	server_hello        = 2,
	certificate         = 11,
	server_key_exchange = 12,
	certificate_request = 13,
	server_hello_done   = 14,
	certificate_verify  = 15,
	client_key_exchange = 16,
	finished            = 20,
} HandshakeType;

typedef struct
{
uint8_t* buffer;
uint32_t available;
uint32_t i;
} NetMsg;

NetMsg* NetMsg_new   ();
void    NetMsg_push8 (NetMsg* msg, uint8_t  v);
void    NetMsg_push16(NetMsg* msg, uint16_t v);
void    NetMsg_push32(NetMsg* msg, uint32_t v);
void    NetMsg_send  (NetMsg* msg, int sock, ContentType type0, HandshakeType type1);

void send_clientHello(int sock);
void get_clientHello (int sock);

#endif
