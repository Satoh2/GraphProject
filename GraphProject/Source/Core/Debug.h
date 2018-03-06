#pragma once

#include <iostream>
#include "AGraph.h"
#include "Graphs\CrossedCube.h"

namespace DEBUG
{
	int MinDim = 2;
	int MaxDim = 16;

	void AGraph_GenerateFaults(AGraph &g)
	{
		int MinDim = 2;
		int MaxDim = 13;

		for (int dim = MinDim; dim <= MaxDim; dim++)
		{
			g.SetDimension(dim);

			for (double f = 0; f < 0.8; f += 0.01)
			{
				std::cout
					<< "\rn = " << dim
					<< ", faultRatio = " << f;

				g.GenerateFaults(f);
				int count = 0;
				for (int i = 0; i < g.GetNodeNum(); i++)
				{
					if (g.IsFault(i)) count++;
				}
				if (count != (int)(g.GetNodeNum() * f))
				{
					std::cout << "...NG";
					return;
				}
			}
			std::cout << "...OK" << std::endl;
		}
	}

	void AGraph_CalcDistance(AGraph &g)
	{
		int MinDim = 2;
		int MaxDim = 13;

		for (int dim = MinDim; dim <= MaxDim; dim++)
		{
			g.SetDimension(dim);
			for (int i = 0; i < g.GetNodeNum(); i++)
			{
				printf_s("\rn = %2d ... %3.0f%%", dim, (double)i / g.GetNodeNum() * 100);
				for (int j = 0; j < g.GetNodeNum(); j++)
				{
					int d1 = g.CalcDistanceBFS(i, j);
					int d2 = g.CalcDistance(i, j);
					if (d1 != d2)
					{
						printf_s("\nNG in d(%d, %d) = %d / %d\n", i, j, d1, d2);
						return;
					}
				}
			}
			printf_s("\rn = %2d ... 100%%\n", dim);
		}
	}

	void CrossedCube_CalcDistanceClassification()
	{
		int dim = 10;
		CrossedCube g;

		g.SetDimension(10);

		for (int node1 = 0; node1 < g.GetNodeNum(); node1++)
		{
			printf_s("\rn = %2d ... %3.0f%%", dim, (double)node1 / g.GetNodeNum() * 100);
			for (int node2 = 0; node2 < g.GetNodeNum(); node2++)
			{
				int d1[MAX_DIMENSION], d2[MAX_DIMENSION];
				g.CalcDistanceClassification(node1, node2, d1);

				// 模範解答の計算
				int distance = g.CalcDistance(node1, node2);
				for (int i = 0; i < g.GetDegree(node1); i++)
				{
					int neighbor = g.GetNeighbor(node1, i);
					d2[i] = g.CalcDistance(neighbor, node2) - distance;
				}

				// 正誤チェック
				bool f = false;
				for (int i = 0; i < g.GetDegree(node1); i++)
				{
					if (d1[i] != d2[i])
					{
						f = true;
						break;
					}
				}

				// 違ってたら表示して修了
				if (f)
				{
					printf_s("(%d, %d)\n", node1, node2);
					for (int i = g.GetDegree(node1) - 1; i >= 0; i--)
					{
						printf_s("%2d ", d1[i]);
					}
					printf_s("\n");
					for (int i = g.GetDegree(node1) - 1; i >= 0; i--)
					{
						printf_s("%2d ", d2[i]);
					}
					printf_s("\n");

				}
			}
		}
		std::cout << "OK" << std::endl;
	}
}