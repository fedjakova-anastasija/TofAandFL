#include "pch.h"
#include "Token.h"
using namespace std;

namespace Lexer {
	enum class AvtomaticState
	{
		STATE_HIDE,
		STATE_READ,
		STATE_ONWARD
	};

	vector<string> KEYWORDS;
	vector<string> OPERATIONS;
	vector<string> DELIMITERS;
	vector<char> HEXADECIMAL_ALPHABET;
	vector<char> OCTAL_ALPHABET;
	vector<char> BINARY_ALPHABET;
	const string SYMBOL_ALPHABET = "abcdefghijklmnopqrstuvwxyz";
	const string NUMBER_ALPHABET = "0123456789";
	const string REAL_NUMBER = NUMBER_ALPHABET + '.' + 'e' + '+' + '-';
	const string ID = SYMBOL_ALPHABET + NUMBER_ALPHABET;
}

const vector<pair<string, string>> INCOMPATIBLE_TYPES =
{
	pair<string, string>("INT", "ID"),
	pair<string, string>("INT", "REGULAR"),
	pair<string, string>("INT", "REAL"),
	pair<string, string>("INT", "ERROR"),
	pair<string, string>("INT", "INT"),
	pair<string, string>("REAL", "ID"),
	pair<string, string>("REAL", "REGULAR"),
	pair<string, string>("REAL", "ERROR"),
	pair<string, string>("REAL", "REAL"),
	pair<string, string>("REGULAR", "ERROR"),
	pair<string, string>("ID", "ERROR"),
	pair<string, string>("ID", "ID")

};


void initializeRegularWords()
{
	Lexer::KEYWORDS.push_back("begin");
	Lexer::KEYWORDS.push_back("end");
	Lexer::KEYWORDS.push_back("if");
	Lexer::KEYWORDS.push_back("else");
	Lexer::KEYWORDS.push_back("then");
	Lexer::KEYWORDS.push_back("while");
	Lexer::KEYWORDS.push_back("do");
	Lexer::KEYWORDS.push_back("var");
	Lexer::KEYWORDS.push_back("int");
	Lexer::KEYWORDS.push_back("char");
	Lexer::KEYWORDS.push_back("string");
	Lexer::KEYWORDS.push_back("real");
	Lexer::KEYWORDS.push_back("float");
}

void initializeOperations()
{
	Lexer::OPERATIONS.push_back("+");
	Lexer::OPERATIONS.push_back("-");
	Lexer::OPERATIONS.push_back("/");
	Lexer::OPERATIONS.push_back("*");
	Lexer::OPERATIONS.push_back("=");
}

void initializeDelimiters()
{
	Lexer::DELIMITERS.push_back("(");
	Lexer::DELIMITERS.push_back(")");
	Lexer::DELIMITERS.push_back(";");
	Lexer::DELIMITERS.push_back(",");
	Lexer::DELIMITERS.push_back(":");
}

void initializeHexadecimalAlphabet()
{
	Lexer::HEXADECIMAL_ALPHABET.push_back('0');
	Lexer::HEXADECIMAL_ALPHABET.push_back('1');
	Lexer::HEXADECIMAL_ALPHABET.push_back('2');
	Lexer::HEXADECIMAL_ALPHABET.push_back('3');
	Lexer::HEXADECIMAL_ALPHABET.push_back('4');
	Lexer::HEXADECIMAL_ALPHABET.push_back('5');
	Lexer::HEXADECIMAL_ALPHABET.push_back('6');
	Lexer::HEXADECIMAL_ALPHABET.push_back('7');
	Lexer::HEXADECIMAL_ALPHABET.push_back('8');
	Lexer::HEXADECIMAL_ALPHABET.push_back('9');
	Lexer::HEXADECIMAL_ALPHABET.push_back('a');
	Lexer::HEXADECIMAL_ALPHABET.push_back('b');
	Lexer::HEXADECIMAL_ALPHABET.push_back('c');
	Lexer::HEXADECIMAL_ALPHABET.push_back('d');
	Lexer::HEXADECIMAL_ALPHABET.push_back('e');
	Lexer::HEXADECIMAL_ALPHABET.push_back('f');
}

