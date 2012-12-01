#ifndef TLS_H
#define TLS_H

#include <stdint.h>

typedef struct __attribute__((packed))
{
	uint8_t major;
	uint8_t minor;
} ProtocolVersion;

#define change_cipher_spec 20
#define alert              21
#define handshake          22
#define application_data   23

typedef struct __attribute__((packed))
{
	uint8_t         type;
	ProtocolVersion version;
	uint16_t        length;
} RecordHeader;

#define hello_request       0
#define client_hello        1
#define server_hello        2
#define certificate         11
#define server_key_exchange 12
#define certificate_request 13
#define server_hello_done   14
#define certificate_verify  15
#define client_key_exchange 16
#define finished            20

typedef struct __attribute__((packed))
{
	uint8_t msg_type;
	uint8_t length[3];
} HandshakeHeader;

typedef struct __attribute__((packed))
{
	uint32_t gmt_unix_time;
	uint8_t  random_bytes[28];
} Random;

typedef uint8_t CipherSuite[2];

typedef uint8_t CompressionMethod;

typedef struct __attribute__((packed))
{
	ProtocolVersion client_version;
	Random          random;
} ClientHelloHeader;

void get_clientHello(int sock);

/*
typedef struct __attribute__((packed))
{
uint8_t* buffer;
uint32_t available;
uint32_t i;
} NetMsg;

NetMsg* NetMsg_new   ();
void    NetMsg_push8 (NetMsg* msg, uint8_t  v);
void    NetMsg_push16(NetMsg* msg, uint16_t v);
void    NetMsg_push32(NetMsg* msg, uint32_t v);
void    NetMsg_send  (NetMsg* msg, int sock, uint8_t type0, uint8_t type1);

void send_clientHello(int sock);
void get_clientHello (int sock);
*/

#endif
