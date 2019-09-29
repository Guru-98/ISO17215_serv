#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "someip.h"

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
	memset(&(ret->header.f.length), 0, sizeof(ret->header.f.length));

	char *f_buf = packet->payload;
	int f_len = packet->header.f.length-8;

	char *r_buf = ret->payload;
	int r_len = ret->header.f.length;

	switch(packet->header.f.messageID.f.methodID){
		case 0x11:
			setCamExclusive(f_buf,f_len,&r_buf,&r_len);
			break;
		case 0x302:
			setCamRegisters(f_buf,f_len,&r_buf,&r_len);
			break;
		case 0x19:
			eraseCamExclusive(f_buf,f_len,&r_buf,&r_len);
			break;
	}
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
