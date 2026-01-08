#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <cstddef>

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
	std::string value;

	Token(TokenType t, const std::string& v)
		: type(t)
		, value(v)
	{
	}
};

class LexicalAnalyser
{
private:
	std::string source_code;
	size_t position;

	bool isAtEnd();
	bool isPositionAtEnd(size_t position);
	char peekCurrent();
	char peekNext();
	void advancePosition();

	bool isWhitespace(char c);

public:
	LexicalAnalyser(const std::string& src);

};