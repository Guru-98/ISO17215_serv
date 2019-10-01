#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <arpa/inet.h>

#include "someip.h"
#include "state.h"

void _setRegister(uint16_t reg, uint16_t val);
uint16_t _getRegister(uint16_t regAddress);
void _setRegisters(struct sImagerRegister* regBlock, uint16_t no_reg);


void setCamExclusive(char* payload,int len,char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);
	*rlen = 0;
	*retbuf = NULL;
	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void setCamRegisters(char* payload,int len,char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);
	uint16_t no_reg = 0;
	struct sImagerRegister *regBlock;
	
	//printf("\t%s: No. reg: %d (%08X)\n", __FUNCTION__, no_reg, no_reg);
	
	memcpy(&no_reg, payload, sizeof(no_reg));
	no_reg = htons(no_reg);
	
	//printf("\t%s: Payload 16B\n", __FUNCTION__);
	//dump(payload,16);
	//printf("\t%s: No. reg: %d (%08X)\n", __FUNCTION__, no_reg, no_reg);
	
	regBlock = (struct sImagerRegister*) malloc(sizeof(struct sImagerRegister) * no_reg );
	if(regBlock == NULL){
		printf("NOMEM");
		return;
	}
	memcpy(regBlock, payload+sizeof(no_reg), sizeof(struct sImagerRegister) * no_reg);

	//dump((char *)regBlock, sizeof(struct sImagerRegister) * no_reg);

	for(int i = 0; i < no_reg; i++){
		regBlock[i].reg = htons(regBlock[i].reg);
		regBlock[i].val = htons(regBlock[i].val);
	}

	//dump((char *)regBlock, sizeof(struct sImagerRegister) * no_reg);

	_setRegisters(regBlock, no_reg);
	
	//dump((char *)&m_ip, sizeof(m_ip));
	//dump((char *)&m_ip_mask, sizeof(m_ip_mask));

	*rlen = 0;
	*retbuf = NULL;
	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void getCamRegisters(char* payload,int len,char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);
	uint16_t no_reg = 0;
	uint16_t *reg_list;
	
	struct sImagerRegister *regBlock;
	
	memcpy(&no_reg, payload, sizeof(no_reg));
	no_reg = htons(no_reg);
	
	reg_list = (uint16_t *) malloc(sizeof(uint16_t) * no_reg);
	if(reg_list == NULL){
		printf("NOMEM");
		exit(-1);
	}
	
	memcpy(reg_list, payload+sizeof(no_reg), sizeof(uint16_t) * no_reg);
	
	regBlock = (struct sImagerRegister*) malloc(sizeof(struct sImagerRegister) * no_reg );
	if(regBlock == NULL){
		printf("NOMEM");
		return;
	}
	
	for(int i=0; i <no_reg; i++){
		reg_list[i] = htons(reg_list[i]);
	}
	
	for(int i=0; i <no_reg; i++){
		regBlock[i].reg = reg_list[i];
		regBlock[i].val = _getRegister(reg_list[i]);
	}
	
	for(int i=0; i <no_reg; i++){
		regBlock[i].reg = htons(regBlock[i].reg);
		regBlock[i].val = htons(regBlock[i].val);
	}
	
	//dump((char *)regBlock, sizeof(struct sImagerRegister) * no_reg);
	dump_registers();
	
	*rlen = sizeof(struct sImagerRegister) * no_reg + sizeof(no_reg);
	*retbuf = (char *) malloc(*rlen + sizeof(no_reg));
	memcpy(*retbuf, (char *)&no_reg, sizeof(no_reg));
	memcpy(*retbuf+sizeof(no_reg), (char *)regBlock, *rlen);
	
	//printf("\t[*] %x",*retbuf);
	
	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void eraseCamExclusive(char* payload,int len,char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);
	*rlen = 0;
	*retbuf = NULL;
	printf("METHOD OUT: %s\n",__FUNCTION__);
}


void _setRegisters(struct sImagerRegister* regBlock, uint16_t no_reg){
	for(int i=0; i<no_reg; i++){
		_setRegister(regBlock[i].reg, regBlock[i].val);
	}
}

/*
const struct sImagerRegister* _getRegisters(uint16_t* addressList, uint16_t regCount){
	const struct sImagerRegister *imagerRegisterBlock;
	imagerRegisterBlock = malloc(sizeof(struct sImagerRegister) * regCount);
	
	for(int i=0; i<no_reg; i++){
		imagerRegisterBlock[i] = _getRegister(regBlock[i]);
	}
	
	return imagerRegisterBlock;
}
*/

uint16_t _getRegister(uint16_t regAddress){
	uint16_t regValue;

	if( 0xB041 <= regAddress && regAddress <= 0xB079 ){
		regValue = registers.reg[regAddress-0xB040];
	}
	else if( 0xB00C == regAddress ){
		regValue = registers.reg[0];
	}
	else if ( 0xB171 <= regAddress && regAddress <= 0xB181 ){
		regValue = registers.reg[56+regAddress-0xB171];
	}
	else{
		printf("No such register: %04X\n", regAddress);
		exit(-1);
	}
	
	return regValue;
}

void _setRegister(uint16_t regAddress, uint16_t regValue){
	if( 0xB041 <= regAddress && regAddress <= 0xB079 ){
		registers.reg[regAddress-0xB040] = regValue;
	}
	else if( 0xB00C == regAddress ){
		registers.reg[0] = regValue;
	}
	else if ( 0xB171 <= regAddress && regAddress <= 0xB181 ){
		registers.reg[56+regAddress-0xB171] = regValue;
	}
	else{
		printf("No such register: %04X\n", regAddress);
		exit(-1);
	}
}
