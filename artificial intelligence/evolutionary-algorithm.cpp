#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <cstdlib>

using namespace std;

float pc = 0.9;
float pm = 0.5;

int evaluate(vector<int> P)
{
	int count = 0;
	for (int i = 0; i < P.size() - 1; i++)
	{
		for (int j = i + 1; j < P.size(); j++)
		{
			if (P[i] == (P[j] - j + i) || P[i] == (P[j] - i + j))
				count++;
		}
	}
	return count;
}

vector<vector<int>> selection(vector<vector<int>> P)
{
	int i1 = 0;
	int i2 = 0;
	vector<vector<int>> Pn = P;
	for (int i = 0; i < P.size(); i++)
	{
		i1 = rand() % P.size();
		i2 = rand() % P.size();
		if (i1 != i2)
		{
			if (evaluate(P[i1]) <= evaluate(P[i2]))
				Pn[i] = P[i1];
			else
				Pn[i] = P[i2];
		}
	}
	return Pn;
}

vector<vector<int>> crossover(vector<vector<int>> P)
{
	vector<vector<int>> Pn = P;
	int mapstart = rand() % Pn[0].size();
	int mapend = rand() % Pn[0].size();
	vector<int> tmp1;
	vector<int> tmp2;
	int tmp = 0;
	float rr = 0;
	bool flaga = true;
	for (int i = 0; i < Pn.size() - 2; i = i + 2)
	{
		mapstart = rand() % Pn[0].size();
		mapend = rand() % Pn[0].size();
		if (mapstart > mapend)
		{
			tmp = mapstart;
			mapstart = mapend;
			mapend = tmp;
		}
		rr = (float)rand() / RAND_MAX;
		if (rr < pc)
		{
			for (int j = mapstart; j <= mapend; j++)
			{
				tmp1.push_back(Pn[i][j]);
				tmp2.push_back(Pn[i + 1][j]);
				tmp = Pn[i][j];
				Pn[i][j] = Pn[i + 1][j];
				Pn[i + 1][j] = tmp;
			}
			random_shuffle(tmp1.begin(), tmp1.end());
			random_shuffle(tmp2.begin(), tmp2.end());
			for (int j = 0; j < Pn[i].size(); j++)
			{
				if (j >= mapstart && j <= mapend)
					continue;
				for (int k = mapstart; k <= mapend; k++)
				{
					if (Pn[i][j] == Pn[i][k])
					{
						for (int a = 0; a < tmp1.size(); a++)
						{
							flaga = true;
							for (int b = 0; b < Pn[0].size(); b++)
							{
								if (tmp1[a] == Pn[i][b])
									flaga = false;
							}
							if (flaga == true)
								Pn[i][j] = tmp1[a];
						}
					}
					if (Pn[i + 1][j] == Pn[i + 1][k])
					{
						for (int a = 0; a < tmp2.size(); a++)
						{
							flaga = true;
							for (int b = 0; b < Pn[0].size(); b++)
							{
								if (tmp2[a] == Pn[i + 1][b])
									flaga = false;
							}
							if (flaga == true)
								Pn[i + 1][j] = tmp2[a];
						}
					}
				}
			}
		}
	}
	return Pn;
}

vector<vector<int>> mutation(vector<vector<int>> P)
{
	int r1 = 0;
	int r2 = 0;
	int tmp = 0;
	vector<vector<int>> Pn = P;
	for (int i = 0; i < Pn.size(); i++)
	{
		if ((float)rand() / RAND_MAX < pm)
		{
			do
			{
				r1 = rand() % Pn[i].size();
				r2 = rand() % Pn[i].size();
			} while (r1 == r2);
			tmp = Pn[i][r1];
			Pn[i][r1] = Pn[i][r2];
			Pn[i][r2] = tmp;
		}
	}
	return Pn;
}

int main()
{
	setlocale(LC_ALL, "Polish");
	srand(time(NULL));
	while (true) {
		int n = 8;
		int pop = 10;
		int genmax = 10000;
		vector<int> tab;
		vector<int> evo;
		vector<vector<int>> Pn;
		vector<vector<int>> P;
		for (int i = 0; i < n; i++)
		{
			tab.push_back(i + 1);
		}
		for (int i = 0; i < pop; i++)
		{
			random_shuffle(tab.begin(), tab.end());
			Pn.push_back(tab);
			evo.push_back(evaluate(tab));
		}
		P = Pn;
		int gen = 0;
		int best = min_element(evo.begin(), evo.end()) - evo.begin();
		int min = *min_element(evo.begin(), evo.end());
		while (gen < genmax && min>0)
		{
			Pn = selection(P);
			Pn = crossover(Pn);
			Pn = mutation(Pn);
			evo.clear();
			for (int i = 0; i < pop; i++)
			{
				evo.push_back(evaluate(Pn[i]));
			}
			best = min_element(evo.begin(), evo.end()) - evo.begin();
			min = *min_element(evo.begin(), evo.end());
			P = Pn;
			gen++;
		}
		cout << min << endl;
		for (int i = 0; i < P[best].size(); i++)
			cout << P[best][i] << " ";
		cout << endl;

		
		getchar();
	}
	return 0;
}
