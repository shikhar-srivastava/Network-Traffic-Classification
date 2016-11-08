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

//				   256KB
#define PIECE_SIZE 256000
#define FILE_ROOT "../res/"

void sendFile(int data_socket, FILE* file_fd) {
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

void main(int argc, char* argv[]) {
	int data_socket, len = 0, servPort, listen_socket, n=0, waitSize=16, clntAddrLen = sizeof(struct sockaddr_in), p;
	struct sockaddr_in servAddr, clntAddr;
	if(argc == 2) char servName[]="192.168.43.68";
	else {
		printf("Usage : $%s \"your_IP_addr\"\n", argv[0]);
		exit(1);
	}
	servPort = 38086;
	memset(&servAddr,0,sizeof(servAddr));
	
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servName);
	servAddr.sin_port = htons(servPort);
	
	if((listen_socket=socket(AF_INET,SOCK_STREAM,0))<0) {
		perror("Error : Listen socket creation failed");
		exit(1);
	}
	
	if((bind(listen_socket,(struct sockaddr*) &servAddr,sizeof(servAddr)))<0) {
		perror("Error : Listen socket bind failed");
		exit(1);
	}
	if(listen(listen_socket,waitSize)<0) {
		perror("Error : Listening failed");
		exit(1);
	}

	while(1) {
		if((data_socket = accept(listen_socket,(struct sockaddr*) &clntAddr, &clntAddrLen)) < 0) {
			perror("Error : accept failed");
			exit(1);
		}
		if(!(p = fork())) {			//child
			char file_path[256] = FILE_ROOT;
			n = recv(data_socket, file_path + strlen(FILE_ROOT), 256 - strlen(FILE_ROOT), 0);	//append the file path to the FILE_ROOT
			file_path[n + (int)strlen(FILE_ROOT) + 1] = '\0';
			FILE *file_fd;

			if((file_fd = fopen(file_path, "r")) != NULL) {	//file exists
				sendFile(data_socket, file_fd);
			}
			else {
				int *fnf = (int) malloc(sizeof(size_t));
				*fnf = -1;
				send(data_socket, &fnf, sizeof(size_t), 0);
			}
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
