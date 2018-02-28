#include <iostream>

#include "Core\BinaryNode.h"
#include "Core\DEBUG.h"
#include "Graphs\Hypercube.h"
#include "Graphs\CrossedCube.h"

class A
{
public:
	virtual void func() = 0;
};

class B : public A
{
public:
	void func() { std::cout << "A!!" << std::endl; }
};

int main() {
	Hypercube hq(10, 0);
	CrossedCube cq(10, 0);

	CrossedCube g(10, 0);

	A &&a = B();
	a.func();

	getchar();

	int d1[32], d2[32];

	for (BinaryNode u(0); u.GetID() < g.GetNodeNum(); u.SetID(u.GetID() + 1))
	{
		printf_s("%d\n", u.GetID());
		for (BinaryNode v(u.GetID() + 1); v.GetID() < g.GetNodeNum(); v.SetID(v.GetID() + 1))
		{
			// d1‚ÌŒvŽZ
			int distance = g.CalcDistance(u, v);
			for (int i = 0; i < g.GetDegree(u); i++)
			{
				d1[i] = g.CalcDistance(g.GetNeighbor(u, i), v) - distance;
			}

			// d2‚ÌŒvŽZ
			g.CalcDistanceClassification(u, v, d2);

			// ”äŠr
			bool f = false;
			for (int i = 0; i < g.GetDegree(u); i++)
			{
				if (d1[i] != d2[i])
				{
					f = true;
					break;
				}
			}
			if (f)
			{
				printf_s("(%d, %d)\n", u.GetID(), v.GetID());
				for (int i = g.GetDegree(u) - 1; i >= 0; i--)
				{
					printf_s("%2d,", d1[i]);
				}
				printf_s("\n");
				for (int i = g.GetDegree(u) - 1; i >= 0; i--)
				{
					printf_s("%2d,", d2[i]);
				}
				printf_s("\n");
				getchar();
			}
		}
	}
}

