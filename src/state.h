#ifndef __STATE_H__
#include <stdint.h>

union{
	struct{
		uint16_t led_mode;
		uint16_t stream_proto;
		struct {
			uint16_t A;
			uint16_t B;
			uint16_t C;
			uint16_t D;
		} m_ip;
		struct {
			uint16_t A;
			uint16_t B;
			uint16_t C;
			uint16_t D;
		} m_ip_mask;
		struct {
			uint16_t A;
			uint16_t B;
			uint16_t C;
			uint16_t D;
			uint16_t E;
			uint16_t F;
		} m_mac;
		uint16_t flash_mode;
		uint16_t sensor_temp;
		uint16_t board_temp;
		uint16_t led_temp;
		uint16_t led_percentage;
		struct {
			uint16_t A;
			uint16_t B;
		} m_vlanid;
		struct {
			uint16_t A;
			uint16_t B;
			uint16_t C;
			uint16_t D;
			uint16_t E;
			uint16_t F;
			uint16_t G;
			uint16_t H;
		} m_streamid;
		struct {
			uint16_t A;
			uint16_t B;
			uint16_t C;
			uint16_t D;
		} stream_ip;
		struct {
			uint16_t A;
			uint16_t B;
			uint16_t C;
			uint16_t D;
			uint16_t E;
			uint16_t F;
		} stream_mac;
		struct {
			uint16_t A;
			uint16_t B;
		} stream_port;
		uint16_t selected_roi;
		uint16_t nostream_boot;
		struct {
			uint16_t A;
			uint16_t B;
		} udp_comm_port;
		struct {
			uint16_t A;
			uint16_t B;
		} stream_src_port;
		uint16_t dynamic_range;
		uint16_t enable_overlay;
		uint16_t enable_dhcp;
		uint16_t wait_mac;
		uint16_t wait_ptpsync;
		uint16_t dhcp_hostname[16];
	};
	uint16_t reg[57];
}registers;

void dump_registers(void);

#endif
