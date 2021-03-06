// minimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

void firstSplit(std::vector<std::vector<int> > matrixY, int countOfX, int countOfQ, std::vector<std::vector<int> >& classOfQ)
{
	int sA, sB;
	std::vector<int> allClasses;

	for (sA = 0; sA < countOfQ; ++sA)
	{
		std::vector<int> tempFirst;

		if (sA != 0) {
			if (std::find(allClasses.begin(), allClasses.end(), sA) != allClasses.end()) continue;
		}
		tempFirst.push_back(sA);
		allClasses.push_back(sA);

		for (sB = sA + 1; sB < countOfQ; ++sB)
		{
			int k = 0;
			for (int i = 0; i < countOfX; ++i)
			{
				if (matrixY[i][sA] != matrixY[i][sB]) break;
				k++;
			}
			if (k == countOfX) {
				tempFirst.push_back(sB);
				allClasses.push_back(sB);
					
			}
		}
		classOfQ.push_back(tempFirst);
	}
}

void getResult(std::vector<std::vector<int> > newClassOfQ, std::vector<std::vector<int> > matrixY, 
	std::vector<std::vector<int> > oldMatrixY, int countOfX, std::ostream& outputFile, int typeOfA) {

	std::vector<std::vector<int> > result;	
	const std::string dotFileName = "output.dot";
	std::ofstream dotFile(dotFileName);
	dotFile << "digraph Avtomatic {" << std::endl;

	outputFile << newClassOfQ.size() << std::endl;
	for (int x = 0; x < countOfX; ++x)
	{
		std::vector<int> tempFirst;
		std::vector<int> tempSecond;
		for (size_t i = 0; i < newClassOfQ.size(); ++i)
		{	
			tempFirst.push_back(matrixY[x][newClassOfQ[i].front()]);
			for (size_t j = 0; j < newClassOfQ.size(); ++j)
			{
				if (std::find(newClassOfQ[j].begin(), newClassOfQ[j].end(), oldMatrixY[x][newClassOfQ[i].front()]) != newClassOfQ[j].end()) {
					tempSecond.push_back(oldMatrixY[x][newClassOfQ[i].front()]);
				}
			}
		}
		for (size_t k = 0; k < tempFirst.size(); k++)	
		{
			if (typeOfA == 2) {
				outputFile << tempFirst[k] << " ";
				outputFile << tempSecond[k] << " ";
			}
			
			dotFile << "\"" << k << "\"" << "->";
			dotFile << "\"" << tempFirst[k] << "\"";
			dotFile << "[label=\"" << x + 1 << "/" << tempSecond[k] << "\"];" << std::endl;
		}

		if (typeOfA == 1) {
			if (x == 0) {
				for (size_t k = 0; k < tempSecond.size(); k++)	
				{
					outputFile << tempSecond[k] << " ";
				}
		        outputFile << std::endl;
			}
			for (size_t k = 0; k < tempFirst.size(); k++)	
			{
				outputFile << tempFirst[k] << " ";
			}
		}
		outputFile << std::endl;	
	}
	
	dotFile << "}" << std::endl;
	dotFile.close();
}

void split(std::vector<std::vector<int> > matrixY, std::vector<std::vector<int> > matrixQ, int countOfX, int& countOfNewClasses, 
	int countOfQ, std::vector<std::vector<int> > & classOfQ, bool& ok, std::vector<std::vector<int> > & result, std::ostream& outputFile, int typeOfA)
{
	int sA, sB;
	int oldCountOfNewClasses = countOfNewClasses;
	std::vector<int> allClasses;
	countOfNewClasses = 0;
	std::vector<std::vector<int> > newClassOfQ;
	for (sA = 0; sA < countOfQ; ++sA)
	{
		std::vector<int> tempFirst;
		if (sA != 0) {
			if (std::find(allClasses.begin(), allClasses.end(), sA) != allClasses.end()) continue;
		}
		tempFirst.push_back(sA);
		allClasses.push_back(sA);
		bool ex;
		int k = -1;
		int m = -1;
		for (sB = sA + 1; sB < countOfQ; ++sB)
		{

			ex = true;
			k = -1;
			m = -1;
			for (int i = 0; i < oldCountOfNewClasses; ++i)
			{
				for (size_t j = 0; j < classOfQ[i].size(); j++)
				{
					if (sB == classOfQ[i][j])
					{
						k = i;
					}
					if (sA == classOfQ[i][j])
					{
						m = i;
					}
			
					if ((k == m) && (k != -1)) {
						ex = false;
						break;
					}
				}

				if (!ex) break;
			}

			if (ex) continue;
			int k = 0;
				for (int j = 0; j < countOfX; ++j) 
				{
					if (matrixY[j][sA] != matrixY[j][sB]) break;
					k++;
				}
			if (k == countOfX) {
				tempFirst.push_back(sB);
				allClasses.push_back(sB);
			}
		}
		newClassOfQ.push_back(tempFirst);
		countOfNewClasses++;
	}
	classOfQ.resize(countOfNewClasses);
	for (int i = 0; i < countOfNewClasses; i++)
	{
		for (size_t j = 0; j < newClassOfQ[i].size(); j++)
			std::copy(newClassOfQ.begin(), newClassOfQ.end(), classOfQ.begin());
	}
	
	if (countOfNewClasses == oldCountOfNewClasses) {
		ok = false;
		getResult(newClassOfQ, matrixY, result, countOfX, outputFile, typeOfA);
	}
}

