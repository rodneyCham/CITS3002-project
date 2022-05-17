#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 6060

int main() {

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	printf("Connecting...\n");
	int status = connect(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address));

	if(status != 0) {
		perror("connecting");
		exit(0);
	}

	char buffer[2048];

	// do things
	while(1) {
		printf(">");
		fgets(buffer, sizeof(buffer), stdin);

		send(socket_fd, buffer, sizeof(buffer), 0);

		recv(socket_fd, buffer, sizeof(buffer), 0);
		printf("server returned: %s\n", buffer);
	}

	close(socket_fd);

	return 0;
}
