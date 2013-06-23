/*\
 *  This is an awesome programm simulating awesome battles of awesome robot tanks
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

#include "tls.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>

void TLS_RecordHeader(int sock, uint8_t type, uint16_t length)
{
	RecordHeader tmp = { type, {0x03, 0x03}, length };
	write(sock, &tmp, sizeof(RecordHeader));
}

void TLS_HandshakeHeader(int sock, uint8_t type, uint32_t length)
{
	TLS_RecordHeader(sock, HANDSHAKE, length + 4);
	length = htons(length << 8);
	HandshakeHeader tmp;
	tmp.msg_type = type;
	memcpy(tmp.length, &length, 3);
	write(sock, &tmp, sizeof(HandshakeHeader));
}

void TLS_ServerHello(int sock, ClientHello* client_hello)
{
	ServerHello server_hello;

	// prepare server hello
	memcpy(&server_hello.server_version,    &client_hello->client_version,    sizeof(ProtocolVersion));
	memcpy(&server_hello.random,            &client_hello->random,            sizeof(Random));

	server_hello.n_session_id = client_hello->n_session_id;
	server_hello.session_id   = client_hello->session_id;

	server_hello.cipher_suite[0] = 0;
	server_hello.cipher_suite[1] = 0;
	server_hello.compression_method = 0;

	char ext = client_hello->n_extensions >= 0;
	if (ext)
	{
		server_hello.n_extensions = client_hello->n_extensions;
		server_hello.extensions   = client_hello->extensions;
	}


	// send TLS message
	TLS_HandshakeHeader(sock, SERVER_HELLO,
		sizeof(ProtocolVersion) +
		sizeof(Random) +
		sizeof(uint8_t) +
		server_hello.n_session_id +
		sizeof(CipherSuite) +
		sizeof(CompressionMethod) +
		ext ? 1 + server_hello.n_extensions : 0
	);
	write(sock, &server_hello.server_version,     sizeof(ProtocolVersion));
	write(sock, &server_hello.random,             sizeof(Random));
	write(sock, &server_hello.n_session_id,       sizeof(uint8_t));
	write(sock, server_hello.session_id,          server_hello.n_session_id);
	write(sock, &server_hello.cipher_suite,       sizeof(CipherSuite));
	write(sock, &server_hello.compression_method, sizeof(CompressionMethod));
	if (ext)
	{
		write(sock, &server_hello.n_extensions, sizeof(uint8_t));
		write(sock, server_hello.extensions,    server_hello.n_extensions);
	}
}

void TLS_ServerHelloDone(int sock)
{
	TLS_HandshakeHeader(sock, SERVER_HELLO_DONE, 0);
}

#define READ(V)                    \
{                                  \
	read(sock, &V, sizeof(V)); \
	avail -= sizeof(V);        \
}
#define READV(V, T, N)      \
{                           \
	V = (T*) malloc(N); \
	read(sock, V, N);   \
	avail -= N;         \
}
void TLS_ClientHello(int sock, int avail)
{
	ClientHello hello;
	READ(hello.client_version);
	READ(hello.random);
	printf("%i:%i\n", hello.client_version.major, hello.client_version.minor);

	READ(hello.n_session_id);
	READV(hello.session_id, uint8_t, hello.n_session_id);

	READ(hello.n_cipher_suites);
	hello.n_cipher_suites = ntohs(hello.n_cipher_suites);
	READV(hello.cipher_suites, CipherSuite, hello.n_cipher_suites);

	READ(hello.n_compression_methods);
	READV(hello.compression_methods, CompressionMethod, hello.n_compression_methods);

	if (avail)
	{
		READ(hello.n_extensions);
		hello.n_extensions = ntohs(hello.n_extensions);
		READV(hello.extensions, Extension, hello.n_extensions);
		printf("Extensions: %i\n", hello.n_extensions);
	}
	else
		hello.n_extensions = -1;

	printf("Done\n");
	printf("%i %i %i\n", hello.n_session_id, hello.n_cipher_suites, hello.n_compression_methods);

	TLS_ServerHello(sock, &hello);
	TLS_ServerHelloDone(sock);

	free(hello.compression_methods);
	free(hello.cipher_suites);
	free(hello.session_id);
}

void TLS_Handshake(int sock)
{
	HandshakeHeader tmp;
	read(sock, &tmp, sizeof(HandshakeHeader));
	switch (tmp.msg_type)
	{
	case CLIENT_HELLO:
	{
		uint32_t* nlen = (uint32_t*) tmp.length;
		uint32_t len = ntohl(*nlen) >> 8;
		TLS_ClientHello(sock, len);
		break;
	}
	default:
		printf("Not client_hello (%i)\n", tmp.msg_type);
		break;
	}
}

void get_clientHello(int sock)
{
	while (1)
	{
		RecordHeader tmp;
		read(sock, &tmp, sizeof(RecordHeader));
		printf("%i:%i\n", tmp.version.major, tmp.version.minor);
		printf("rlen = %i\n", ntohs(tmp.length));

		switch (tmp.type)
		{
		case HANDSHAKE:
			TLS_Handshake(sock);
			break;
		default:
			printf("Not handshake (%i)\n", tmp.type);
		}
	}
}
