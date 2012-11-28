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
