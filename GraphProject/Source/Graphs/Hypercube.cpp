#include "Hypercube.h"

Hypercube::Hypercube() : AGraph() {}
Hypercube::Hypercube(const Hypercube &obj) : AGraph(obj) {}

int Hypercube::GetDegree(int node) const 
{
	return Dimension;
}

int Hypercube::GetNodeNum() const 
{ 
	return 1 << Dimension;
}

int Hypercube::GetNeighbor(int node, int i) const
{
	return node ^ (1 << i);
}

int Hypercube::CalcDistance(int node1, int node2) const
{
	int bits = node1 ^ node2;
	bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
	bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
	bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
	bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
	return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
}