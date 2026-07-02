#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

typedef struct {
	void (*init)(void);
} Delay_Struct_t;

extern Delay_Struct_t delay;

void Ip_Delay_Init(void);
void Ip_Delay_Ms(uint32_t ms);
void Ip_Delay_Us(uint32_t us);
extern unsigned long Ip_Get_Tick(void);

#endif /* DELAY_H */
