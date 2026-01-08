#pragma once
#include <string>
#include <fstream>
#include <iostream>

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

public:
	LexicalAnalyser(const std::string& src);

};