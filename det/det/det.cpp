#include "pch.h"

struct Avtomatic
{
	int id = -1;
	std::set<int> st;
};

void VisualPart(std::vector<std::vector<Avtomatic>> newA, std::set<int> newK)
{
	const std::string dotFileName = "output.dot";
	std::ofstream dotFile(dotFileName);
	dotFile << "digraph Avtomatic {" << std::endl;
	for (size_t i = 0; i < newA.size(); ++i)
	{
		if (newK.find(i) != newK.end())
		{
			dotFile << i << " [color = red]" << std::endl;
		}
		else 
		{
			dotFile << i << std::endl;
		}
	}

	for (size_t i = 0; i < newA.size(); ++i)
	{
		for (size_t j = 0; j < newA[i].size(); ++j)
		{
			if (newA[i][j].id != -1)
			{
				dotFile << "	" << i << "->" << newA[i][j].id << "[label=" << j << ']' << std::endl;
			}
		}
	}

	dotFile << "}";
	dotFile.close();
}

void PrintNewAvtomatic(int x, std::vector<std::vector<Avtomatic>> newA, std::set<int> newK)
{
	const std::string outputFileName = "output.txt";
	std::ofstream outputFile(outputFileName);

	outputFile << x << std::endl << newA.size() << std::endl << newK.size() << std::endl;
	for (std::set<int>::iterator it = newK.begin(); it != newK.end(); ++it)
	{
		outputFile << *it << ' ';
	}
	outputFile << std::endl;
	for (int i = 0; i < newA[0].size(); ++i)
	{
		for (int j = 0; j < newA.size(); ++j)
		{
			outputFile << newA[j][i].id << " ";
		}
		outputFile << std::endl;
	}
}

void AddSt(const std::set<int>& oldSt, std::queue<std::set<int>>& queue, int x, std::vector<std::vector<Avtomatic>>& A,
	std::map<std::set<int>, int>& states, std::set<int>& K, std::vector<std::vector<Avtomatic>>& newA, std::set<int>& newK)
{
	std::vector<Avtomatic> st(x);
	for (int i = 0; i < A[0].size(); ++i)
	{
		for (std::set<int>::iterator it = oldSt.begin(); it != oldSt.end(); ++it)
		{
			for (auto state : A[*it][i].st)
			{
				st[i].st.insert(state);
			}
		}

		if (st[i].st.size() != 0)
		{
			if (states.find(st[i].st) == states.end())
			{
				st[i].id = states.size();
				states.insert(std::pair<std::set<int>, int>(st[i].st, st[i].id));
				queue.push(st[i].st);
				for (std::set<int>::iterator it = K.begin(); it != K.end(); ++it)
				{
					if (st[i].st.find(*it) != st[i].st.end())
					{
						newK.insert(st[i].id);
					}
				}
			}
			else
			{
				st[i].id = states.find(st[i].st)->second;
			}
		}
	}
	newA.push_back(st);
}

void AutomaticConversion(std::set<int> K, int x, std::vector<std::vector<Avtomatic>>& A)
{
	std::vector<std::vector<Avtomatic>> newA;
	std::set<int> newK;
	std::queue<std::set<int>> Q;
	std::map<std::set<int>, int> states;
	states.insert(std::pair<std::set<int>, int>({ 0 }, 0));
	std::set<int> set = { 0 };
	Avtomatic st; 
	for (std::set<int>::iterator it = K.begin(); it != K.end(); ++it)
	{
		if (st.st.find(*it) != st.st.end())
		{
			newK.insert(st.id);
		}
	}
	AddSt(set, Q, x, A, states, K, newA, newK);
	while (!Q.empty())
	{
		std::set<int> st = Q.front();
		Q.pop();
		AddSt(st, Q, x, A, states, K, newA, newK);
	}

	PrintNewAvtomatic(x, newA, newK);
	VisualPart(newA, newK);
}

void ReadData(std::ifstream& inputFile)
{
	int x, z, k;
	inputFile >> x >> z >> k;
	std::vector<std::vector<Avtomatic>> A(z, std::vector<Avtomatic>(x));
	std::set<int> K;
	std::string line;

	for (int i = 0; i < k; ++i)
	{
		int st;
		inputFile >> st;
		K.insert(st);
	}

	for (int i = 0; i < A.size(); ++i)
	{
		for (int j = 0; j < A[i].size(); ++j)
		{
			Avtomatic st;
			A[i][j] = st;
		}
	}

	std::getline(inputFile, line);
	int i = 0;
	while (std::getline(inputFile, line))
	{
		if (!empty(line))
		{
			std::stringstream stream(line);
			int st, s;
			while (stream >> st && stream >> s)
			{
				A[i][s].st.insert(st);
			}
		}
		i++;
	}

	AutomaticConversion(K, x, A);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Wrong number of arguments." << std::endl;
		std::cout << "Usage: det.exe <input file>" << std::endl;
		return 1;
	}

	const std::string inputFileName = argv[1];

	std::ifstream inputFile(inputFileName);
	if (!inputFile.is_open())
	{
		std::cout << "Input file cannot be opened." << std::endl;
		return 1;
	}

	ReadData(inputFile);

	return 0;
}