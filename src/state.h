#ifndef __STATE_H__

struct ip_t{
	uint16_t A;
	uint16_t B;
	uint16_t C;
	uint16_t D;
};

extern struct ip_t m_ip;
extern struct ip_t m_ip_mask;

#endif
