#pragma once

#include <string>
#include "ANode.h"
#include "Core\AGraph.h"

class BinaryNode : public ANode
{
public:
	BinaryNode() : ANode() {}
	BinaryNode(int id) : ANode(id) {}
	BinaryNode(const ANode &obj) : ANode(obj) {}

	// Getter for ID
	inline int GetID() const { return ID; }

	// Setter for ID
	inline void SetID(int id) { ID = id; }

	// Returns address in binary string
	std::string ToString(int length, int interval);

	// Returns index-th bit
	int GetBit(int index);

	// Set value to index-th bit
	// * value must be 0 or 1
	void SetBit(int value, int index);
};

