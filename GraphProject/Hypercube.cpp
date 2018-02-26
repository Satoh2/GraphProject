#include "Hypercube.h"

BinaryNode Hypercube::GetNeighbor(BinaryNode node, int i)
{
	BinaryNode neighbor(node.GetID() ^ (1 << i));
	return neighbor;
}

int Hypercube::CalcDistance(BinaryNode node1, BinaryNode node2)
{
	int bits = node1.GetID() ^ node2.GetID();
	bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
	bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
	bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
	bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
	return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
}