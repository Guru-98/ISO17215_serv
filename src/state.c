#include <stdio.h>

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
