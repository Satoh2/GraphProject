#include "CrossedCube.h"

bool CrossedCube::DPPR(BinaryNode node1, BinaryNode node2, int index, int sum) const
{
	int u0 = node1.GetBit(index), u1 = node1.GetBit(index + 1);
	int v0 = node2.GetBit(index), v1 = node2.GetBit(index + 1);

	return (u0 == 1) && (v0 == 1) && ((u1 == v1) ^ (sum & 1)) // (1) or (2)
		|| (u0 == 0) && (v0 == 0) && (u1 == v1);	// (3)

	// The above expression equals to the following.
	//return (u0 == 1) && (v0 == 1) && (u1 == v1) && ((sum & 1) == 0) // (1)
	//|| (u0 == 1) && (v0 == 1) && (u1 != v1) && ((sum & 1) == 1) // (2)
	//|| (u0 == 0) && (v0 == 0) && (u1 == v1);	// (3)
}

BinaryNode CrossedCube::GetNeighbor(BinaryNode node, int i) const
{
	int mask = ((node.GetID() & 0x55555555 & ((1 << i) - 1)) << 1) | (1 << i);
	return BinaryNode(node.GetID() ^ mask);
}

int CrossedCube::CalcDistance(BinaryNode node1, BinaryNode node2) const
{
	int sum = 0;
	for (int i = Dimension - (Dimension & 1); i >= 0; i -= 2)
	{
		if (sum == 0)
		{
			sum = (node1.GetBit(i + 1) ^ node2.GetBit(i + 1)) + (node1.GetBit(i) ^ node2.GetBit(i));
		}
		else if (!DPPR(node1, node2, i, sum))
		{
			sum++;
		}
	}
	return sum;
}

void CrossedCube::CalcDistanceClassification(BinaryNode node1, BinaryNode node2, int* distanceArray) const
{
	// “¯ˆê’¸“_‚È‚ç‚·‚×‚ÄŒã•û
	if (node1.GetID() == node2.GetID())
	{
		for (int i = 0; i < Dimension; i++) distanceArray[i] = 1;
	}
	
	// ƒÏ‚Æh^‚ðŒvŽZ
	int ph;	// ph = floor(max{h | u_h != v_h} / 2)
	int rho[32];	// rho[i] = ƒÏ(u, v, i)
	int rhoSum[32];	// rhoSum[i] = ƒ°_{0 to i + 1}ƒÏ(u, v, i)
	int sum = 0;
	for (int i = Dimension - (Dimension & 1); i >= 0; i -= 2)
	{
		int pi = i >> 1;
		if (sum == 0)
		{
			rho[pi] = (node1.GetBit(i + 1) ^ node2.GetBit(i + 1)) + (node1.GetBit(i) ^ node2.GetBit(i));
			sum = rho[pi];
			if (sum > 0) ph = i >> 1;
		}
		else if (!DPPR(node1, node2, i, sum))
		{
			rho[i >> 1] = 1;
			sum++;
		}
		else
		{
			rho[i >> 1] = 0;
		}
		if (pi > 0) rhoSum[pi - 1] = sum;
	}

	// flag
	int f1 = 0, f2 = -1;
	
	for (int k = 0; k < Dimension; k++)
	{
		int pk = k >> 1;
		if (pk < ph)
		{
			int kk = k - (k & 1);
			distanceArray[k] =
				rho[pk] == 0											? 1
				: DPPR(GetNeighbor(node1, k), node2, kk, rhoSum[pk])	? -1
																		: f1;
			// Update f1
			if ((k & 1) == 0 && node1.GetBit(k) == 1 && node2.GetBit(k) == 1)
			{
				f1 = 
					rho[pk] == 0 
					? 1 
					: -1;
			}
			// Update f2
			if ((k & 1) == 0 && rho[pk] > 0)
			{
				f2 = (node2.GetBit(k + 1) ^ node2.GetBit(k)) == node1.GetBit(k + 1)
						? f1
						: -1;
			}
		}
		else if (pk == ph)
		{
			distanceArray[k] =
				rho[pk] == 2 ? -1
				: node1.GetBit(k) == node2.GetBit(k) ? 1	// rho[pk] != 2 ‚È‚ç‚Î rho[pk] == 1 ‚ÍŽ©–¾
				: f2;

		}
		else
		{
			distanceArray[k] =
				rho[ph] == 1 
				? 1
				: f1;
		}
	}
}