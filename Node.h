/**
 * Data type representing node in huffmann tree
 *
 * @author Vladan Kudlac, xkudla15
 * @date created on 25.04.2021
 */

#ifndef KKO_NODE_H_
#define KKO_NODE_H_

#include <sys/types.h> // uint
#include <cstdint> // uint8_t

typedef struct Node {
	uint index;
	uint freq;
	uint8_t value;
	Node* parent;
	bool isLeft;
} Node;

#endif
