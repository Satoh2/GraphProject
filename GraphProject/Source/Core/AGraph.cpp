#include "AGraph.h"

AGraph::AGraph()
{
	Dimension = 0;
	mt.seed(0);
	FaultArray = nullptr;
}

AGraph::AGraph(const AGraph &obj)
{
	Dimension = obj.Dimension;
	mt.seed(0);
	FaultArray = nullptr;
}

int AGraph::GetDimension() const
{
	return Dimension;
}

AGraph::~AGraph()
{
	delete[] FaultArray;
}

void AGraph::SetDimension(int dim)
{
	Dimension = dim;
	if (FaultArray != nullptr)
	{
		delete[] FaultArray;
	}
	FaultArray = new int[GetNodeNum()];
	GenerateFaults(0.0);
}

void AGraph::SetRandSeed(int seed)
{
	mt.seed(seed);
}

int AGraph::CalcDistanceBFS(int node1, int node2) const
{
	if (node1 == node2) return 0;

	std::queue<int> q;
	int *distance = new int[GetNodeNum()];
	for (int i = 0; i < GetNodeNum(); i++)
	{
		distance[i] = 1000;
	}

	distance[node1] = 0;
	q.push(node1);

	while (!q.empty())
	{
		int current = q.front();
		q.pop();

		for (int i = 0; i < GetDegree(current); i++)
		{
			int neighbor = GetNeighbor(current, i);
			if (neighbor == node2)
			{
				int d = distance[current] + 1;
				delete[] distance;
				return d;
			}
			if (distance[current] < distance[neighbor] - 1)
			{
				q.push(neighbor);
				distance[neighbor] = distance[current] + 1;
			}
		}
	}

	delete[] distance;
	throw "node1 and node2 are not connected.";
}

int AGraph::CalcDistance(int node1, int node2)
{
	return CalcDistanceBFS(node1, node2);
}

bool AGraph::IsFault(int node) const
{
	return FaultArray[node];
}

void AGraph::GenerateFaults(double faultRatio)
{
	int nodeNum = GetNodeNum();
	int faultNodeNum = static_cast<int>(nodeNum * faultRatio);
	std::uniform_int_distribution<> rand(0, nodeNum - 1);

	for (int i = 0; i < nodeNum; i++)
	{
		FaultArray[i] = false;
	}

	int count = 0;
	while (count < faultNodeNum)
	{
		int id = rand(mt);
		if (!FaultArray[id])
		{
			FaultArray[id] = true;
			count++;
		}
	}
}

bool AGraph::IsConnected(int node1, int node2) const
{
	int nodeNum = GetNodeNum();
	bool *visited = new bool[nodeNum];
	for (int i = 0; i < nodeNum; i++)
	{
		visited[i] = false;
	}

	std::stack<int> s;
	s.push(node1);
	visited[node1] = true;

	while (!s.empty())
	{
		int current = s.top();
		s.pop();

		for (int i = 0; i < GetDegree(current); i++)
		{
			int neighbor = GetNeighbor(current, i);
			if (!visited[neighbor] && !IsFault(neighbor))
			{
				if (neighbor == node2)
				{
					delete[] visited;
					return true;
				}
				s.push(neighbor);
				visited[neighbor] = true;
			}
		}
	}

	delete[] visited;
	return false;
}

void AGraph::GetNodePair(int &node1, int &node2)
{
	std::uniform_int_distribution<> form(0, GetNodeNum() - 1);

	do
	{
		while (IsFault(node1 = form(mt)));
		while (IsFault(node2 = form(mt)) && node1 != node2);
	} while (!IsConnected(node1, node2));
}


int AGraph::Routing(int node1, int node2, std::function<int(int, int)> getNext, int path[], int maxSteps) const
{
	int steps = 0;
	int current = node1;
	path[steps] = node1;
	while (current != node2)
	{
		if (++steps > maxSteps)
		{
			return -1;
		}

		current = getNext(current, node2);
		path[steps] = current;
	}
	return steps;
}