void initializeOctalAlphabet()
{
	Lexer::OCTAL_ALPHABET.push_back('0');
	Lexer::OCTAL_ALPHABET.push_back('1');
	Lexer::OCTAL_ALPHABET.push_back('2');
	Lexer::OCTAL_ALPHABET.push_back('3');
	Lexer::OCTAL_ALPHABET.push_back('4');
	Lexer::OCTAL_ALPHABET.push_back('5');
	Lexer::OCTAL_ALPHABET.push_back('6');
	Lexer::OCTAL_ALPHABET.push_back('7');
}
void initializeBinaryAlphabet()
{
	Lexer::BINARY_ALPHABET.push_back('0');
	Lexer::BINARY_ALPHABET.push_back('1');
}

Lexer::AvtomaticState state = Lexer::AvtomaticState::STATE_ONWARD;

bool isCommentEnd(char symbol) {
	if (symbol == '}' && state == Lexer::AvtomaticState::STATE_HIDE)
	{
		state = Lexer::AvtomaticState::STATE_ONWARD;
		return true;
	}
	return false;
}

bool isComment(char symbol)
{
	if (state == Lexer::AvtomaticState::STATE_HIDE)
	{
		return true;
	}
	else if (symbol == '{')
	{
		state = Lexer::AvtomaticState::STATE_HIDE;
		return true;
	}
	return false;
}

Token stringToken;
string result;
bool isEcraning = false;

bool isStringEnd(const char symbol)
{
	return (symbol == '"' && state == Lexer::AvtomaticState::STATE_READ && !isEcraning);
}

string getResult()
{
	return result;
}

Token getStringToken()
{
	return stringToken;
}

Lexer::AvtomaticState getReadState()
{
	return state;
}

void setReadState(Lexer::AvtomaticState readState)
{
	state = readState;
}

bool isString(const char symbol)
{
	return state == Lexer::AvtomaticState::STATE_READ;
}

bool isStringBegin(const char symbol)
{
	return (symbol == '"' && state == Lexer::AvtomaticState::STATE_ONWARD);
}

vector <string> id;
ofstream idFile;

bool isIndent(char arg)
{
	if (arg == '\n' || arg == ' ' || arg == '	') {
		return true;
	}
	return false;
}

void handleIncompatibleTypes(vector <Token> &tokens)
{
	vector <Token> processedTokens;
	if (!tokens.empty())
	{
		processedTokens.push_back(tokens[0]);
	}
	for (size_t i = 0; i < tokens.size() - 1; ++i)
	{
		if ((find(INCOMPATIBLE_TYPES.begin(), INCOMPATIBLE_TYPES.end(), pair<string, string>(tokens[i].getType(), tokens[i + 1].getType())) != INCOMPATIBLE_TYPES.end()) ||
		(find(INCOMPATIBLE_TYPES.begin(), INCOMPATIBLE_TYPES.end(), pair<string, string>(tokens[i + 1].getType(), tokens[i].getType())) != INCOMPATIBLE_TYPES.end())) {
			processedTokens[processedTokens.size() - 1].setType(TokenType::ERROR);
			processedTokens[processedTokens.size() - 1].setValue(processedTokens[processedTokens.size() - 1].getValue() + tokens[i + 1].getValue());
			continue;
		}
		processedTokens.push_back(tokens[i + 1]);
	}
	tokens = processedTokens;
}

