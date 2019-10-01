#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "someip.h"

void find_someip(char* buffer, int maxlen, int* nextpos, struct someip_t* packet){
	*nextpos = 0;

	if(maxlen < sizeof(packet->header.b)){
		*nextpos = -1;
		printf("head len error");
		exit(-1);
	}

	memcpy(packet->header.b, buffer , sizeof(packet->header.b));
	buffer += sizeof(packet->header.b);
	*nextpos += sizeof(packet->header.b);

	packet->header.f.messageID.b = htonl(packet->header.f.messageID.b); 
    packet->header.f.length = htonl(packet->header.f.length);
    packet->header.f.requestID.b = htonl(packet->header.f.requestID.b);
	
	//dump_someip(packet);

	//printf("[*] ML: %d, NP: %d, HL: %d\n << %d < %d >>?\n\n",maxlen, *nextpos, packet->header.f.length, maxlen-((*nextpos)+8), packet->header.f.length);

	if((maxlen-(*nextpos)) < packet->header.f.length-8){
		*nextpos = -1;
		printf("payload error");
		exit(-1);
	}

    packet->payload = (char *) malloc(packet->header.f.length-8);
	memcpy(packet->payload, buffer,  packet->header.f.length-8);
	buffer += packet->header.f.length-8;
	*nextpos += packet->header.f.length-8;
}

void pac2buf(struct someip_t* packet, char** buffer){
	if(*buffer == NULL)
		*buffer = malloc(sizeof(packet->header)+packet->header.f.length-8);
	
	memcpy(*buffer, packet, sizeof(packet->header)+packet->header.f.length-8);
	memcpy(*buffer+sizeof(packet->header), packet->payload, packet->header.f.length-8);
	
	for(int i=0; i < 3; i++){
		uint32_t t;
		
		memcpy(&t, (*buffer)+(i*sizeof(uint32_t)), sizeof(uint32_t));
		t = htonl(t);
		memcpy((*buffer)+(i*sizeof(uint32_t)), &t, sizeof(uint32_t));
	}
}

void process_someip(struct someip_t* packet,struct someip_t* ret){
	memset(ret, 0, sizeof(ret));
	memcpy(ret, packet, sizeof(packet->header.b));
	memset(&(ret->header.f.length), 0, sizeof(ret->header.f.length));

	char *f_buf = packet->payload;
	int f_len = packet->header.f.length-8;

	char **r_buf = &ret->payload;
	uint32_t* r_len = &ret->header.f.length;
	
	ret->header.f.msg_typ = 0x80;

	switch(packet->header.f.messageID.f.methodID){
		case 0x11:
			setCamExclusive(f_buf,f_len,r_buf,r_len);
			break;
		case 0x302:
			setCamRegisters(f_buf,f_len,r_buf,r_len);
			break;
		case 0x19:
			eraseCamExclusive(f_buf,f_len,r_buf,r_len);
			break;
		case 0x304:
			getCamRegisters(f_buf,f_len,r_buf,r_len);
			break;
		default:
			printf("NO METHOD: %04X", packet->header.f.messageID.f.methodID);
			exit(-1);
	}
	
	*r_len += 8;
}

void dump_someip(struct someip_t* packet){
	printf("MessageID: %08X\n",packet->header.f.messageID.b);
	printf(" +> ServiceID: %04X ; MethodID: %04X\n",
						packet->header.f.messageID.f.serviceID,
						packet->header.f.messageID.f.methodID );
	printf("Length: %u (%08X)\n",packet->header.f.length,packet->header.f.length);
	printf("RequestID: %X\n",packet->header.f.requestID.b);
	printf(" +> ClientID: %04X ; SessionID: %04X\n",
						packet->header.f.requestID.f.clientID,
						packet->header.f.requestID.f.sessionID );
	printf("Protocol_v: %02X ; Interface_v: %02X ; ",
						packet->header.f.proto_v,
						packet->header.f.iface_v );
	printf("Message_t: %02X ; Return_c: %02X\n",
						packet->header.f.msg_typ,
						packet->header.f.retcode );
	printf("Payload: \n");
	dump(packet->payload, packet->header.f.length-8);
}

void dump(char* buffer, int len){
	if(buffer == NULL){
		printf("<<Empty Buffer>>\n\n");
		return;
	}
    for(int i = 0; i<len;){
    	for (int j = 0; i<len && j < 4; j++){
    		printf(" %02X ", buffer[i++]);
    	}
    	printf("\n");
    }
    printf("\n\n");
}
