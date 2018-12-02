// Determination.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include "detor.h"
struct StateMachineState
{
	StateMachineState(int index, std::set<int> sts)
	{
		states = sts;
		stateIndex = index;
	}

	int stateIndex = -1;
	std::set<int> states;
};

void ShowDeterminatedStateMachine(int _numberOfInputSignals, std::vector<std::vector<StateMachineState>> _determinatedStateMachine, std::set<int> _determinatedFinalStates)
{
	const std::string outputFileName = "output.txt";
	std::ofstream outputFile(outputFileName);
	if (!outputFile.is_open())
	{
		std::cout << "Can't open the file for writing! " << outputFileName << std::endl;

		return;
	}

	outputFile << _numberOfInputSignals << std::endl;
	outputFile << _determinatedStateMachine.size() << std::endl;
	outputFile << _determinatedFinalStates.size() << std::endl;
	for (auto finalState : _determinatedFinalStates)
	{
		outputFile << finalState << " ";
	}

	outputFile << std::endl;
	for (int i = 0; i < _determinatedStateMachine[0].size(); ++i)
	{
		for (int j = 0; j < _determinatedStateMachine.size(); ++j)
		{
			outputFile << _determinatedStateMachine[j][i].stateIndex << " ";
		}

		outputFile << std::endl;
	}
}

void InsertStatesInState(const std::set<int>& initStates, StateMachineState& state, int initStateIndex, std::vector<std::vector<StateMachineState>> _initialStateMachine)
{

	bool isFirstIteration = false;
	for (auto initState : initStates)
	{
		if (isFirstIteration)
		{
			StateMachineState st;
			state = st;
		}

		for (auto st : _initialStateMachine[initState][initStateIndex].states)
		{
			state.states.insert(st);
		}
	}
}

void AddStateInFinalStates(StateMachineState state, std::set<int>& _finalStates, std::set<int>& _determinatedFinalStates)
{
	for (auto finalState : _finalStates)
	{
		std::cout << state.states.count(finalState) << std::endl;
		if (state.states.count(finalState))
		{
			_determinatedFinalStates.insert(state.stateIndex);
			return;
		}
	}
}

void AddNewState(const std::set<int>& initStates, std::queue<std::set<int>>& queue, int _numberOfInputSignals, std::vector<std::vector<StateMachineState>>& _initialStateMachine, std::map<std::set<int>, int>& _statesList, std::set<int>& _finalStates, std::vector<std::vector<StateMachineState>>& _determinatedStateMachine, std::set<int>& _determinatedFinalStates)
{
	std::vector<StateMachineState> states(_numberOfInputSignals);

	for (int i = 0; i < _initialStateMachine[0].size(); ++i)
	{
		InsertStatesInState(initStates, states[i], i, _initialStateMachine);

		if (states[i].states.size() != 0)
		{
			auto search = _statesList.find(states[i].states);
			if (search == _statesList.end())
			{
				states[i].stateIndex = _statesList.size();
				_statesList.insert(std::pair<std::set<int>, int>(states[i].states, states[i].stateIndex));
				queue.push(states[i].states);
				AddStateInFinalStates(states[i], _finalStates, _determinatedFinalStates);
			}
			else
			{
				states[i].stateIndex = search->second;
			}
		}
	}

	_determinatedStateMachine.push_back(states);
}

void DeterminateStateMachine(std::set<int> _finalStates, int _numberOfInputSignals, std::vector<std::vector<StateMachineState>>& _initialStateMachine)
{
	std::vector<std::vector<StateMachineState>> _determinatedStateMachine;
	std::set<int> _determinatedFinalStates;
	std::queue<std::set<int>> statesQueue;
	std::map<std::set<int>, int> _statesList;
	_statesList.insert(std::pair<std::set<int>, int>({ 0 }, 0));
	std::set<int> set = { 0 };
	StateMachineState st(0, set);
	AddStateInFinalStates(st, _finalStates, _determinatedFinalStates);
	AddNewState(set, statesQueue, _numberOfInputSignals, _initialStateMachine, _statesList, _finalStates, _determinatedStateMachine, _determinatedFinalStates);
	while (statesQueue.size() != 0)
	{
		std::set<int> state = statesQueue.front();
		statesQueue.pop();
		AddNewState(state, statesQueue, _numberOfInputSignals, _initialStateMachine, _statesList, _finalStates, _determinatedStateMachine, _determinatedFinalStates);
	}

	ShowDeterminatedStateMachine(_numberOfInputSignals, _determinatedStateMachine, _determinatedFinalStates);
}

void ReadStateMachineInfoFromFile(std::ifstream& inputFile)
{
	int _numberOfInputSignals;
	int _numberOfStates;
	int _numberOfFinalStates;

	inputFile >> _numberOfInputSignals;
	inputFile >> _numberOfStates;
	inputFile >> _numberOfFinalStates;
	std::set<int> _finalStates;

	for (int i = 0; i < _numberOfFinalStates; ++i)
	{
		int tempNumb;
		inputFile >> tempNumb;
		_finalStates.insert(tempNumb);
	}

	std::vector<std::vector<StateMachineState>> _initialStateMachine(_numberOfStates, std::vector<StateMachineState>(_numberOfInputSignals));
	
	std::cout << _initialStateMachine.size() << std::endl;
	for (int i = 0; i < _initialStateMachine.size(); ++i)
	{
		std::cout << "size" << std::endl;
		for (int j = 0; j < _initialStateMachine[i].size(); ++j)
		{
			std::cout << _initialStateMachine[i].size() << std::endl;
			StateMachineState st;
			_initialStateMachine[i][j] = st;
			std::cout << st.stateIndex << std::endl;
		}
	}

	std::string str;
	std::getline(inputFile, str);
	int index = 0;
	while (std::getline(inputFile, str))
	{
		if (index < _numberOfStates)
		{
			std::stringstream stream(str);
			int state;
			int signal;
			while (stream >> state >> signal)
			{
				_initialStateMachine[index][signal].states.insert(state);
			}
		}

		index++;
	}

	DeterminateStateMachine(_finalStates, _numberOfInputSignals, _initialStateMachine);
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

	ReadStateMachineInfoFromFile(inputFile);
	/*CreateDotFile(argv[2]);*/

	return 0;
}
