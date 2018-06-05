#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define SERVER_PORT		(5000)
#define MAXLINE 1024

char server_addr[] = "147.46.119.144";

void strip_newline(char *s)
{
	while(*s != '\0')
	{
		if (*s == '\r' || *s == '\n')
			*s = '\0';
		s++;
	}
}


void *handle_recv(void *arg)
{
	int* pfd = (int*) arg;
	int	sockfd = (*pfd);
	int rlen;
	char buf[MAXLINE];

	memset(buf, 0x00, MAXLINE);
	while( (rlen = read(sockfd, buf, (MAXLINE-1))) > 0) 
	{
		buf[rlen] = '\0';
		strip_newline(buf);
		printf("[server]:%s\n", buf);
	}
	printf("handle_recv : end \n");
}


int main(int argc, char **argv) {
	struct sockaddr_in serveraddr;
	int server_sockfd;
	int client_len;
	char buf[MAXLINE];

	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("error : ");
		return 1;
	}
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(server_addr);
	serveraddr.sin_port = htons(SERVER_PORT);

	client_len = sizeof(serveraddr);

	if (connect(server_sockfd, (struct sockaddr*)&serveraddr, client_len) == -1) {
		perror("connect error : ");
		return 1;
	}

	pthread_t	tid;
	pthread_create(&tid, NULL, &handle_recv, (void*) &server_sockfd);

	while(1)
	{
		memset(buf, 0x00, MAXLINE);
		int rlen = read(0, buf, MAXLINE);
		if (buf[0] == '\n')
			break;

		if (write(server_sockfd, buf, rlen) <= 0) {
			perror("write error : ");
			return 1;
		}
	}
	close(server_sockfd);

	return 0;
}