void rewriteItem(std::vector<std::vector<int> > classOfQ, std::vector<std::vector<int> > matrixQ, 
	std::vector<std::vector<int> >& matrixY, int countOfNewClasses, int k, int sA)
{
	for (int j = 0; j < countOfNewClasses; j++)
	{
		for (int i = 0; i < classOfQ[j].size(); i++)
		{
			if (classOfQ[j][i] == matrixQ[k][sA] /*- 1*/) {
				matrixY[k][sA] = j;
			}
		}
	}
}

void rewriteMatrix(std::vector<std::vector<int> > classOfQ, std::vector<std::vector<int> > matrixQ, 
	std::vector<std::vector<int> > & matrixY, int countOfX, int countOfNewClasses, int countOfQ)
{
	int sA;
	for (sA = 0; sA < countOfQ; sA++) 
	{
		for (int k = 0; k < countOfX; k++)
		{
			rewriteItem(classOfQ, matrixQ, matrixY, countOfNewClasses, k, sA);
		}
	}
}

void ReadMatrix(std::istream& input, int countOfX, int countOfY, int countOfQ, int typeOfA, std::ostream& outputFile)
{
	std::vector<std::vector<int> > matrixQ;
	std::vector<std::vector<int> > matrixY;
	std::vector<std::vector<int> > result;
	std::vector<std::vector<int> > classOfQ;

	if (typeOfA == 1) {
		std::vector<int> tempFirst;
		for (int j = 0; j < countOfQ; j++)
		{
			int q;
			input >> q;
			tempFirst.push_back(q);
		}
		for (int i = 0; i < countOfX; i++)
		{
			matrixY.push_back(tempFirst);
			matrixY.push_back(tempFirst);
			result.push_back(tempFirst);
			result.push_back(tempFirst);
			std::vector<int> tempSecond;
			for (int j = 0; j < countOfQ; j++)
			{
				int q;
				input >> q;
				tempSecond.push_back(q);
			}
			matrixQ.push_back(tempSecond);
		}
	}
	else {
		for (int i = 0; i < countOfX; i++)
		{
			std::vector<int> tempFirst;
			std::vector<int> tempSecond;
			for (int j = 0; j < countOfQ; j++)
			{
				int q;
				int y;
				input >> q >> y;
				tempFirst.push_back(q);
				tempSecond.push_back(y);
			}
			matrixQ.push_back(tempFirst);
			matrixY.push_back(tempSecond);
			result.push_back(tempSecond);
		}
	}
	firstSplit(matrixY, countOfX, countOfQ, classOfQ);
	int countOfNewClasses =  classOfQ.size();

	bool ok = true;
	while (ok) {
		rewriteMatrix(classOfQ, matrixQ, matrixY, countOfX, countOfNewClasses, countOfQ);
		split(matrixY, matrixQ, countOfX, countOfNewClasses, countOfQ, classOfQ, ok, result, outputFile, typeOfA);
	}
}

void GetArguments(std::istream& input)
{
	int typeOfA, countOfX, countOfY, countOfQ;
	input >> typeOfA >> countOfX >> countOfY >> countOfQ;

	const std::string outputFileName = "out12.txt";
	std::ofstream outputFile(outputFileName);
	if (!outputFile.is_open())
	{
		std::cout << "Input file cannot be opened." << std::endl;
	}

	outputFile << typeOfA << std::endl << countOfX << std::endl << countOfY << std::endl;
	
	ReadMatrix(input, countOfX, countOfY, countOfQ, typeOfA, outputFile);
}

int main(int argc, char* argv[])
{
	const std::string inputFileName = "in12.txt";

	std::ifstream inputFile(inputFileName);
	if (!inputFile.is_open())
	{
		std::cout << "Input file cannot be opened." << std::endl;
		return 1;
	}

	GetArguments(inputFile);
	
	return 0;
}