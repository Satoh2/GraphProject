#pragma once

#include "..\Core\AGraph.h"

class Hypercube : public AGraph
{
public:
	Hypercube();
	Hypercube(const Hypercube &obj);

	// Returns degree of node
	int GetDegree(int node) const;

	// Returns number of nodes
	int GetNodeNum() const;

	// Returns i-th neighbor of node
	int GetNeighbor(int node, int i) const;

	// Calclations distance between node1 and node2
	int CalcDistance(int node1, int node2) const;
};