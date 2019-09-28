#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "someip.h"

#define PORT	17215
#define MAXLINE 1024

void process_payload(char* payload, int len);

int main() {
	int sd;
	char buffer[MAXLINE];
	struct sockaddr_in me, you;
	struct in_addr localaddr;

	char* hello = "OK";

	if ( (sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&me, 0, sizeof(me));
	memset(&you, 0, sizeof(you));

	me.sin_family = AF_INET;
	me.sin_addr.s_addr = htonl(INADDR_ANY);
	me.sin_port = htons(PORT);

   	if ( bind(sd, (struct sockaddr *)&me, sizeof(me)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	char *ifname = "usb0";
    	if(setsockopt(sd, SOL_SOCKET, SO_BINDTODEVICE, ifname, sizeof(ifname)) != -1){
        	perror("bind2dev error");
	        exit(EXIT_FAILURE);
    	}

	int you_len, n;
	memset(buffer, 0 , sizeof(buffer));
	n = recvfrom(sd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &you, &you_len);

   	printf("CLIENT : %s\n",inet_ntoa(you.sin_addr));
   	process_payload(buffer, n);

	if(sendto(sd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &you, you_len) <0 ){
		perror("response error");
		exit(EXIT_FAILURE);
	}
	printf("Hello message sent.\n");

	return 0;
}

void process_payload(char* payload, int len){
	struct someip_t packet;
	struct someip_t ret;
	int i=0,pos=0;

	while(i<len){
        memset(&packet, 0 , sizeof(struct someip_t));
		find_someip(payload+i,len,&pos,&packet);
		if(pos == -1)
			break;
        i+=pos;
		process_someip(&packet,&ret);
	}
}