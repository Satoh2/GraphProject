#pragma once

#include <iostream>
#include "AGraph.h"

namespace DEBUG
{
	int MinDim = 2;
	int MaxDim = 16;

	template <typename NodeType>
	void AGraph_GenerateFaults(AGraph<NodeType> *g)
	{
		g->SetDimension(maxDim);
		int *faultArray = new int[g->GetNodeNum()];

		for (int dim = MinDim; dim <= MaxDim; dim++)
		{
			g->SetDimension(dim);

			for (double f = 0; f < 0.8; f += 0.01)
			{
				std::cout
					<< "\rn = " << dim
					<< ", faultRatio = " << f;

				g->GenerateFaults(f, faultArray);
				int count = 0;
				for (int i = 0; i < g->GetNodeNum(); i++)
				{
					count += faultArray[i];
				}
				if (count != (int)(g->GetNodeNum() * f))
				{
					std::cout << "\nNG";
					return;
				}
			}
			std::cout << "...OK" << std::endl;
		}
		delete[] faultArray;
	}

	template <typename NodeType>
	void AGraph_CalcDistance(AGraph<NodeType> *g)
	{
		for (int dim = MinDim; dim <= MaxDim; dim++)
		{
			g->SetDimension(dim);
			std::cout << "n = " << dim;

			for (int u = 0; u < g->GetNodeNum(); u++)
			{
				for (int v = u + 1; v < g->GetNodeNum(); v++)
				{
					BinaryNode uu(u), vv(v);
					int d1 = g->CalcDistanceBFS(uu, vv);
					int d2 = g->CalcDistance(uu, vv);

					if (d1 != d2)
					{
						printf_s("...NG in d(%d, %d) = %d / %d\n", u, v, d1, d2);
						return;
					}
				}
			}

			std::cout << "...OK\n";
		}
	}
}