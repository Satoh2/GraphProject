#include "CrossedCube.h"
#include "..\Tools\Binary.h"

CrossedCube::CrossedCube() : AGraph() {}
CrossedCube::CrossedCube(const CrossedCube &obj) : AGraph(obj) {}

bool CrossedCube::DPPR(int node1, int node2, int index, int sum) const
{
	int u0 = (node1 >> index) & 1, u1 = (node1 >> (index + 1)) & 1;
	int v0 = (node2 >> index) & 1, v1 = (node2 >> (index + 1)) & 1;

	return (u0 == 1) && (v0 == 1) && ((u1 == v1) ^ (sum & 1)) // (1) or (2)
		|| (u0 == 0) && (v0 == 0) && (u1 == v1);	// (3)

	// The above expression equals to the following.
	//return (u0 == 1) && (v0 == 1) && (u1 == v1) && ((sum & 1) == 0) // (1)
	//|| (u0 == 1) && (v0 == 1) && (u1 != v1) && ((sum & 1) == 1) // (2)
	//|| (u0 == 0) && (v0 == 0) && (u1 == v1);	// (3)
}

int CrossedCube::GetDegree(int node) const 
{ 
	return Dimension;
}

int CrossedCube::GetNodeNum() const 
{ 
	return 1 << Dimension; 
};

int CrossedCube::GetNeighbor(int node, int i) const
{
	return node ^ (((node & 0x55555555 & ((1 << i) - 1)) << 1) | (1 << i));
}

int CrossedCube::CalcDistance(int node1, int node2) const
{
	int sum = 0;
	Binary u(node1), v(node2);
	for (int i = Dimension - (Dimension & 1); i >= 0; i -= 2)
	{
		if (sum == 0)
		{
			sum = (u[i] ^ v[i]) + (u[i + 1] ^ v[i + 1]);
		}
		else if (!DPPR(node1, node2, i, sum))
		{
			sum++;
		}
	}
	return sum;
}

void CrossedCube::CalcDistanceClassification(int node1, int node2, int(&distanceArray)[MAX_DIMENSION]) const
{
	// 同一頂点ならすべて後方
	if (node1 == node2)
	{
		for (int i = 0; i < Dimension; i++) distanceArray[i] = 1;
		return;
	}

	Binary u(node1), v(node2);
	
	// ρとh^を計算
	int ph;			// ph = floor(max{h | u_h != v_h} / 2)
	int rho[32];	// rho[i] = ρ(u, v, i)
	int rhoSum[32];	// rhoSum[i] = Σ_{0 to i + 1}ρ(u, v, i)
	int sum = 0;
	for (int i = Dimension - (Dimension & 1); i >= 0; i -= 2)
	{
		int pi = i >> 1;
		if (sum == 0)
		{
			rho[pi] = (u[i] ^ v[i]) + (u[i + 1] ^ v[i + 1]);
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
			if ((k & 1) == 0 && u[k] == 1 && v[k] == 1)
			{
				f1 = 
					rho[pk] == 0 
					? 1 
					: -1;
			}
			// Update f2
			if ((k & 1) == 0 && rho[pk] > 0)
			{
				f2 = (v[k] ^ v[k + 1]) == u[k + 1]
						? f1
						: -1;
			}
		}
		else if (pk == ph)
		{
			distanceArray[k] =
				rho[pk] == 2 ? -1
				: u[k] == v[k] ? 1	// rho[pk] != 2 ならば rho[pk] == 1 は自明
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

int CrossedCube::GetNext_MasterThesis(int node1, int node2)
{
	int d[MAX_DIMENSION];
	CalcDistanceClassification(node1, node2, d);

	// 非故障な前方、横、後方をそれぞれ数える
	int count[] = { 0, 0, 0 };
	int ary[3][MAX_DIMENSION];
	for (int i = 0; i < Dimension; i++)
	{
		if (!IsFault(GetNeighbor(node1, i)))
		{
			ary[d[i] + 1][count[d[i] + 1]++] = i;
		}
	}

	// 前方、横、後方のどれにいくか
	int dist = count[0] > 0		? 0
				: count[1] > 0	? 1
								: 2;

	// 何番目に行くか;
	return GetNeighbor(
		node1,
		ary[dist][std::uniform_int_distribution<>(0, count[dist] - 1)(mt)]);
}

int CrossedCube::GetNext_Efe(int node1, int node2)
{
	Binary u(node1), v(node2);
	// Leftmost Different Bit Index
	int l = Dimension - 1;
	while (u[l] == v[l]) l--;

	int next[2];
	bool flag = true;

	if ((l & 1) == 1 && u[l - 1] != v[l - 1])
	{
		next[0] = GetNeighbor(node1, l);
		next[1] = GetNeighbor(node1, l - 1);
	}
	else
	{
		for (int k = l - 1 - (l & 1); k > 0; k -= 2)
		{
			// 上がずれる
			if ((u[k] ^ u[k - 1]) != v[k])
			{
				// 下もずれる
				if (u[k - 1] != v[k - 1])
				{
					// {0}のパターン
					next[0] = GetNeighbor(node1, k - 1);
					next[1] = -1;
				}
				else
				{
					// {1, 2}パターン
					next[0] = GetNeighbor(node1, k);
					next[1] = GetNeighbor(node1, l);
				}
				flag = false;
				break;
			}
			else
			{
				// 下だけずれる
				if (u[k - 1] != v[k - 1])
				{
					// {2}のパターン
					next[0] = GetNeighbor(node1, l);
					next[1] = -1;
					flag = false;
					break;
				}
			}
		}
		if (flag)
		{
			next[0] = GetNeighbor(node1, l);
			next[1] = -1;
		}
	}
	if (!IsFault(next[0]))
	{
		return next[0];
	}
	else if (next[1] >= 0 && !IsFault(next[1]))
	{
		return next[1];
	}
	else
	{
		int unfault[MAX_DIMENSION];
		int count = 0;
		for (int i = 0; i < Dimension; i++)
		{
			int neighbor = GetNeighbor(node1, i);
			if (!IsFault(neighbor))
			{
				unfault[count++] = neighbor;
			}
		}

		return unfault[std::uniform_int_distribution<>(0, count - 1)(mt)];
	}
}