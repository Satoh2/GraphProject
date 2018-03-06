#include <iostream>
#include <fstream>

#include "Core\DEBUG.h"
#include "Graphs\Hypercube.h"
#include "Graphs\CrossedCube.h"

void test(int dim, int seed, int maxsteps, int trials);

int main() {
	int dim = 15;
	int seed = 0;
	int maxsteps = dim * 2;
	int trials = 10000;

	test(dim, seed, maxsteps, trials);
}

void test(int dim, int seed, int maxsteps, int trials)
{
	CrossedCube g;
	g.SetDimension(dim);
	g.SetRandSeed(seed);

	// 何種類の経路選択を行うか。今回は２つ
	const int cases = 2;
	std::function<int(int, int)> funcs[cases] = 
	{
		std::bind( // 既存の手法
			&CrossedCube::GetNext_Efe,
			std::ref(g),
			std::placeholders::_1,
			std::placeholders::_2),
		std::bind( // 提案手法
			&CrossedCube::GetNext_MasterThesis,
			std::ref(g),
			std::placeholders::_1,
			std::placeholders::_2),
	};

	// 実験する故障率を設定
	const int ratioCount = 28;
	double faultRatio[ratioCount] =
	{
		0.00, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09,
		0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19,
		0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 
	};

	// 結果を保持する配列の初期化
	int success[cases][ratioCount];
	int steps[cases][ratioCount];
	for (int i = 0; i < ratioCount; i++)
	{
		for (int j = 0; j < cases; j++)
		{
			success[j][i] = 0;
			steps[j][i] = 0;
		}
	}
	int *path = new int[maxsteps + 1];

	for (int fr = 0; fr < ratioCount; fr++)
	{
		for (int i = 0; i < trials; i++)
		{
			// 進捗の表示
			if (i % 100 == 0)
			{
				printf_s("\rfault ratio = %2.2f ... %d%%",
					faultRatio[fr], (i + 1) * 100 / trials);
			}

			// 故障ノードを設定
			g.GenerateFaults(faultRatio[fr]);

			// 出発ノードと目的ノードを設定
			int src, dst;
			g.GetNodePair(src, dst);

			for (int j = 0; j < cases; j++)
			{
				// ルーティング
				int step = g.Routing(src, dst, funcs[j], path, maxsteps);

				// 結果の保持と検証
				if (step > 0)
				{
					// 記録
					success[j][fr]++;
					steps[j][fr] += step;

					// 検証
					for (int i = 0; i < step; i++)
					{
						if (g.CalcDistance(path[i], path[i + 1]) == 1 && g.IsFault(path[i]))
						{
							std::cout << "!";
							getchar();
						}
					}
				}

			}
		}
		printf_s("\rfault ratio = %2.2f ... %d%%\n", faultRatio[fr], 100);
	}

	// 結果をcsvファイルに書き出し
	std::ofstream outputfile("test.csv");

	outputfile << "dimension = " << dim << ",trials = " << trials;
	outputfile << "\n\nSuccess ratio\n";
	for (int fr = 0; fr < ratioCount; fr++)
	{
		outputfile << "," << faultRatio[fr];
	}
	for (int i = 0; i < 2; i++)
	{
		outputfile << "\nCase" << i;
		for (int fr = 0; fr < ratioCount; fr++)
		{
			outputfile << "," << (double)success[i][fr] / trials;
		}
	}
	outputfile << "\n\n" << "Average steps\n";
	for (int fr = 0; fr < ratioCount; fr++)
	{
		outputfile << "," << faultRatio[fr];
	}
	for (int i = 0; i < 2; i++)
	{
		outputfile << "\nCase" << i;
		for (int fr = 0; fr < ratioCount; fr++)
		{
			outputfile << "," << (double)steps[i][fr] / success[i][fr];
		}
	}

	outputfile.close();

	delete[] path;
}