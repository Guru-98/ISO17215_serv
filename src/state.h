#ifndef __STATE_H__
#include <stdint.h>

union{
	struct{
		uint16_t led_mode;		//0
		uint16_t stream_proto;	//1
		struct {
			uint16_t A;			//2
			uint16_t B;			//3
			uint16_t C;			//4
			uint16_t D;			//5
		} m_ip;
		struct {
			uint16_t A;			//6
			uint16_t B;			//7
			uint16_t C;			//8
			uint16_t D;			//9
		} m_ip_mask;
		struct {
			uint16_t A;			//10
			uint16_t B;			//11
			uint16_t C;			//12
			uint16_t D;			//13
			uint16_t E;			//14
			uint16_t F;			//15
		} m_mac;
		uint16_t flash_mode;	//16
		uint16_t sensor_temp;	//17
		uint16_t board_temp;	//18
		uint16_t led_temp;		//19
		uint16_t led_percentage;//20
		struct {
			uint16_t A;			//21
			uint16_t B;			//22
		} m_vlanid;
		struct {
			uint16_t A;			//23
			uint16_t B;			//24
			uint16_t C;			//25
			uint16_t D;			//26
			uint16_t E;			//27
			uint16_t F;			//28
			uint16_t G;			//29
			uint16_t H;			//30
		} m_streamid;
		struct {
			uint16_t A;			//31
			uint16_t B;			//32
			uint16_t C;			//33
			uint16_t D;			//34
		} stream_ip;
		struct {
			uint16_t A;			//35
			uint16_t B;			//36
			uint16_t C;			//37
			uint16_t D;			//38
			uint16_t E;			//39
			uint16_t F;			//40
		} stream_mac;
		struct {
			uint16_t A;			//41
			uint16_t B;			//42
		} stream_port;
		uint16_t selected_roi;	//43
		uint16_t nostream_boot;	//44
		struct {
			uint16_t A;			//45
			uint16_t B;			//46
		} udp_comm_port;
		struct {
			uint16_t A;			//47
			uint16_t B;			//48
		} stream_src_port;
		uint16_t dynamic_range;	//49
		uint16_t enable_overlay;//50
		uint16_t enable_dhcp;	//51
		uint16_t wait_mac;		//52
		uint16_t wait_ptpsync;	//53
		uint16_t dhcp_hostname[16];	//54-69
	};
	uint16_t reg[70];
}registers;

uint8_t lock;
uint32_t lockedby;
uint32_t client;

void fileout(void);
void filein(void);

void dump_registers(void);

#endif
