#include "lexer.h"
#include <string>
#include <vector>
#include <iostream>

bool Lexer::isAtEnd()
{
	return cursor >= source.size();
}

char Lexer::peek(int offset)
{
	if (cursor + offset >= source.size()) 
	{
		return 0;
	}
	else
	{
		return source[cursor];
	}
}

char Lexer::advance()
{
	if (cursor < size)
	{
		char temp = current;
		cursor ++;
		characterNumber ++;
	}

	return '\0';
}

void Lexer::checkAndSkipWhitespace()
{
	while (current == ' ' || current == '\t' || current == '\r')
	{
		advance();
	}
}

void Lexer::checkAndSkipNewline()
{
	while (current == '\n')
	{
		lineNumber ++;
		characterNumber = 0;
		advance();
	}
}

std::vector<Token> Lexer::tokenize()
{
	std::cout << "Reached inside tokenize" << std::endl; //debug
	std::vector<Token> tokens;
	bool notEOF = true;
	bool newLine = true;

	while (!isAtEnd())
	{
		char current = peek(0);

		checkAndSkipWhitespace();
		
		/*
		.. implementation here .. 
		*/
	}

	return tokens;
}