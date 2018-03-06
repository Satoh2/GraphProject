#pragma once

#include "..\Core\AGraph.h"

class CrossedCube : public AGraph
{
private:
	// Distance-preserving pair related
	//  * (node1_{index+1}, node1_{index}) ~ (node2_{index+1}, node2_{index})
	//  * ƒ°_{0 to index + 1}ƒÏ(node1, node2, index)
	bool DPPR(int node1, int node2, int index, int sum) const;

public:
	//
	// Constructors
	//
	CrossedCube();
	CrossedCube(const CrossedCube &obj);

	// Returns degree of node
	int GetDegree(int node) const;

	// Returns number of nodes
	int GetNodeNum() const;

	// Returns i-th neighbor of node
	int GetNeighbor(int node, int i) const;

	// Calclations distance between node1 and node2
	int CalcDistance(int node1, int node2) const;

	// Classifies all neithbors of node1 into forward/sideward/backward to node2.
	// * Length of distanceArray must be MAX_DIMENSION.
	// * distanceArray[i] is d(i-th neighbor of node1, node2) - d(node1, node2).
	void CalcDistanceClassification(int node1, int node2, int (&distanceArray)[MAX_DIMENSION]) const;

	int GetNext_MasterThesis(int node1, int node2);
	int GetNext_Efe(int node1, int node2);
};