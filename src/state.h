#ifndef __STATE_H__
#include <stdint.h>

union{
	struct{
		uint16_t led_mode;		//0 //b00c
		uint16_t stream_proto;	//1 //b041
		struct {
			uint16_t A;			//2 //b042
			uint16_t B;			//3 //b043
			uint16_t C;			//4 //b044
			uint16_t D;			//5 //b045
		} m_ip;
		struct {
			uint16_t A;			//6 //b046
			uint16_t B;			//7 //b047
			uint16_t C;			//8 //b048
			uint16_t D;			//9 //b049
		} m_ip_mask;
		struct {
			uint16_t A;			//10 //b04a
			uint16_t B;			//11 //b04b
			uint16_t C;			//12 //b04c
			uint16_t D;			//13 //b04d
			uint16_t E;			//14 //b04e
			uint16_t F;			//15 //b04f
		} m_mac;
		uint16_t flash_mode;	//16 //b050
		uint16_t sensor_temp;	//17 //b051
		uint16_t board_temp;	//18 //b052
		uint16_t led_temp;		//19 //b053
		uint16_t led_percentage;//20 //b054
		struct {
			uint16_t A;			//21 //b055
			uint16_t B;			//22 //b056
		} m_vlanid;
		struct {
			uint16_t A;			//23 //b057
			uint16_t B;			//24 //b058
			uint16_t C;			//25 //b059
			uint16_t D;			//26 //b05a
			uint16_t E;			//27 //b05b
			uint16_t F;			//28 //b05c
			uint16_t G;			//29 //b05d
			uint16_t H;			//30 //b05e
		} m_streamid;
		struct {
			uint16_t A;			//31 //b05f
			uint16_t B;			//32 //b060
			uint16_t C;			//33 //b061
			uint16_t D;			//34 //b062
		} stream_ip;
		struct {
			uint16_t A;			//35 //b063
			uint16_t B;			//36 //b064
			uint16_t C;			//37 //b065
			uint16_t D;			//38 //b066
			uint16_t E;			//39 //b067
			uint16_t F;			//40 //b068
		} stream_mac;
		struct {
			uint16_t A;			//41 //b069
			uint16_t B;			//42 //b06a
		} stream_port;
		uint16_t selected_roi;	//43 //b06b
		uint16_t nostream_boot;	//44 //b06c
		struct {
			uint16_t A;			//45 //b06d
			uint16_t B;			//46 //b06e
		} udp_comm_port;
		struct {
			uint16_t A;			//47 //b06f
			uint16_t B;			//48 //b070
		} stream_src_port;
		uint16_t dynamic_range;	//49 //b071
		uint16_t enable_overlay;//50 //b072
		uint16_t enable_dhcp;	//51 //b073
		uint16_t enable_rtsp;   //52 //b077 //implemeted in batch file
        uint16_t wait_mac;		//53 //b078
		uint16_t wait_ptpsync;	//54 //b079
		uint16_t dhcp_hostname[16];	//55-69 //b171-b182
	};
	uint16_t reg[70];
}registers;

struct __attribute__((__packed__)){
    uint32_t roiIndex;
    uint16_t upl_x;
    uint16_t upl_y;
    uint16_t dnr_x;
    uint16_t dnr_y;
    uint8_t videoFormat_en;
    uint8_t transMethod;
    uint16_t transCycle;
    uint16_t width;
    uint16_t height;
    uint32_t framerate;
    uint8_t interlaced_en;
    uint8_t colourSpace;
    uint32_t bitrate;
    uint8_t compression_type;   //0 - uncompressed; 1 - jpeg; 2 - H.264
    uint8_t hist_en;
    uint8_t hist_updateCycle;
    uint8_t usedVideoComponent;
    uint8_t dataType;
    uint8_t binsize;
    uint16_t noBins;
}inp_setROI;

uint8_t lock;
uint32_t lockedby;
uint32_t client;

void fileout(void);
void filein(void);

void dump_registers(void);

#endif
