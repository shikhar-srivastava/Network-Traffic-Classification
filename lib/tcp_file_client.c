#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define PIECE_SIZE 256000


void main(int argc, char* argv[]) {
	char buffer[PIECE_SIZE];
	int data_socket, servPort;
	size_t file_size;
	struct sockaddr_in servAddr;
	char* file_path, *myIP;

	if(argc == 3) {
		file_path = argv[1];
		myIP = argv[2];
	}
	else {
		printf("Usage : $%s \"file_name\" \"your_IP_addr\"\n", argv[0]);
		exit(1);
	}
	char *servName=myIP;
	servPort = 38086;
	
	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET,servName,&servAddr.sin_addr);
	servAddr.sin_port = htons(servPort);
	
	if((data_socket=socket(AF_INET,SOCK_STREAM,0))<0) {
		perror("Error : Socket creation failed");
		exit(1);
	}
	
	if((connect(data_socket, (struct sockaddr*) &servAddr, sizeof(servAddr)))<0) {
		printf("The server is offline!\n");
		exit(1);
	}
	
	send(data_socket, file_path, strlen(file_path), 0);
	
	if(recv(data_socket, &file_size, sizeof(size_t), 0) <= 0) {
		printf("Unable to establish connection!\n");
		exit(1);
	}

	if(file_size > 0) {
		int bytes_read = 0;
		long total_bytes_read;
		FILE *file_fd = fopen(file_path, "w+");

		while(total_bytes_read < file_size) {
			bytes_read = recv(data_socket, buffer, PIECE_SIZE, 0);
			if(bytes_read <= 0)
				break;
			fwrite(buffer, sizeof(char), bytes_read, file_fd);
			total_bytes_read += bytes_read;
		}
		fclose(file_fd);
		printf("Read %ld bytes into %s succesfully!\n", total_bytes_read, file_path);

	}
	else
		printf("File not found!\n");


	close(data_socket);
}