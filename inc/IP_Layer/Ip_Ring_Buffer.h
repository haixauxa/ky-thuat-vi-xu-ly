#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint8_t *buffer;
	uint8_t size;
	uint8_t head;
	uint8_t tail;
} __attribute__((packed)) Ring_Buffer_t;


void Ip_Ring_Buffer_Put(Ring_Buffer_t *rb, uint8_t data);
uint8_t Ip_Ring_Buffer_Get(Ring_Buffer_t *rb);
uint8_t Ip_Ring_Buffer_Peek(const Ring_Buffer_t *rb);
bool Ip_Ring_Buffer_Empty(const Ring_Buffer_t *rb);
bool Ip_Ring_Buffer_Full(const Ring_Buffer_t *rb);


#endif /* RING_BUFFER_H */
