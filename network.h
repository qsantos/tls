#ifndef NETWORK_H
#define NETWORK_H

#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>

int TCPConnect(const char* IP, uint16_t port);
int listenAt(uint16_t port);

/*
typedef struct
{
	struct pollfd* data;
	uint8_t size;
	uint8_t available;
} pollList;

pollList* pollList_new();
void pollList_delete(pollList* fdl);
void pollList_add(pollList* fdl, int socket);
void pollList_remove(pollList* fdl, uint8_t idx);
int pollList_poll(pollList* fdl, int timeout);
int pollList_getSocket(pollList* fdl, uint8_t idx);
bool pollList_newEvent(pollList* fdl, uint8_t idx);
*/

#endif
