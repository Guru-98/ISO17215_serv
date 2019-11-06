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

	lock = 1;
	lockedby = client;

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
	//dump_registers();

	*rlen = sizeof(struct sImagerRegister) * no_reg + sizeof(no_reg);
	*retbuf = (char *) malloc(*rlen + sizeof(no_reg));
	memcpy(*retbuf, (char *)&no_reg, sizeof(no_reg));
	memcpy(*retbuf+sizeof(no_reg), (char *)regBlock, *rlen);

	//printf("\t[*] %x",*retbuf);

	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void eraseCamExclusive(char* payload,int len,char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);

    fileout();

	lock = 0;
	lockedby = 0;

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
		regValue = registers.reg[regAddress-0xB041+1];
	}
	else if( 0xB00C == regAddress ){
		regValue = registers.reg[0];
	}
	else if ( 0xB171 <= regAddress && regAddress <= 0xB181 ){
		regValue = registers.reg[54+regAddress-0xB171];
	}
	else{
		printf("No such register: %04X\n", regAddress);
		exit(-1);
	}

	return regValue;
}

void _setRegister(uint16_t regAddress, uint16_t regValue){
	if( 0xB041 <= regAddress && regAddress <= 0xB079 ){
		registers.reg[regAddress-0xB041+1] = regValue;
	}
	else if( 0xB00C == regAddress ){
		registers.reg[0] = regValue;
	}
	else if ( 0xB171 <= regAddress && regAddress <= 0xB181 ){
		registers.reg[54+regAddress-0xB171] = regValue;
	}
	else{
		printf("No such register: %04X\n", regAddress);
		exit(-1);
	}
}

void setRegionOfInterest(char* payload, int len, char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);

    dump(payload,len);
    printf("strctlen:%d ; paylen:%d",sizeof(inp_setROI),len);

    memcpy(&inp_setROI, payload, sizeof(inp_setROI));

    #define S16(X) ((X) = htons(X))
    #define S32(X) ((X) = htonl(X))
    S32(inp_setROI.roiIndex);
    S16(inp_setROI.upl_x);
    S16(inp_setROI.upl_y);
    S16(inp_setROI.dnr_x);
    S16(inp_setROI.dnr_y);
    S16(inp_setROI.transCycle);
    S16(inp_setROI.width);
    S16(inp_setROI.height);
    S32(inp_setROI.framerate);
    S32(inp_setROI.bitrate);
    S16(inp_setROI.noBins);

    printf("width:%x\n",inp_setROI.width);
    printf("height:%x\n",inp_setROI.height);
    printf("bitrate:%x\n",inp_setROI.bitrate);
    printf("codec:%x\n",inp_setROI.compression_type);

	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void setRegionsOfInterest(char* payload, int len, char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);

    

	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void getRegionOfInterest(char* payload, int len, char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);

    

	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void getRegionsOfInterest(char* payload, int len, char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);

    

	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void eraseRegionOfInterest(char* payload, int len, char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);

    

	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void subscribeROIVideo(char* payload, int len, char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);

    

	printf("METHOD OUT: %s\n",__FUNCTION__);
}

void unsubscribeROIVideo(char* payload, int len, char** retbuf, int* rlen){
	printf("METHOD: %s\n",__FUNCTION__);

    

	printf("METHOD OUT: %s\n",__FUNCTION__);
}
