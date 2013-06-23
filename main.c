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

#include <stdio.h>
#include <unistd.h>

#include "socket.h"
#include "tls.h"

int main(int argc, char** argv)
{
	(void) argc;
	(void) argv;

	int sock = TCP_Listen("4242");
	int client;
	while ((client = TCP_Accept(sock)))
	{
		printf("Got a client\n");
		get_clientHello(client);
	}
	close(sock);
	return 0;
	
	/*
	srand(time(NULL));
	
	//int sock = TCPConnect("74.125.230.81", 443);
	int sock = TCPConnect("140.77.129.194", 4242);
	if (!sock)
	{
		fprintf(stderr, "Could not connect\n");
		exit(-1);
	}
	send_clientHello(sock);
	char buffer[1024];
	int r;
	do
	{
		r = read(sock, &buffer, sizeof(buffer));
		for (uint32_t i = 0; i < r; i++)
			putc(buffer[i], stdout);
	}
	while (r == sizeof(buffer));
	close(sock);
	return 0;
	*/
}
