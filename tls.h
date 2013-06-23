/*\
 *  Unsecure implementation of TLS
 *  Copyright (C) 2012  Quentin SANTOS
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
\*/

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
typedef uint8_t Extension;

typedef struct __attribute__((packed))
{
	ProtocolVersion client_version;
	Random          random;

	uint8_t  n_session_id;
	uint8_t* session_id;

	uint16_t     n_cipher_suites;
	CipherSuite* cipher_suites;

	uint8_t            n_compression_methods;
	CompressionMethod* compression_methods;

	int16_t    n_extensions;
	Extension* extensions;
} ClientHello;

typedef struct __attribute__((packed))
{
	ProtocolVersion server_version;
	Random          random;

	uint8_t  n_session_id;
	uint8_t* session_id;

	CipherSuite       cipher_suite;
	CompressionMethod compression_method;

	int8_t   n_extensions;
	uint8_t* extensions;
} ServerHello;

void get_clientHello(int sock);

#endif
