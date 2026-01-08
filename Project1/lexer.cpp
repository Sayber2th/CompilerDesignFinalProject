#include "lexer.h"

LexicalAnalyser::LexicalAnalyser(const std::string& src)
	: source_code(src)
	, position(0)
{
	
}


bool LexicalAnalyser::isAtEnd()
{
	return position >= source_code.size();
}

char LexicalAnalyser::peekCurrent()
{
	if (position >= source_code.size()) return '\0';

	return source_code[position];
}

char LexicalAnalyser::peekNext()
{
	if (position >= source_code.size()) return '\0';
	if (position + 1 >= source_code.size()) return '\0';
	
	return source_code[position + 1];
}

void LexicalAnalyser::advancePosition()
{
	if (isNewLine(peekCurrent()))
	{
		updateCharLocation(true);
	}
	else
	{
		updateCharLocation(false);
	}
	position++;
}

void LexicalAnalyser::updateCharLocation(bool isNewline)
{
	if (isNewline)
	{
		lineNumber += 1;
		columnNumber = 1;
	}
	else
	{
		columnNumber += 1;
	}
}

bool LexicalAnalyser::isWhitespace(char currentChar)
{
	if (currentChar == ' ' || currentChar == '\t' || currentChar == '\r' || currentChar == '\n')
	{
		return true;
	}

	return false;
}

bool LexicalAnalyser::isNewLine(char currentChar)
{
	if (currentChar == '\n')
	{
		return true;
	}

	return false;
}

bool LexicalAnalyser::isAlphabetical(char currentChar)
{
	if ((currentChar >= 'A' && currentChar <= 'Z') || (currentChar >= 'a' && currentChar <= 'z'))
	{
		return true;
	}

	return false;
}

std::vector<Token> LexicalAnalyser::tokenize()
{
	std::vector<Token> tokens;

	while (!isAtEnd())
	{
		char currentChar = peekCurrent();

		if (isWhitespace(currentChar))
		{
			advancePosition();
			continue;
		}
		else
		{
			//....//
		}
	}
}