#ifndef IP_LINE_SENSOR_H
#define IP_LINE_SENSOR_H

#include "stm32f10x.h"

#define LINE_SENSOR_COUNT       2u
#define LINE_SENSOR_BLACK_LEVEL 0u

typedef struct
{
	uint8_t raw[LINE_SENSOR_COUNT];
	uint8_t black[LINE_SENSOR_COUNT];
	uint8_t black_count;
	uint8_t black_mask;
	int8_t position;
	_Bool line_detected;
} Line_Sensor_Data_t;

void Ip_Line_Sensor_Init(void);
void Ip_Line_Sensor_Read(Line_Sensor_Data_t *data);

#endif /* IP_LINE_SENSOR_H */
