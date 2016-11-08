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
#include <poll.h>
#include <sys/wait.h>

#define PIECE_SIZE 256
#define BCAST_LISTEN 38080
#define DATA_PORT 38086

void main(int argc, char* argv[]) {
	char buffer[PIECE_SIZE];
	int data_socket, servPort, clntAddrLen;
	struct sockaddr_in myAddr, bcastAddr, clntAddr;
	char* file_path;
	
	if(argc == 2) file_path = argv[1];
	else {
		printf("Usage : $%s \"file_name\"\n", argv[0]);
		exit(1);
	}
	//char servName[]="127.0.0.1";
	//servPort = 38086;

	
	memset(&myAddr, 0, sizeof(myAddr));
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = inet_addr("192.168.43.67");
	myAddr.sin_port = htons(DATA_PORT);	
	
	memset(&bcastAddr, 0, sizeof(bcastAddr));
	bcastAddr.sin_family = AF_INET;
	bcastAddr.sin_addr.s_addr = INADDR_BROADCAST;//inet_addr("192.168.43.255");
	bcastAddr.sin_port = htons(BCAST_LISTEN);
	
	int bcast_socket;
	if((bcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) { //Broadcast socket for sending requests
		perror("Error : Broadcast socket creation failed");
		exit(1);
	}

	int bcastON = 1;
	if (setsockopt(bcast_socket, SOL_SOCKET, SO_BROADCAST, &bcastON, sizeof(bcastON)) == -1) {	//enable broadcasting
		perror("setsockopt (SO_BROADCAST)");
		exit(1);
	}
	printf(" \n");	//very important for some weird reason

	//prepare Broadcast message
	char myIP[] = "192.168.43.67";
	strcpy(buffer, "myIP:");
	strcpy(buffer, strcat(buffer,myIP));
	strcpy(buffer, strcat(buffer,"FILE:"));
	strcpy(buffer, strcat(buffer,file_path));
	strcpy(buffer, strcat(buffer,"\0"));
        puts(buffer);
	if(sendto(bcast_socket, buffer, strlen(buffer), 0,(struct sockaddr*) &bcastAddr, sizeof(bcastAddr)) <= 0) {
		perror("Broadcast failed : ");
		exit(1);
	}
	printf("Broadcasted request.\n");
	
	//listen for incoming offers
	int listen_socket;
	if((listen_socket = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("Error : Listen socket creation failed");
		exit(1);
	}
	if((bind(listen_socket,(struct sockaddr*) &myAddr, sizeof(myAddr)))<0) {
		perror("Error : Listen socket bind failed");
		exit(1);
	}
	if(listen(listen_socket, 1)<0) {
		perror("Error : Listening failed");
		exit(1);
	}

	printf("Listening for responses...\n");
	// struct pollfd data_poll;
	// data_poll.fd = listen_socket;
	// data_poll.events = POLLIN;

	// poll(&data_poll,1, 5000);

	// if(data_poll.revents != POLLIN) {	//exit on timeout for data transfer initiaion
	// 	printf("No responses!\n");
	// 	exit(0);
	// }
	if((data_socket = accept(listen_socket, (struct sockaddr*)&clntAddr, &clntAddrLen)) < 0) {
		perror("Error : accept failed");
		exit(1);
	}

	size_t file_size;
	if(recv(data_socket, &file_size, sizeof(size_t), 0) < 0) {	//get file size
		printf("Unable to establish connection!\n");
		exit(1);
	}

	if(file_size > 0) {	//read file
		printf("Reading file...\n");
		int bytes_read = 0;
		size_t total_bytes_read;
		FILE *file_fd = fopen(file_path, "w+");

		while(total_bytes_read < file_size) {
			bytes_read = recv(data_socket, buffer, PIECE_SIZE, 0);
			if(bytes_read <= 0)
				break;
			fwrite(buffer, sizeof(char), bytes_read, file_fd);
			total_bytes_read += bytes_read;
			printf("\r");
			int i, current_progress = (bytes_read/total_bytes_read) * 50;
			for(i = 0; i < current_progress; i++)
				printf("*");
			for(; i < 50; i++)
				printf("-");
		}
		printf("\r");
		fclose(file_fd);
		printf("Read %ld bytes into %s succesfully!\n", total_bytes_read, file_path);

	}
	else
		printf("File not found!\n");

	close(data_socket);
}
