#pragma once

#include "AGraph.h"
#include "BinaryNode.h"

class Hypercube : public AGraph<BinaryNode>
{
public:
	// Returns degree of node
	int GetDegree(BinaryNode node) { return Dimension; }

	// Returns number of nodes
	int GetNodeNum() { return 1 << Dimension; };

	// Returns i-th neighbor of node
	BinaryNode GetNeighbor(BinaryNode node, int i);

	// Calclations distance between node1 and node2
	int CalcDistance(BinaryNode node1, BinaryNode node2);
};