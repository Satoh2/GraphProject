#include <iostream>

#include "BinaryNode.h"
#include "Hypercube.h"

int main() {
	BinaryNode bin;
	Hypercube g;
	g.SetDimension(4);
	
	for (BinaryNode u(0); u.GetID() < g.GetNodeNum(); u.SetID(u.GetID() + 1))
	{
		for (BinaryNode v(u.GetID() + 1); v.GetID() < g.GetNodeNum(); v.SetID(v.GetID() + 1))
		{
			int d1 = g.CalcDistanceBFS(u, v);
			int d2 = g.CalcDistance(u, v);
			printf_s("d(%d, %d) = %d / %d ... %s\n",
				u.GetID(), v.GetID(), d1, d2, d1 == d2 ? "ok" : "ng");
		}
	}
}