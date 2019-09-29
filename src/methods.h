#ifndef __METHODS_H__
#define __METHODS_H__

void setCamExclusive(char* payload,int len,char** retbuf, int* rlen);
void setCamRegisters(char* payload,int len,char** retbuf, int* rlen);
void eraseCamExclusive(char* payload,int len,char** retbuf, int* rlen);

#endif
