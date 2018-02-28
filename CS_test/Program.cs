using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS_test
{
    class Program
    {
        static void Main(string[] args)
        {
            var g = new Hypercube();

            int minDim = 2;
            int maxDim = 16;

            for (int dim = minDim; dim < maxDim; dim++)
            {
                g.Dimension = dim;
                Console.Write($"n = {dim}");

                for (int u = 0; u < g.GetNodeNum(); u++)
                {
                    for (int v = u + 1; v < g.GetNodeNum(); v++)
                    {
                        var uu = new BinaryNode(u);
                        var vv = new BinaryNode(v);

                        int d1 = g.CalcDistanceBFS(uu, vv);
                        int d2 = g.CalcDistance(uu, vv);

                        if(d1 != d2)
                        {
                            Console.WriteLine($"...NG in d({u}, {v}) = {d1} / {d2}");
                            return;
                        }
                    }
                }

                Console.WriteLine($"...OK");
            }
        }
    }

    class Hypercube : AGraph<BinaryNode>
    {
        public override int GetDegree(BinaryNode node)
        {
            return Dimension;
        }

        public override BinaryNode GetNeighbor(BinaryNode node, int i)
        {
            return new BinaryNode(node.ID ^ (1 << i));
        }

        public override int GetNodeNum()
        {
            return 1 << Dimension;
        }

        public override int CalcDistance(BinaryNode node1, BinaryNode node2)
        {
            int bits = node1.ID ^ node2.ID;
            bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
            bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
            bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
            bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
            return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
        }
    }

    class BinaryNode : ANode
    {
        public BinaryNode() : base() { }
        public BinaryNode(int id) : base(id) { }
        public BinaryNode(BinaryNode obj) : base(obj) { }
    }

    class ANode
    {
        public int ID { get; set; }

        public ANode() { ID = 0; }
        public ANode(int id) { ID = id; }
        public ANode(ANode obj) { ID = obj.ID; }
    }

    abstract class AGraph<NodeType>
        where NodeType : ANode
    {
        public int Dimension { get; set; }

        public abstract int GetNodeNum();

        public abstract int GetDegree(NodeType node);

        public abstract NodeType GetNeighbor(NodeType node, int i);

        public int CalcDistanceBFS(NodeType node1, NodeType node2)
        {
            var q = new Queue<NodeType>();
            var distance = new int[GetNodeNum()];
            for (int i = 0; i < GetNodeNum(); i++)
            {
                distance[i] = 1000;
            }

            distance[node1.ID] = 0;
            q.Enqueue(node1);

            while (q.Any())
            {
                NodeType current = q.Dequeue();

                for (int i = 0; i < GetDegree(current); i++)
                {
                    NodeType neighbor = GetNeighbor(current, i);
                    if (neighbor.ID == node2.ID)
                    {
                        int d = distance[current.ID] + 1;
                        return d;
                    }
                    if (distance[current.ID] < distance[neighbor.ID] - 1)
                    {
                        q.Enqueue(neighbor);
                        distance[neighbor.ID] = distance[current.ID] + 1;
                    }
                }
            }

            throw new Exception("Graph definition is wrong.");
        }

        public virtual int CalcDistance(NodeType node1, NodeType node2)
        {
            return CalcDistanceBFS(node1, node2);
        }
    }

}
