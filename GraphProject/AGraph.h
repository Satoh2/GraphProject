#pragma once

#include <queue>
#include "ANode.h"

template <typename NodeType>
class AGraph
{
protected:
	int Dimension;

public:
	/** Basic functions **/

	// Returns degree of node
	virtual int GetDegree(NodeType node) = 0;

	// Returns nodenum
	virtual int GetNodeNum() = 0;

	// Getter for Dimension
	int GetDimension() { return Dimension; }

	// Setter for Dimension
	void SetDimension(int dim) { Dimension = dim; }

	// Returns i-th neighbor of node
	virtual NodeType GetNeighbor(NodeType node, int i) = 0;


	/** Functions for experiment **/
	
	// Calclations distance between node1 and node2 by breadth-first serch
	int CalcDistanceBFS(NodeType node1, NodeType node2)
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

	// Calclations distance between node1 and node2
	virtual int CalcDistance(NodeType node1, NodeType node2)
	{
		return CalcDistanceBFS(node1, node2);
	}
};