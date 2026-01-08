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

void LexicalAnalyser::advancePosition(bool isNewline)
{
	updateCharLocation(isNewline);
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

bool LexicalAnalyser::isSkippableWhitespace(char currentChar)
{
	if (currentChar == ' ' || currentChar == '\t' || currentChar == '\r' || currentChar == '//')
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

		if (isSkippableWhitespace(currentChar))
		{
			advancePosition(false);
			continue;
		}
		else if (isNewLine(currentChar))
		{
			advancePosition(true);
			continue;
		}
		else
		{
			//....//
		}
	}
}