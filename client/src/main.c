#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "libnetcaller.h"

void dump_any(const char *packet)
{
	printf("received something. id %x\n", packet[0]);
}

int main()
{
	struct sockaddr_in target;
	const char *targetServer;
	const char *targetAuxPort;
	unsigned short targetPort = 2593;
	struct __attribute__((packed)) {
		char id;
		char user[30];
		char pass[30];
		char trash;
	} authenticate;
	Netcaller setup[] = {
		{ 0, 0, NULL, dump_any } // last entry
	};
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(fd >= 0 && "socket returned -1");

	targetServer = getenv("SERVER");
	if (targetServer == NULL)
		targetServer = "127.0.0.1";

	targetAuxPort = getenv("PORT");
	if (targetAuxPort != NULL)
	{
		targetPort = atoi(targetAuxPort);
		assert(targetPort > 0 && "atoi failed");
	}

	memset(&target, 0, sizeof(target));
	target.sin_family = AF_INET;
	target.sin_port = htons(targetPort);
	target.sin_addr.s_addr = inet_addr(targetServer);

	if (connect(fd, (struct sockaddr *) &target, sizeof(target)) < 0)
	{
		perror("connect");
		assert(0 && "connect failed");
	}

	memset(&authenticate, 0, sizeof(authenticate));
	authenticate.id = 0x80;
	strncpy(authenticate.user, "player1", 30);
	strncpy(authenticate.pass, "player1", 30);

	if (send(fd, &authenticate, sizeof(authenticate), 0) < 0)
	{
		perror("send");
		assert(0 && "send failed");
	}

	printf("sent %ld bytes to server\n", sizeof(authenticate));

	//select

	while (netcaller_generic_receive(setup, fd) == 0)
	{
	}

	perror("recv");
	assert(0 && "recv failed");

	close(fd);
	return 0;
}
