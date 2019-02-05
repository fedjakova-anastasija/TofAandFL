#include "pch.h"
#include "LexicalAnalysator.h"
#include "Constants.h"
#include "Token.h"

//using namespace std;
//
//bool LexicalAnalysator::isIndent(char arg)
//{
//	return find(Constants::INDENTS.begin(), Constants::INDENTS.end(), arg) != Constants::INDENTS.end();
//}
//
//bool LexicalAnalysator::isIncompatibleTypes(const Token &lhs,const Token &rhs)
//{
//	return find(Constants::INCOMPATIBLE_TYPES.begin(), Constants::INCOMPATIBLE_TYPES.end(), pair<string, string>(lhs.getType(), rhs.getType())) != Constants::INCOMPATIBLE_TYPES.end();
//}
//
//void LexicalAnalysator::handleIncompatibleTypes(vector <Token> &tokens)
//{
//	vector <Token> processedTokens;
//	if (!tokens.empty())
//	{
//		processedTokens.push_back(tokens[0]);
//	}
//	for (size_t i = 0; i < tokens.size() - 1; ++i)
//	{
//		if (isIncompatibleTypes(tokens[i], tokens[i + 1]))
//		{
//			processedTokens[processedTokens.size() - 1].setType(TokenType::ERROR);
//			processedTokens[processedTokens.size() - 1].setValue(processedTokens[processedTokens.size() - 1].getValue() + tokens[i + 1].getValue());
//			continue;
//		}
//		processedTokens.push_back(tokens[i + 1]);
//	}
//	tokens = processedTokens;
//}
//
//void LexicalAnalysator::printTokens(const vector <Token> &tokens)
//{
//	for (const Token &element : tokens)
//	{
//		element.printToken();
//	}
//}
//
//void LexicalAnalysator::getTokensFromString(const string &arg)
//{
//	size_t stringPos = 0;
//	Token currentToken;
//	Token nonErrorToken;
//	vector <Token> tokens;
//	nonErrorToken.setType(TokenType::ERROR);
//	if (arg.empty())
//	{
//		return;
//	}
//	string result;
//	for (size_t i = 0; i < arg.length(); ++i)
//	{
//		result = "";
//		nonErrorToken.setType(TokenType::ERROR);
//		for (size_t j = i; j < arg.length(); ++j)
//		{
//			result += arg[j];
//			currentToken.tokenFromString(result);
//			if (currentToken.getType() == "ERROR" && j == i)
//			{
//				break;
//			}
//			if (currentToken.getType() != "ERROR")
//			{
//				nonErrorToken = currentToken;
//			}
//		}
//		if (nonErrorToken.getType() == "ERROR")
//		{
//			nonErrorToken.setValue(result);
//		}
//		i += nonErrorToken.getValue().length() - 1;
//		tokens.push_back(nonErrorToken);
//		currentToken.clear();
//		nonErrorToken.clear();
//	}
//	handleIncompatibleTypes(tokens);
//	printTokens(tokens);
//}
