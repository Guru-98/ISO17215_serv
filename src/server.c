#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	17215
#define MAXLINE 1024

struct someip_t{
	union{
		struct{
			union{
				struct{
					uint16_t methodID;
					uint16_t serviceID;
				}f;
				uint32_t b;
			}messageID;
			uint32_t length;
			union{
				struct{
					uint16_t sessionID;
					uint16_t clientID;
				}f;
				uint32_t b;
			}requestID;
			uint8_t proto_v;
			uint8_t iface_v;
			uint8_t msg_typ;
			uint8_t retcode;
		}f;
		uint8_t b[16];
	}header;
	char *payload;
};

void process_payload(char* payload, int len);
void process_someip(struct someip_t* packet);
void find_someip(char* buffer, int maxlen, int* nextpos, struct someip_t* packet);
void process_someip(struct someip_t* pakcet);
void dump(char* buffer, int len);

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

void find_someip(char* buffer, int maxlen, int* nextpos, struct someip_t* packet){
	*nextpos = 0;

	if(maxlen < sizeof(packet->header.b)){
		*nextpos = -1;
		printf("head len error");
		return;
	}

	memcpy(packet->header.b, buffer , sizeof(packet->header.b));
	buffer += sizeof(packet->header.b);
	*nextpos += sizeof(packet->header.b);

	packet->header.f.messageID.b = htonl(packet->header.f.messageID.b);
    packet->header.f.length = htonl(packet->header.f.length);
    packet->header.f.requestID.b = htonl(packet->header.f.requestID.b);

	if(maxlen-((*nextpos)+8) < packet->header.f.length){
		*nextpos = -1;
		printf("payload error");
		return;
	}

    packet->payload = (char *) malloc(packet->header.f.length-8);
	memcpy(packet->payload, buffer,  packet->header.f.length-8);
	buffer += packet->header.f.length-8;
	*nextpos += packet->header.f.length-8;
}

void process_someip(struct someip_t* packet,struct someip_t* ret){
	memset(ret, 0, sizeof(ret));
	memcpy(ret, packet, sizeof(packet->header.b));
	memset(ret->length, 0, sizeof(ret->length));
	
	char *f_buf = packet->payload;
	int f_len = packet->header.f.length-8;
	
	char *r_buf = ret->payload;
	int r_len = ret->header.f.length;	
	
	switch(packet->methodID){
		case 0x11:
			setCamExclusive(f_buf,f_len,&r_buf,&r_len);
			break;
		case 0x302:
			setCamRegisters(f_buf,f_len,&r_buf,&r_len);
			break;
		case 0x19;
			eraseCamExclusive(f_buf,f_len,&r_buf,&r_len);
			break;
	}
}

void setCamExclusive(char* payload,int len,char** retbuf, int* rlen){
	
}

void setCamRegisters(char* payload,int len,char** retbuf, int* rlen){
	
}

void eraseCamExclusive(char* payload,int len,char** retbuf, int* rlen){
	
}

void dump_someip(struct someip_t* packet){
	printf("MessageID: %X\n",packet->header.f.messageID.b);
	printf(" +> ServiceID: %X ; MethodID: %X\n",
						packet->header.f.messageID.f.serviceID,
						packet->header.f.messageID.f.methodID );
	printf("Length: %u (%04X)\n",packet->header.f.length,packet->header.f.length);
	printf("RequestID: %X\n",packet->header.f.requestID.b);
	printf(" +> ClientID: %X ; SessionID: %X\n",
						packet->header.f.requestID.f.clientID,
						packet->header.f.requestID.f.sessionID );
	printf("Protocol_v: %X ; Interface_v: %X ; ",
						packet->header.f.proto_v,
						packet->header.f.iface_v );
	printf("Message_t: %X ; Return_c: %X\n",
						packet->header.f.msg_typ,
						packet->header.f.retcode );
	printf("Payload: \n");
	dump(packet->payload, packet->header.f.length-8);
}

void dump(char* buffer, int len){
    for(int i = 0; i<len;){
    	for (int j = 0; i<len && j < 4; j++){
    		printf(" %02X ", buffer[i++]);
    	}
    	printf("\n");
    }
    printf("\n\n");
}
