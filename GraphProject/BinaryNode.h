#pragma once

#include <string>
#include "ANode.h"

class BinaryNode : ANode
{
public:
	BinaryNode() : ANode() {}
	BinaryNode(int id) : ANode(id) {}
	BinaryNode(const ANode &obj) : ANode(obj) {}

	// Getter for ID
	inline int GetID() { return ID; }

	// Setter for ID
	inline void SetID(int id) { ID = id; }

	// Returns address in binary string
	std::string ToString(int length, int interval);

	// Returns index-th bit
	int GetBit(int index);
};