#pragma once

#include "..\Core\AGraph.h"
#include "..\Core\BinaryNode.h"

class CrossedCube : public AGraph<BinaryNode>
{
private:
	// Distance-preserving pair related
	//  * (u1, u0) ~ (v1, v0)
	bool DPPR(int u0, int u1, int v0, int v1, int sum) const;

	// Distance-preserving pair related
	//  * (node1_{index+1}, node1_{index}) ~ (node2_{index+1}, node2_{index})
	//  * ƒ°_{0 to index + 1}ƒÏ(node1, node2, index)
	bool DPPR(BinaryNode node1, BinaryNode node2, int index, int sum) const;

public:
	CrossedCube() : AGraph() {}
	CrossedCube(int dim, int seed) : AGraph(dim, seed) {}
	CrossedCube(const CrossedCube &obj) : AGraph(obj) {}

	// Returns degree of node
	int GetDegree(BinaryNode node) const { return Dimension; }

	// Returns number of nodes
	int GetNodeNum() const { return 1 << Dimension; };

	// Returns i-th neighbor of node
	BinaryNode GetNeighbor(BinaryNode node, int i) const;

	// Calclations distance between node1 and node2
	int CalcDistance(BinaryNode node1, BinaryNode node2) const;

	// Classifies all neithbors of node1 into forward/sideward/backward to node2.
	// * distanceArray[i] is d(i-th neighbor of node1, node2) - d(node1, node2).
	void CalcDistanceClassification(BinaryNode node1, BinaryNode node2, int* distanceArray) const;
};