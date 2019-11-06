#ifndef __METHODS_H__
#define __METHODS_H__

struct sImagerRegister{
		uint16_t reg;
		uint16_t val;
};

void setCamExclusive(char* payload,int len,char** retbuf, int* rlen);
void setCamRegisters(char* payload,int len,char** retbuf, int* rlen);
void getCamRegisters(char* payload,int len,char** retbuf, int* rlen);
void eraseCamExclusive(char* payload,int len,char** retbuf, int* rlen);
void setRegionOfInterest(char* payload,int len,char** retbuf, int* rlen);
void setRegionsOfInterest(char* payload,int len,char** retbuf, int* rlen);
void getRegionOfInterest(char* payload,int len,char** retbuf, int* rlen);
void getRegionsOfInterest(char* payload,int len,char** retbuf, int* rlen);
void eraseRegionOfInterest(char* payload,int len,char** retbuf, int* rlen);
void subscribeROIVideo(char* payload,int len,char** retbuf, int* rlen);
void unsubscribeROIVideo(char* payload,int len,char** retbuf, int* rlen);

#endif
