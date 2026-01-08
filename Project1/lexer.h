#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <cstddef>
#include <tuple>
#include <vector>
#include <cctype>

enum TokenType 
{ 
	IDENTIFIER, 
	INTEGER_LITERAL, 
	SEMICOLON, COMMA, 
	PLUS, MINUS, STAR, SLASH,
	UNKNOWN
};

struct Token
{
	TokenType type;
	std::string lexeme;
	std::tuple<int, int> location;

	Token(TokenType t, const std::string& le, const std::tuple<int, int>& lo)
		: type(t)
		, lexeme(le)
		, location(lo)
	{
	}
};

class LexicalAnalyser
{
private:
	std::string source_code;
	size_t position;

	bool isAtEnd();
	char peekCurrent();
	char peekNext();
	void advancePosition();
	void updateCharLocation(bool isNewline);

	bool isWhitespace(char currentChar);
	bool isNewLine(char currentChar);
	bool isAlphabetical(char currentChar);

	std::vector<Token> tokenize();

public:
	LexicalAnalyser(const std::string& src);

	int lineNumber = 1;
	int columnNumber = 1;

};