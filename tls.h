#ifndef TLS_H
#define TLS_H

#include <stdint.h>

typedef struct __attribute__((packed))
{
	uint8_t major;
	uint8_t minor;
} ProtocolVersion;

#define CHANGE_CIPHER_SPEC 20
#define ALERT              21
#define HANDSHAKE          22
#define APPLICATION_DATA   23

typedef struct __attribute__((packed))
{
	uint8_t         type;
	ProtocolVersion version;
	uint16_t        length;
} RecordHeader;

#define HELLO_REQUEST       0
#define CLIENT_HELLO        1
#define SERVER_HELLO        2
#define CERTIFICATE         11
#define SERVER_KEY_EXCHANGE 12
#define CERTIFICATE_REQUEST 13
#define SERVER_HELLO_DONE   14
#define CERTIFICATE_VERIFY  15
#define CLIENT_KEY_EXCHANGE 16
#define FINISHED            20

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