void printTokens(const vector <Token> &tokens)
{
	for (const Token &element : tokens)
	{
		element.printToken();
	}
}
void printAllTokens(const vector <Token> &tokens)
{
	for (const Token &element : tokens)
	{
		element.printAllToken();
	}
}
vector <Token> allTokens;
void getTokensFromString(const string &arg)
{
	size_t stringPos = 0;
	Token currentToken;
	Token nonErrorToken;
	vector <Token> tokens;
	nonErrorToken.setType(TokenType::ERROR);
	if (arg.empty())
	{
		return;
	}
	string result;
	for (size_t i = 0; i < arg.length(); ++i)
	{
		result = "";
		nonErrorToken.setType(TokenType::ERROR);
		for (size_t j = i; j < arg.length(); ++j)
		{
			result += arg[j];
			currentToken.tokenFromString(result);
			if (currentToken.getType() == "ERROR" && j == i)
			{
				break;
			}
			if (currentToken.getType() != "ERROR")
			{
				nonErrorToken = currentToken;
			}
		}
		if (nonErrorToken.getType() == "ERROR")
		{
			nonErrorToken.setValue(result);
		}
		i += nonErrorToken.getValue().length() - 1;
		tokens.push_back(nonErrorToken);
		allTokens.push_back(nonErrorToken);
		currentToken.clear();
		nonErrorToken.clear();
	}
	handleIncompatibleTypes(tokens);
	handleIncompatibleTypes(allTokens);
	//printTokens(tokens);
	//printAllTokens(allTokens);
}

bool find(const string& what, vector<string> WHERE)
{
	return find(WHERE.begin(), WHERE.end(), what) != WHERE.end();
}

bool Token::isRegularWord(const string& arg)
{
	return find(toLowerCase(arg), Lexer::KEYWORDS);
}

bool Token::isOperation(const string& arg)
{
	return find(arg, Lexer::OPERATIONS);
}

bool Token::isDelimiter(const string& arg)
{
	return find(arg, Lexer::DELIMITERS);
}

bool Token::isNumber(const string &arg, const char &ch, vector<char> alphabet)
{
	if (arg.length() < 3 || arg.front() != '0' || tolower(arg[1]) != ch)
		return false;
	for (size_t i = 2; i < arg.length(); ++i)
	{
		if (find(alphabet.begin(), alphabet.end(), arg[i]) == alphabet.end())
			return false;
	}
	return true;
}


size_t Token::tokenFromString(string arg)
{
	if (arg.empty())
	{
		setValue("");
		setType(TokenType::ERROR);
		return 0;
	}
	if (isIntNumber(arg))
	{
		setType(TokenType::INT);
	}
	else if (isNumber(arg, 'x', Lexer::HEXADECIMAL_ALPHABET))
	{
		setType(TokenType::INT);
	}
	else if (isNumber(arg, 'o', Lexer::OCTAL_ALPHABET))
	{
		setType(TokenType::INT);
	}
	else if (isNumber(arg, 'b', Lexer::BINARY_ALPHABET))
	{
		setType(TokenType::INT);
	}
	else if (isRealNumber(arg))
	{
		setType(TokenType::REAL);
	}
	else if (isRegularWord(arg))
	{
		setType(TokenType::REGULAR);
	}
	else if (isId(arg))
	{
		setType(TokenType::ID);
	}
	else if (isOperation(arg))
	{
		setType(TokenType::OPERATION);
	}
	else if (isDelimiter(arg))
	{
		setType(TokenType::DELIMITER);
	}
	else {
		setType(TokenType::ERROR);
	}
	setValue(arg);
	return arg.length();
}
void Token::clear()
{
	value = "";
	type = ERROR;
}

string Token::getValue() const
{
	return value;
}

void Token::setValue(const string &argValue)
{
	value = argValue;
}

void Token::setType(TokenType argType)
{
	type = argType;
}

