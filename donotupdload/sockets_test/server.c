#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 6060

int main() {

	int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket_fd, (struct sockaddr*) &server_address, sizeof(server_address));

	printf("Listening...\n");
	listen(server_socket_fd, 5);

	printf("Waiting...\n");
	int client_socket_fd = accept(server_socket_fd, NULL, NULL);

	printf("Connected!\n");

	// do stuff

	char buffer[2048];

	do {
		recv(client_socket_fd, buffer, sizeof(buffer), 0);

		printf("%s", buffer);

		send(client_socket_fd, "message received", sizeof("message received"), 0);
	} while (strcmp(buffer, "close\n") != 0);

	close(server_socket_fd);

	return 0;
}
