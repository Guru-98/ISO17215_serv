#include <stdio.h>
#include <stdlib.h>

#include "state.h"

void dump_registers(void){
	printf("%s : %d.%d.%d.%d\n","MY IP",registers.m_ip.A,registers.m_ip.B,registers.m_ip.C,registers.m_ip.D);
	printf("%s : %d.%d.%d.%d\n","MY IP MASK",registers.m_ip_mask.A,registers.m_ip_mask.B,registers.m_ip_mask.C,registers.m_ip_mask.D);

	printf("%x == %x\n", &registers, &registers.reg);

	printf("%x == %x\n", &registers.m_ip.A, &registers.reg[2]);
	printf("%x == %x\n", &registers.m_ip.B, &registers.reg[3]);
	printf("%x == %x\n", &registers.m_ip.C, &registers.reg[4]);
	printf("%x == %x\n", &registers.m_ip.D, &registers.reg[5]);
}

void fileout(void){
	FILE *fp,*temp;
	char str[512];
	int linectr = 0;
	fp = fopen("network_settings.txt","r");
	temp = fopen("temp","w+");

	if(fp == NULL || temp == NULL){
		printf("FILE IO ERROR\n");
		exit(-1);
	}
	if(feof(fp)){
		printf("EMPTY FILE\n");
		exit(-1);
	}

	while(!feof(fp)){
		fgets(str, 512, fp);
		if(!feof(fp)){
			linectr++;
		}
		if(linectr == 2){
			fprintf(temp,"%d.%d.%d.%d\n",registers.m_ip.A,registers.m_ip.B,registers.m_ip.C,registers.m_ip.D);
		}
		else if(linectr == 3){
			fprintf(temp,"%d.%d.%d.%d\n",registers.m_ip_mask.A,registers.m_ip_mask.B,registers.m_ip_mask.C,registers.m_ip_mask.D);
		}
		else{
			fputs(str, temp);
		}
	}

	fclose(fp);
	fclose(temp);

	remove("network_settings.txt");
	rename("temp", "network_settings.txt");
}