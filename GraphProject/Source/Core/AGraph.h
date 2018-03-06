#pragma once

#include <queue>
#include <stack>
#include <random>
#include <functional>


#define MAX_DIMENSION 32

class AGraph
{
protected:
	int Dimension;
	int *FaultArray;

	// Random number generator
	std::mt19937 mt;

public:
	//
	// Constructors
	//

	AGraph();
	AGraph(const AGraph &obj);
	virtual ~AGraph();


	//
	// Basic functions (graph definitions)
	//

	// Returns degree of node
	virtual int GetDegree(int node) const = 0;

	// Returns nodenum
	virtual int GetNodeNum() const = 0;

	// Getter for Dimension
	virtual int GetDimension() const;

	// Setter for Dimension
	virtual void SetDimension(int dim);

	// Returns i-th neighbor of node
	virtual int GetNeighbor(int node, int i) const = 0;


	//
	// Experiment functions
	//

	// Set new random seed
	void SetRandSeed(int seed);

	// Calclations distance between node1 and node2 by breadth-first serch
	int CalcDistanceBFS(int node1, int node2) const;

	// Calclations distance between node1 and node2
	virtual int CalcDistance(int node1, int node2);

	// Returns if node is fault
	bool IsFault(int node) const;

	// Initialize faultArray that node is fault if faultArray[node.GetID()] = true.
	// * faultRatio must be in from 0 to 1.
	// * Length of faultArray must be learger than number of nodes.
	void GenerateFaults(double faultRatio);

	// Returns if node1 and node2 are connected with non-fault pathes
	bool IsConnected(int node1, int node2) const;

	// Returns node1 and node2 that are connected with non-fault pathes
	void GetNodePair(int &node1, int &node2);

	// Routing function.
	// * getNext(u,v) must return a neighbor of u.
	// * path[] must be longer than maxSteps + 1.
	int Routing(int node1, int node2, std::function<int(int, int)> getNext, int path[], int maxSteps) const;
};
