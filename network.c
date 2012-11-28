#include "network.h"

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>


int TCPConnect(const char* IP, uint16_t port)
{
	int sock;
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		//std::cerr << "Could not make socket\n";
		return 0;
	}
	
	struct sockaddr_in host;
	inet_pton(AF_INET, IP, &host.sin_addr);
	host.sin_family = AF_INET;
	host.sin_port = htons(port);
	
	if (connect(sock, (struct sockaddr*)&host, sizeof(host)) < 0)
	{
		//std::cerr << "Could not connect to host\n";
		return 0;
	}
	
	int v=1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(int));
	
	return sock;
}

int listenAt(uint16_t port)
{
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0)
		return -1;
	
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	
	int v=1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(int));
	
	if (bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		return -1;
	
	if (listen(sock, 1) < 0)
		return -1;
	
	return sock;
}

/*
pollList* pollList_new()
{
	pollList* ret = malloc(sizeof(pollList));
	ret->data = NULL;
	ret->size = 0;
	ret->available = 0;
	return ret;
}

void pollList_delete(pollList* fdl)
{
	free(fdl->data);
	free(fdl);
}

void pollList_add(pollList* fdl, int socket)
{
	if (fdl->size >= fdl->available)
	{
		if (fdl->available)
		{
			fdl->available *= 2;
			fdl->data = realloc(fdl->data, fdl->available * sizeof(pollList));
		}
		else
		{
			fdl->available = 1;
			fdl->data = malloc(sizeof(pollList));
		}
	}
	struct pollfd fd = { socket, POLLIN, 0 };
	fdl->data[fdl->size++] = fd;
}

void pollList_remove(pollList* fdl, uint8_t idx)
{
	memmove(&(fdl->data[idx]), &(fdl->data[idx+1]), (fdl->size-idx-1) * sizeof(pollList));
	fdl->size--;
}

int pollList_poll(pollList* fdl, int timeout)
{
	return poll(fdl->data, fdl->size, timeout);
}

int pollList_getSocket(pollList* fdl, uint8_t idx)
{
	return fdl->data[idx].fd;
}

#include <stdio.h>
bool pollList_newEvent(pollList* fdl, uint8_t idx)
{
	return fdl->data[idx].revents != 0;
}
*/
