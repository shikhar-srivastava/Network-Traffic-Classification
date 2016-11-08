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
#include <time.h>
#include <poll.h>

//				   256KB
#define PIECE_SIZE 256000
#define FILE_ROOT "../res/"
#define BCAST_LISTEN 38080
#define DATA_PORT 38086

void sendFile(int data_socket, FILE* file_fd) {	//send contents of file_fd to data_socket in pieces of size PIECE_SIZE
	char buffer[PIECE_SIZE];
	
	fseek(file_fd, 0, SEEK_END);
	size_t flen = ftell(file_fd);
	fseek(file_fd, 0, SEEK_SET);

	//send file size to client
	send(data_socket, &flen, sizeof(size_t), 0);

	int n;
	while((n = fread(buffer, sizeof(char), PIECE_SIZE, file_fd)) > 0)
		send(data_socket, buffer, n, 0);
	printf("Sent file to client.\n");
	fclose(file_fd);
}

void main() {
	int bcast_socket;
	int data_socket, listen_socket;
	
	char buffer[PIECE_SIZE];
	int len = 0, n = 0, bcastAddrLen = sizeof(struct sockaddr_in), p;
	struct sockaddr_in clntAddr, bcastAddr;
	
	char clntName[] = "127.0.0.1";	//10.86.0.39
	
	memset(&clntAddr, 0, sizeof(clntAddr));
	memset(&bcastAddr, 0, sizeof(bcastAddr));
	
	if((bcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) { //Broadcast socket for accepting requests
		perror("Error : Broadcast socket creation failed");
		exit(1);
	}
	
	int bcastON = 1;
	if (setsockopt(bcast_socket, SOL_SOCKET, SO_BROADCAST, &bcastON, sizeof(bcastON)) == -1) {	//enable broadcasting
		perror("setsockopt (SO_BROADCAST)");
		exit(1);
	}
	printf(" \n");	//very important for some weird reason

	bcastAddr.sin_family = AF_INET;
	bcastAddr.sin_addr.s_addr = inet_addr("10.52.7.255");
	bcastAddr.sin_port = htons(BCAST_LISTEN);
 	
	clntAddr.sin_family = AF_INET;
	// clntAddr.sin_addr.s_addr = inet_addr(servName);
	clntAddr.sin_port = htons(DATA_PORT);
	
	// if((bind(bcast_socket, (struct sockaddr*) &clntAddr, sizeof(bcastAddr))) < 0) {
	// 	perror("Error : Broadcast socket bind failed");
	// 	exit(1);
	// }
	
	printf("Now listening for file requests...\n");
	while(1) {
		if(recvfrom(bcast_socket, buffer, strlen(buffer), 0,(struct sockaddr*)&bcastAddr, &bcastAddrLen) <= 0) {
			perror("recvfrom failed : ");
			exit(1);
		}
		printf("Received request : %s\n", buffer);
		if(!(p = fork())) {			//child
			sscanf(buffer,"myIP:%[0-9.]FILE:%s",clntName ,file_path + strlen(FILE_ROOT));
			clntAddr.sin_addr.s_addr = inet_addr(clntName);

			char file_path[256] = FILE_ROOT;
			sscanf(buffer, "FILE:%[^\n]", );	//append the file path to the FILE_ROOT
			FILE *file_fd;

			printf("File request : %s\n", buffer);

			if((file_fd = fopen(file_path, "r")) == NULL) {	//file not found
				printf("File not found\n");
				exit(0);
			}	//establish connection only in file exists

			if((data_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				perror("Error : data_socket creation failed");
				exit(1);
			}
			
			if((connect(data_socket, (struct sockaddr*) &clntAddr, sizeof(clntAddr))) < 0) {
				printf("The server is offline!\n");
				exit(1);
			}

			sendFile(data_socket, file_fd);
			
			close(data_socket);
			break;
		}
		else if(p > 0) continue;	//parent
		else {						//error
			perror("Fork failed : ");
			exit(1);
		}
	}
	if(p) close(listen_socket);
}