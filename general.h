#ifndef GENREAL_H
#define GENREAL_H
#include "msglist.h"
/* Some definition */
#define GENCONT 4


enum msg_type {BYZANTINE = 1, ACK = 2};

/* The structure of the Byzantine message */
typedef struct {
	uint32_t type; // Must be equal to 1
	uint32_t size; // size of message in bytes
	uint32_t round; // round number
	uint32_t order; // the order (retreat = 0 and attack = 1)
	uint32_t ids[]; // id's of the senders of the message
} ByzantineMessage;

/* The sturcture of the ACK */
typedef struct {
	uint32_t type; // Must be equal to 2
	uint32_t size; // size of message in bytes
	uint32_t round; // round number
}Ack;



#endif