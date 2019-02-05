#pragma once
#include <string>
enum TokenType {
	ID,
	REGULAR,
	INT,
	REAL,
	OPERATION,
	COMMENT,
	DELIMITER,
	DOT_DELIMITER,
	STRING,
	UNCLOSED_STRING,
	ERROR
};

class Token
{
public:
	size_t tokenFromString(std::string arg);
	void clear();
	void printToken() const;
	std::string getValue() const;
	std::string getType() const;
	void setValue(const std::string &argValue);
	void setType(TokenType argType);
	bool isId(const std::string &arg);
	bool isRegularWord(const std::string &arg);
	bool isOperation(const std::string &arg);
	bool isIntNumber(const std::string &arg);
	bool isNumber(const std::string &arg, const char &ch, std::vector<char> alphabet);
	bool isRealNumber(const std::string &arg);
	bool isDelimiter(const std::string &arg);
	std::string toLowerCase(const std::string &arg);
private:
	std::string value;
	TokenType type;
};

