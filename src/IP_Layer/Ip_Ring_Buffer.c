#include "Ip_Ring_Buffer.h"

void Ip_Ring_Buffer_Put(Ring_Buffer_t *rb, uint8_t data)
{
	rb->buffer[rb->head] = data;
	rb->head++;
	
	if (rb->head == rb->size)
	{
		rb->head = 0;
	}
}

uint8_t Ip_Ring_Buffer_Get(Ring_Buffer_t *rb)
{
	const uint8_t data = rb->buffer[rb->tail];
	rb->tail++;
	if (rb->tail == rb->size)
	{
		rb->tail = 0;
	}
	return data;
}

uint8_t Ip_Ring_Buffer_Peek(const Ring_Buffer_t *rb)
{
	return rb->buffer[rb->tail];
}

bool Ip_Ring_Buffer_Empty(const Ring_Buffer_t *rb)
{
	return rb->head == rb->tail;
}

bool Ip_Ring_Buffer_Full(const Ring_Buffer_t *rb)
{
	uint8_t idx_after_head = rb->head + 1;
	if (idx_after_head == rb->size)
	{
		idx_after_head = 0;
	}
	return idx_after_head == rb->tail;
}