string Token::getType() const
{
	string stringType = "";
	switch (type) {
		case ID:
			stringType = "ID";
			break;
		case INT:
			stringType = "INT";
			break;
		case REAL:
			stringType = "REAL";
			break;
		case OPERATION:
			stringType = "OPERATION";
			break;
		case COMMENT:
			stringType = "COMMENT";
			break;
		case DELIMITER:
			stringType = "DELIMITER";
			break;
		case DOT_DELIMITER:
			stringType = "DELIMITER";
			break;
		case REGULAR:
			stringType = "REGULAR";
			break;
		case STRING:
			stringType = "STRING";
			break;
		case UNCLOSED_STRING:
			stringType = "ERROR";
			break;
		default:
			stringType = "ERROR";
			break;
	}
	return stringType;
}

void Token::printToken() const
{
	if (!getValue().empty())
	{
		cout << getType() + ": " + getValue() << endl;
	}
}
void Token::printAllToken() const
{
	if (!getValue().empty())
	{
		cout << getValue() << endl;
	}
}

string Token::toLowerCase(const string &arg)
{
	string result;
	for (char symbol : arg)
	{
		result += (char)tolower(symbol);
	}
	return result;
}


bool Token::isId(const string &arg)
{
	if (!arg.empty())
	{
		if ((find(Lexer::SYMBOL_ALPHABET.begin(), Lexer::SYMBOL_ALPHABET.end(), tolower(arg[0])) == Lexer::SYMBOL_ALPHABET.end()) || arg.length() > 30)
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	for (char symbol : arg)
	{
		if (find(Lexer::ID.begin(), Lexer::ID.end(), tolower(symbol)) == Lexer::ID.end())
		{
			return false;
		}
	}
	return true;
}

bool Token::isIntNumber(const string &arg)
{
	if (arg.empty())
		return false;
	for (size_t i = 0; i < arg.length(); ++i)
	{
		if (find(Lexer::NUMBER_ALPHABET.begin(), Lexer::NUMBER_ALPHABET.end(), arg[i]) == Lexer::NUMBER_ALPHABET.end())
			return false;
	}
	return true;
}

bool Token::isRealNumber(const string &arg)
{
	if 
	(
		arg.empty() 
		|| (arg.length() == 1 && arg.front() == '.')
		|| arg.front() == 'e'
		|| arg.back() == 'e'	
		|| arg.back() == '+'
		|| arg.back() == '-'
	)
		return false;
	bool isDotUsed = false;
	bool isEUsed = false;
	bool isExpectedOperation = false;
	bool isOperationUsed = false;
	for (size_t i = 0; i < arg.length(); ++i)
	{
		if
		(
			find(Lexer::REAL_NUMBER.begin(), Lexer::REAL_NUMBER.end(), arg[i]) == Lexer::REAL_NUMBER.end()
			|| (arg[i] == '.' && isDotUsed)
			|| (arg[i] == 'e' && isEUsed)
			|| (isExpectedOperation && arg[i] != '+' && arg[i] != '-')
			|| (isOperationUsed && (arg[i] == '+' || arg[i] == '-'))
			|| (arg[i] == 'e' && isEUsed)
			|| (!isExpectedOperation && (arg[i] == '+' || arg[i] == '-'))
		)
			return false;
		if (arg[i] == '.')
		{
			isDotUsed = true;
		}
		if (arg[i] == 'e')
		{
			isEUsed = true;
			isExpectedOperation = true;
		}
		else
		{
			isExpectedOperation = false;
		}
		if ((arg[i] == '+' || arg[i] == '-') && !isOperationUsed)
		{
			isOperationUsed = true;
		}
	}
	return true;
}

bool CheckId(size_t& j)
{
    if (allTokens[j + 1].getValue() == ",")
    {
        if (allTokens[j + 2].getType() == "ID")
        {
			j += 2;
            return CheckId(j);
        }
    }
    else
    {
        return true;
    }

    return false;
}

bool CheckVar()
{
	bool isOk = true;
	if (((allTokens[0].getValue() == "var") || (allTokens[0].getValue() == "VAR")) && (allTokens[allTokens.size() - 1].getValue() == ";"))
	{
		size_t i = 1;
		while (i != allTokens.size() && isOk)
		{
			if ((find(Lexer::KEYWORDS.begin(), Lexer::KEYWORDS.end(), allTokens[i].getType()) == Lexer::KEYWORDS.end()) && ((allTokens[i].getValue() == "INT") || (allTokens[i].getValue() == "REAL") || (allTokens[i].getValue() == "STRING") || (allTokens[i].getValue() == "FLOAT") || (allTokens[i].getValue() == "CHAR") || (allTokens[i].getValue() == "int") || (allTokens[i].getValue() == "real") || (allTokens[i].getValue() == "string") || (allTokens[i].getValue() == "float") || (allTokens[i].getValue() == "char")) && (allTokens[i + 1].getValue() == ":"))
			{
				size_t j = i + 2;
				isOk = false;
				while (allTokens[j].getValue() != ";")
				{
					isOk = true;
					if (allTokens[j].getType() != "ID")
					{
						isOk = false;
						break;
					}
					else
					{
						isOk = CheckId(j);
						if (!isOk)
						{
							break;
						}
					}
					j++;
				}
				i = j + 1;
			}			
			else
			{
				isOk = false;
				break;
			}
		}
	}
	else
	{
		isOk = false;
	}
	return isOk;
}

bool fileProcessing(ifstream &input)
{
	string line;
	string result;
	Token token;
	Token stringToken;
	bool isNeedPrintStringToken = false;
	bool isProcessingString = false;
	while (!input.eof())
	{
		char symbol;
		input.get(symbol);
		if (isCommentEnd(symbol) && !isProcessingString || input.eof())
		{
			symbol = ' ';
		}
		else if (isComment(symbol) && !isProcessingString)
		{
			continue;
		}
		if (isStringBegin(symbol))
		{
			if (input.eof())
			{
				stringToken.setType(TokenType::UNCLOSED_STRING);
				isNeedPrintStringToken = true;
			}
			else
			{
				setReadState(Lexer::AvtomaticState::STATE_READ);
				stringToken.setValue(stringToken.getValue() + symbol);
				stringToken.setType(TokenType::STRING);
				isProcessingString = true;
				continue;
			}
		}
		else if (isStringEnd(symbol))
		{
			if (input.eof())
			{
				stringToken.setType(TokenType::UNCLOSED_STRING);
				isNeedPrintStringToken = true;
			}
			else
			{
				if (!isEcraning)
				{
					setReadState(Lexer::AvtomaticState::STATE_ONWARD);
				}
				stringToken.setValue(stringToken.getValue() + symbol);
				isNeedPrintStringToken = true;
				symbol = ' ';
				isProcessingString = false;
			}
		}
		else if (isString(symbol))
		{
			if (input.eof())
			{
				stringToken.setType(TokenType::UNCLOSED_STRING);
				isNeedPrintStringToken = true;
			}
			else
			{
				stringToken.setValue(stringToken.getValue() + symbol);
				if (symbol == '\\')
				{
					isEcraning = true;
				}
				else
				{
					isEcraning = false;
				}
				continue;
			}
		}
		if (isIndent(symbol) || input.eof())
		{
			getTokensFromString(result);
			result.clear();
		}
		else {
			result += symbol;
		}
		if (isNeedPrintStringToken)
		{
			stringToken.printToken();
			isNeedPrintStringToken = false;
			stringToken.clear();
		}
	}
	//printTokens(allTokens);
	return CheckVar();
}
int main()
{
	const string inputFileName = "in.txt";

	ifstream inputFile(inputFileName);

	if (!inputFile.is_open())
	{
		cout << "Input file cannot be opened." << endl;
		return 1;
	}

	initializeRegularWords();
	initializeOperations();
	initializeDelimiters();
	initializeHexadecimalAlphabet();
	initializeOctalAlphabet();
	initializeBinaryAlphabet();

	bool isOk = false;
	isOk = fileProcessing(inputFile);
	if (isOk)
	{
		cout << "ok" << endl;
	}
	else
	{
		cout << "not ok" << endl;
	}
	return 0;
}
