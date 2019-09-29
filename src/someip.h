#ifndef __SOMEIP_H__
#define __SOMEIP_H__

#include <sys/types.h>
#include <sys/socket.h>

#include "methods.h"

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

void find_someip(char* buffer, int maxlen, int* nextpos, struct someip_t* packet);
void process_someip(struct someip_t* pakcet, struct someip_t* ret);
void dump(char* buffer, int len);

#endif
