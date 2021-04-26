/**
 * Class with huffman tree state and operations
 *
 * @author Vladan Kudlac, xkudla15
 * @date created on 25.04.2021
 */

#ifndef KKO_HUFFTREE_H_
#define KKO_HUFFTREE_H_

#include "main.h"
#include "Node.h"

class HuffmanTree {

private:
	std::map<uint8_t, Node*> leaves;
	Node* nodes[NODES];
	Node* NYT;

public:
	HuffmanTree();
	~HuffmanTree();
	Node* findNode(uint8_t c);
	bool isNYT(Node* node);
	Node* getNYT();
	void updateHuffmanTree(Node* foundNode, uint8_t c);
	std::string getCode(Node* node);
};

HuffmanTree::HuffmanTree() {
	// Clear nodes array
	for (uint i = 0; i < NODES; i++) {
		nodes[i] = nullptr;
	}
	// Insert NYT into empty tree
	this->NYT  = new Node { NODES - 1, 0, 0, nullptr, true };
	nodes[NYT->index] = NYT;
}

HuffmanTree::~HuffmanTree() {
	for (Node* node : nodes) {
		if (node != nullptr) {
			delete node;
		}
	}
}

Node* HuffmanTree::findNode(uint8_t c) {
	Node* foundNode;
	try {
		foundNode = leaves.at(c);
	} catch(std::out_of_range const&) {
		foundNode = NYT;
	}
	return foundNode;
}

bool HuffmanTree::isNYT(Node* node) {
	return (node == this->NYT);
}

Node* HuffmanTree::getNYT() {
	return this->NYT;
}

void HuffmanTree::updateHuffmanTree(Node* foundNode, uint8_t c) {
	// First appearance for symbol
	if (foundNode == this->NYT) {
		// Split NYT into two leaves - NYT and the new value
		Node* leftLeaf  = new Node { foundNode->index - 2, 0, 0, foundNode, true };
		Node* rightLeaf = new Node { foundNode->index - 1, 1, c, foundNode, false };
		foundNode->freq = 1;
		this->nodes[leftLeaf->index] = leftLeaf;
		this->nodes[rightLeaf->index] = rightLeaf;
		this->leaves[c] = rightLeaf;
		// Change current NYT to the left child
		this->NYT = leftLeaf;
		// Goto parent node
		foundNode = foundNode->parent;
	}

	while (foundNode != nullptr) {
		// Node number max in block ?
		uint nextId = foundNode->index + 1;
		if (nextId < NODES && foundNode->freq >= this->nodes[nextId]->freq) {
			Node* mostRight = nullptr;
			for (uint i = foundNode->index + 1; i < NODES; i++) {
				if (foundNode->freq == this->nodes[i]->freq) {
					mostRight = this->nodes[i];
				}
			}
			// Swap with most right node with same freq
			if (mostRight != nullptr && mostRight != foundNode->parent) {
				// Swap indexes
				uint tmpIndex = foundNode->index;
				foundNode->index = mostRight->index;
				mostRight->index = tmpIndex;
				this->nodes[foundNode->index] = foundNode;
				this->nodes[mostRight->index] = mostRight;
				// Swap pointers
				Node* tmpParent = foundNode->parent;
				foundNode->parent = mostRight->parent;
				mostRight->parent = tmpParent;
				// Swap sides
				bool tmpIsLeft = foundNode->isLeft;
				foundNode->isLeft = mostRight->isLeft;
				mostRight->isLeft = tmpIsLeft;
			}
		}
		// Increment node weight
		foundNode->freq += 1;
		// Goto parent node
		foundNode = foundNode->parent;
	}
}

std::string HuffmanTree::getCode(Node* node) {
	std::string path = "";
	while (node->parent != nullptr) {
		path = (node->isLeft ? '1' : '0') + path;
		node = node->parent;
	}
	return path;
}

#endif
