#include "Ip_Line_Sensor.h"

#define LINE_SENSOR_PINS (GPIO_Pin_1 | GPIO_Pin_2)

static const uint16_t s_sensor_pins[LINE_SENSOR_COUNT] = {
	GPIO_Pin_1, /* PA1 -> trai */
	GPIO_Pin_2  /* PA2 -> phai */
};

/*
 * Trong so co ban:  PA1 = -2,  PA2 = +2
 * Tang do phan giai len 5 muc: -2, -1, 0, +1, +2
 * bang cach ket hop trang thai 2 sensor voi vi tri truoc do.
 *
 * black_mask  | Y nghia                | position
 * ------------|------------------------|----------
 *  0x01       | Chi PA1 thay line       | -2 - lech ben trai nhieu hon
 *  0x03       | Ca 2 thay line         |  0 (hoac -1/+1 tuy huong) - tuy vao vi tri luc truoc
 *  0x02       | Chi PA2 thay line      | +2 - lech ben phai nhieu hon
 *  0x00       | Khong thay line        | giu nguyen vi tri cu
 */

static int8_t s_last_position = 0;

static int8_t Decode_Position_2ch(uint8_t black_mask, int8_t prev_pos)
{
	switch (black_mask)
	{
	case 0x01u: /* Chi PA1 (trai) */
		return -2;

	case 0x02u: /* Chi PA2 (phai) */
		return 2;

	case 0x03u: /* Ca 2 sensor thay line -> line o giua */
		if (prev_pos < 0) /* truoc do line lech trai */
		{
			return -1; /* line dang chuyen tu trai sang giua */
		}
		else if (prev_pos > 0) /* truoc do line lech phai */
		{
			return 1; /* line dang chuyen tu phai sang giua */
		}
		else // prev_pos == 0
		{
			return 0; /* line da on dinh o giua */
		}

	default: /* 0x00 - khong thay line, se xu ly o ngoai */
		return prev_pos;
	}
}

void Ip_Line_Sensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = LINE_SENSOR_PINS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	s_last_position = 0;
}

void Ip_Line_Sensor_Read(Line_Sensor_Data_t *data)
{
	uint8_t i;
	uint8_t black_count = 0;

	data->black_mask = 0;

	// 
	for (i = 0; i < LINE_SENSOR_COUNT; ++i)
	{
		uint8_t raw = (uint8_t)GPIO_ReadInputDataBit(GPIOA, s_sensor_pins[i]);
		uint8_t is_black = (raw == LINE_SENSOR_BLACK_LEVEL) ? 1u : 0u;

		data->raw[i] = raw;
		data->black[i] = is_black;

		if (is_black != 0u)
		{
			black_count++; // dem so cam bien thay vach
			data->black_mask |= (uint8_t)(1u << i); // luu vao back_mask
		}
	}

	data->black_count = black_count;

	if (black_count > 0u) // thay co line
	{
		data->position = Decode_Position_2ch(data->black_mask, s_last_position); // giai ma do line 2 channel
		data->line_detected = 1;
		s_last_position = data->position; // cap nhat vi tri cuoi
	}
	else // neu khong thay line: giu nguyen vi tri cu
	{
		data->position = s_last_position;
		data->line_detected = 0;
	}
}
