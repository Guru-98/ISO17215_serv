#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "someip.h"
#include "state.h"

#define PORT	17215
#define MAXLINE 1024

void process_payload(char* payload, int len, char** ret_payload, int* ret_len);

int main() {
	int sd;
	char buffer[MAXLINE];
	struct sockaddr_in me, you;
	struct in_addr localaddr;

	char *ret_buf;
	int ret_len;

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


	while(1){
		int you_len, n;
		memset(buffer, 0 , sizeof(buffer));
		n = recvfrom(sd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &you, &you_len);

		client = you.sin_addr.s_addr;

		//printf("CLIENT : %s\n",inet_ntoa(you.sin_addr));
		//printf("D_CLIENT: %d(%04X)\n",n,n);
		//dump(buffer, n);
		if(lock == 0 || (lock == 1 && lockedby == you.sin_addr.s_addr))
			process_payload(buffer, n, &ret_buf, &ret_len);

		//printf("RESP:\n");
		//dump(ret_buf,ret_len);

		if(sendto(sd, (const char *)ret_buf, ret_len, MSG_CONFIRM, (const struct sockaddr *) &you, you_len) <0 ){
			perror("response error");
			exit(EXIT_FAILURE);
		}

		client = 0;
	}

	return 0;
}

void process_payload(char* payload, int maxlen, char** ret_payload, int* ret_len){
	struct someip_t packet;
	struct someip_t ret_pack;
	int i=0,pos=0,len=maxlen;

	*ret_len = 0;
	*ret_payload = NULL;

	while(i<maxlen){
		char* temp = NULL;

        memset(&packet, 0 , sizeof(struct someip_t));
		find_someip(payload+i,len,&pos,&packet);
		if(pos == -1)
			break;
        i+=pos;
		len-=pos;

		//printf("\n\nREQUEST %04X\n",packet.header.f.requestID.f.sessionID+1);
		//dump_someip(&packet);

		process_someip(&packet,&ret_pack);

		//printf("\n\nREPLY %04X\n",ret_pack.header.f.requestID.f.sessionID+1);
		//dump_someip(&ret_pack);

		//printf("Before realloc: (%d B)\n",*ret_len);
		//dump(*ret_payload,*ret_len);

		pac2buf(&ret_pack, &temp);

		//printf("Buffer to add:");
		//dump(temp,sizeof(ret_pack.header)+ret_pack.header.f.length-8);

		*ret_payload = realloc(*ret_payload, *ret_len+sizeof(ret_pack.header)+ret_pack.header.f.length-8);
		memcpy((*ret_payload)+(*ret_len), temp, sizeof(ret_pack.header)+ret_pack.header.f.length-8);
		*ret_len += sizeof(ret_pack.header)+ret_pack.header.f.length-8;

		//printf("After realloc: (%d B)\n",*ret_len);
		//dump(*ret_payload,*ret_len);
	}
}