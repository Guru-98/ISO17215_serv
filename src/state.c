#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"

void dump_registers(void){
	printf("%s : %s\n","DHCP_EN",registers.enable_dhcp? "enable": "disable");
	printf("%s : %d.%d.%d.%d\n","MY IP",registers.m_ip.A,registers.m_ip.B,registers.m_ip.C,registers.m_ip.D);
	printf("%s : %d.%d.%d.%d\n","MY IP MASK",registers.m_ip_mask.A,registers.m_ip_mask.B,registers.m_ip_mask.C,registers.m_ip_mask.D);
	printf("%s : %s\n","HOST",&registers.dhcp_hostname);
	printf("%s : %d\n","STREAM PORT",(registers.stream_port.A<<8)+registers.stream_port.B);
	printf("%s : %d\n","COMM PORT",(registers.udp_comm_port.A<<8)+registers.udp_comm_port.B);

	/*
	printf("%x == %x\n",&registers.led_mode,&registers.reg[0]);
	printf("%x == %x\n",&registers.m_mac.A,&registers.reg[10]);
	printf("%x == %x\n",&registers.flash_mode,&registers.reg[16]);
	printf("%x == %x\n",&registers.m_streamid.B,&registers.reg[24]);
	printf("%x == %x\n",&registers.m_streamid.D,&registers.reg[26]);
	printf("%x == %x\n",&registers.m_streamid.E,&registers.reg[27]);
	printf("%x == %x\n",&registers.stream_ip.A,&registers.reg[31]);
	printf("%x == %x\n",&registers.stream_port.A,&registers.reg[41]);
	printf("%x == %x\n",&registers.udp_comm_port.B,&registers.reg[46]);
	printf("%x == %x\n",&registers.stream_src_port.B,&registers.reg[48]);
	*/
}
unsigned char gateway[32];
void fileout(void){
	FILE *fp,*network;
	char strin[512];
	char strout[512];
	int linectr = 0;
	fp = fopen("network_settings.txt","r");
	network = fopen("network","w+");

	if(fp == NULL || network == NULL){
		printf("FILE IO ERROR\n");
		exit(-1);
	}
	if(feof(fp)){
		printf("EMPTY FILE\n");
		exit(-1);
	}

	while(!feof(fp)){
		fgets(strin, 512, fp);
		if(!feof(fp)){
			++linectr;
		}
		printf("-%d : %s",linectr,strin);
		switch(linectr){
			case 1:
				sprintf(strout,"%s\n",registers.enable_dhcp? "enable": "disable");
			break;
			case 2:
				sprintf(strout,"%d.%d.%d.%d\n",registers.m_ip.A,registers.m_ip.B,registers.m_ip.C,registers.m_ip.D);
			break;
			case 3:
				sprintf(strout,"%d.%d.%d.%d\n",registers.m_ip_mask.A,registers.m_ip_mask.B,registers.m_ip_mask.C,registers.m_ip_mask.D);
			break;
			case 4:
			        memcpy(strout,gateway,strlen(gateway));
			        
			break;
			case 9:
				sprintf(strout,"%s\n",&registers.dhcp_hostname);
			break;
			case 20:
				sprintf(strout,"%d\n",(registers.stream_port.A<<8)+registers.stream_port.B);
			break;
			case 19:
				sprintf(strout,"%d\n",(registers.udp_comm_port.A<<8)+registers.udp_comm_port.B);
			break;
			default:
				sprintf(strout,"%s",strin);
		}
		printf("+%d : %s",linectr,strout);
		fputs(strout, network);
	}

	fclose(fp);
	fclose(network);

	remove("network_settings.txt");
	rename("network", "network_settings.txt");
}

void filein(void){
	FILE *fp,*network;
	char strin[512];
	char strout[512];
	uint16_t data;
	int linectr = 0;
	fp = fopen("network_settings.txt","r");
	network = fopen("network","w+");

	if(fp == NULL || network == NULL){
		printf("FILE IO ERROR\n");
		exit(-1);
	}
	if(feof(fp)){
		printf("EMPTY FILE\n");
		exit(-1);
	}

	while(!feof(fp)){
		fgets(strin, 512, fp);
		if(!feof(fp)){
			++linectr;
		}
		printf("-%d : %s",linectr,strin);
		switch(linectr){
			case 1:
				//sscanf(strin,"%s\n",registers.enable_dhcp? "enable": "disable");
			break;
			case 2:
				sscanf(strin,"%d.%d.%d.%d\n",&registers.m_ip.A,&registers.m_ip.B,&registers.m_ip.C,&registers.m_ip.D);
			break;
			case 3:
				sscanf(strin,"%d.%d.%d.%d\n",&registers.m_ip_mask.A,&registers.m_ip_mask.B,&registers.m_ip_mask.C,&registers.m_ip_mask.D);
			break;
			case 9:
				sscanf(strin,"%s\n",registers.dhcp_hostname);
			break;
		}
	}

	fclose(fp);
}
