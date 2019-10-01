#ifndef __METHODS_H__
#define __METHODS_H__

struct sImagerRegister{
		uint16_t reg;
		uint16_t val;
}w;

void setCamExclusive(char* payload,int len,char** retbuf, int* rlen);
void setCamRegisters(char* payload,int len,char** retbuf, int* rlen);
void getCamRegisters(char* payload,int len,char** retbuf, int* rlen);
void eraseCamExclusive(char* payload,int len,char** retbuf, int* rlen);

#endif
