#pragma once

#include <queue>
#include <stack>
#include <random>

#include "..\ANode.h"

template <typename NodeType>
class AGraph
{
protected:
	int Dimension;

	// Random number generator
	std::mt19937 mt;

public:
	//
	// Constructors
	//

	AGraph();
	AGraph(int dim, int seed);
	AGraph(const AGraph &obj);


	//
	// Basic functions (graph definitions)
	//

	// Returns degree of node
	virtual int GetDegree(NodeType node) = 0;

	// Returns nodenum
	virtual int GetNodeNum() = 0;

	// Getter for Dimension
	int GetDimension();

	// Setter for Dimension
	void SetDimension(int dim);

	// Returns i-th neighbor of node
	virtual NodeType GetNeighbor(NodeType node, int i) = 0;


	//
	// Experiment functions
	//

	// Set new random seed
	void SetRandSeed(int seed);

	// Calclations distance between node1 and node2 by breadth-first serch
	int CalcDistanceBFS(NodeType node1, NodeType node2);

	// Calclations distance between node1 and node2
	virtual int CalcDistance(NodeType node1, NodeType node2);

	// Initialize faultArray that node is fault if faultArray[node.GetID()] = true.
	// * faultRatio must be in from 0 to 1.
	// * Length of faultArray must be learger than number of nodes.
	void GenerateFaults(double faultRatio, int *faultArray);

	// Returns if node1 and node2 are connected with non-fault pathes
	bool IsConnected(NodeType node1, NodeType node2, int *faultArray);

	// Returns node1 and node2 that are connected with non-fault pathes
	void GetNodePair(NodeType *node1, NodeType *node2, int *faultArray);
};


// Implementation

template <typename NodeType>
AGraph<NodeType>::AGraph()
{
	mt.seed(0);
	Dimension = 1;
}

template <typename NodeType>
AGraph<NodeType>::AGraph(int dim, int seed)
{
	Dimension = dim;
	mt.seed(seed);
}

template <typename NodeType>
AGraph<NodeType>::AGraph(const AGraph &obj)
{
	Dimension = obj.Dimension;
	mt.seed(0);
}

template <typename NodeType>
int AGraph<NodeType>::GetDimension() { return Dimension; }

template <typename NodeType>
void AGraph<NodeType>::SetDimension(int dim) { Dimension = dim; }

template <typename NodeType>
void AGraph<NodeType>::SetRandSeed(int seed) { mt.seed(seed); }

template <typename NodeType>
int AGraph<NodeType>::CalcDistanceBFS(NodeType node1, NodeType node2)
{
	std::queue<NodeType> q;
	int *distance = new int[GetNodeNum()];
	for (int i = 0; i < GetNodeNum(); i++)
	{
		distance[i] = 1000;
	}

	distance[node1.GetID()] = 0;
	q.push(node1);

	while (!q.empty())
	{
		NodeType current = q.front();
		q.pop();

		for (int i = 0; i < GetDegree(current); i++)
		{
			NodeType neighbor = GetNeighbor(current, i);
			if (neighbor.GetID() == node2.GetID())
			{
				int d = distance[current.GetID()] + 1;
				delete[] distance;
				return d;
			}
			if (distance[current.GetID()] < distance[neighbor.GetID()] - 1)
			{
				q.push(neighbor);
				distance[neighbor.GetID()] = distance[current.GetID()] + 1;
			}
		}
	}

	delete[] distance;
	throw "Graph definition is wrong.";
}

template <typename NodeType>
int AGraph<NodeType>::CalcDistance(NodeType node1, NodeType node2)
{
	return CalcDistanceBFS(node1, node2);
}

template <typename NodeType>
void AGraph<NodeType>::GenerateFaults(double faultRatio, int *faultArray)
{
	int nodeNum = GetNodeNum();
	int faultNodeNum = nodeNum * faultRatio;
	std::uniform_int_distribution<> rand(0, nodeNum - 1);

	for (int i = 0; i < nodeNum; i++)
	{
		faultArray[i] = 0;
	}

	int count = 0;
	while (count < faultNodeNum)
	{
		int id = rand(mt);
		if (!faultArray[id])
		{
			faultArray[id] = 1;
			count++;
		}
	}
}

template <typename NodeType>
bool AGraph<NodeType>::IsConnected(NodeType node1, NodeType node2, int *faultArray)
{
	int nodeNum = GetNodeNum();
	bool *visited = new bool[nodeNum];
	for (int i = 0; i < nodeNum; i++)
	{
		visited[i] = false;
	}

	std::stack<NodeType> s;
	visited[node1.GetID()] = true;

	while (!s.empty())
	{
		NodeType current = s.top();
		s.pop();

		for (int i = 0; i < GetDegree(current); i++)
		{
			NodeType neighbor = GetNeighbor(current, i);
			if (!visited[neighbor.GetID()] && !faultArray[neighbor.GetID()])
			{
				if (neighbor.GetID() == node2.GetID())
				{
					delete[] visited;
					return true;
				}
				s.push(neighbor);
				visited[neighbor.GetID()] = true;
			}
		}
	}

	delete[] visited;
	return false;
}

template <typename NodeType>
void AGraph<NodeType>::GetNodePair(NodeType *node1, NodeType *node2, int *faultArray)
{
	int nodeNum = GetNodeNum();
	std::uniform_int_distribution<> rand(0, nodeNum - 1);

	do
	{
		int tmp1, tmp2;

		while (faultArray[tmp1 = rand(mt)]);
		node1->SetID(tmp1);

		while (faultArray[tmp2 = rand(mt)] && tmp2 != tmp1);
		node2->SetID(tmp2);

	} while (IsConnected(*node1, *node2, faultArray));
}
