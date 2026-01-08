#include "lexer.h"

LexicalAnalyser::LexicalAnalyser(const std::string& src)
	: source_code(src)
	, position(0)
{
	//Debug code; remove later
	std::cout << "Lexer received source of size: "
		<< source_code.size() << std::endl;
}


bool LexicalAnalyser::isAtEnd()
{
	return isPositionAtEnd(position);
}

bool LexicalAnalyser::isPositionAtEnd(size_t position)
{
	return position >= source_code.size();
}

char LexicalAnalyser::peekCurrent()
{
	if (!isAtEnd())
	{
		return source_code[position];
	}

	//If current position is the end
	return '\0';
}

char LexicalAnalyser::peekNext()
{
	if (!isPositionAtEnd(position+1))
	{
		return source_code[position + 1];
	}

	//If next position is the end
	return '\0';
}

void LexicalAnalyser::advancePosition()
{
	position++;
}

bool LexicalAnalyser::isWhitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '//')
	{
		return true;
	}

	return false;
}