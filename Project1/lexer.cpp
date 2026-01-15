#include "lexer.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <algorithm>

char Lexer::peek(int offset)
{
	if (cursor + offset >= source.size()) 
	{
		return 0;
	}
	else
	{
		return source[cursor + offset];
	}
}

char Lexer::advance()
{
	if (cursor < size)
	{
		char temp = current;
		cursor ++;
		characterNumber ++;
		current = (cursor < size) ? source[cursor] : '\0';
		return temp;
	}
	else
	{
		return '\0';
	}
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

void Lexer::errorUnidentifiedSymbol() const
{
	std::cout << "[!] PARSER ERROR : Unidentified symbol: " << current << " " << "at" << " ";
	std::cout << "Line: " << lineNumber << " " << "Character: " << characterNumber << std::endl;
}

std::string typeToString(enum TokenType type)
{
	switch (type)
	{
	case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
	case TOKEN_INT: return "TOKEN_INT";
	case TOKEN_EQUALS: return "TOKEN_EQUALS";
	case TOKEN_KEYWORD: return "TOKEN_KEYWORD";
	case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
	case TOKEN_LEFT_PAREN: return "TOKEN_LEFT_PAREN";
	case TOKEN_RIGHT_PAREN: return "TOKEN_RIGHT_PAREN";
	case TOKEN_LEFT_CURLY: return "TOKEN_LEFT_CURLY";
	case TOKEN_RIGHT_CURLY: return "TOKEN_RIGHT_CURLY";
	case TOKEN_COMMA: return "TOKEN_COMMA";
	case TOKEN_REL_EQUALS: return "TOKEN_REL_EQUALS";
	case TOKEN_REL_NOTEQUALS: return "TOKEN_REL_NOTEQUALS";
	case TOKEN_REL_LESSTHAN: return "TOKEN_REL_LESSTHAN";
	case TOKEN_REL_LESSTHANEQUALS: return "TOKEN_REL_LESSTHANEQUALS";
	case TOKEN_REL_GREATERTHAN: return "TOKEN_REL_GREATERTHAN";
	case TOKEN_REL_GREATERTHANEQUALS: return "TOKEN_REL_GREATERTHANEQUALS";
	case TOKEN_PLUS: return "TOKEN_PLUS";
	case TOKEN_MINUS: return "TOKEN_MINUS";
	case TOKEN_STAR: return "TOKEN_STAR";
	case TOKEN_SLASH: return "TOKEN_SLASH";
	case TOKEN_IF: return "TOKEN_IF";
	case TOKEN_ELSE: return "TOKEN_ELSE";
	case TOKEN_EOF: return "TOKEN_EOF";
	default: return "UNRECOGNIZED TOKEN";
	}
}

Token* Lexer::tokenizeKeywordOrIdentifier()
{
	std::stringstream buffer;
	buffer << advance();

	while (isalpha(current) || current == '_')
	{
		buffer << advance();
	}

	Token* newToken = new Token();

	newToken->value = buffer.str();
	newToken->type = (
		std::find(keywords.begin(), keywords.end(), newToken->value) != keywords.end()
		) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;

	return newToken;
}

Token* Lexer::tokenizeInteger()
{
	std::stringstream buffer;
	buffer << advance();

	while (isdigit(current))
	{
		buffer << advance();
	}

	Token* newToken = new Token();

	newToken->type = TokenType::TOKEN_INT;
	newToken->value = buffer.str();

	return newToken;
}

Token* Lexer::tokenizeSpecial(TokenType type)
{
	Token* newToken = new Token;
	newToken->type = type;
	bool isDoubleCharSpecial = (
		std::find(doubleCharacterSpecials.begin(), doubleCharacterSpecials.end(), peek(-1)
		) != doubleCharacterSpecials.end());
	
	if (peek(-1) != ' ' && isDoubleCharSpecial)
	{
		std::stringstream buffer;
		buffer << peek(-1);
		buffer << advance();

		newToken->value = buffer.str();
	}
	else
	{
		newToken->value = std::string(1, advance());
	}

	return newToken;
}

std::vector<Token *> Lexer::tokenize()
{
	std::vector<Token *> tokens;
	bool notEOF = true;
	bool doDebugCheck = true;

	while (cursor < size && notEOF)
	{
		checkAndSkipWhitespace();
		//if token is keyword or identifier
		if (isalpha(current) || current == '_')
		{
			tokens.push_back(tokenizeKeywordOrIdentifier());
			continue;
		}
		
		//if token is integer
		if (isdigit(current))
		{
			tokens.push_back(tokenizeInteger());
			continue;
		}

		switch (current)
		{
			case '\n':
			{
				checkAndSkipNewline();
				break;
			}
			case ';' :
			{
				tokens.push_back(tokenizeSpecial(TOKEN_SEMICOLON));
				break;
			}
			case '=':
			{
				if (peek(1) == '=')
				{
					advance();
					tokens.push_back(tokenizeSpecial(TOKEN_REL_EQUALS));
					break;
				}
				else
				{
					tokens.push_back(tokenizeSpecial(TOKEN_EQUALS));
					break;
				}
			}
			case '!':
			{
				if (peek(1) == '=')
				{
					advance();
					tokens.push_back(tokenizeSpecial(TOKEN_REL_NOTEQUALS));
					break;
				}
				else
				{
					advance();
					errorUnidentifiedSymbol();
					exit(1);
				}
			}
			case '<':
			{
				if (peek(1) == '=')
				{
					advance();
					tokens.push_back(tokenizeSpecial(TOKEN_REL_LESSTHANEQUALS));
					break;
				}
				else
				{
					tokens.push_back(tokenizeSpecial(TOKEN_REL_LESSTHAN));
					break;
				}
			}
			case '>':
			{
				if (peek(1) == '=')
				{
					advance();
					tokens.push_back(tokenizeSpecial(TOKEN_REL_GREATERTHANEQUALS));
					break;
				}
				else
				{
					tokens.push_back(tokenizeSpecial(TOKEN_REL_GREATERTHAN));
					break;
				}
			}
			case '(':
			{
				tokens.push_back(tokenizeSpecial(TOKEN_LEFT_PAREN));
				break;
			}
			case ')':
			{
				tokens.push_back(tokenizeSpecial(TOKEN_RIGHT_PAREN));
				break;
			}
			case '{':
			{
				tokens.push_back(tokenizeSpecial(TOKEN_LEFT_CURLY));
				break;
			}
			case '}':
			{
				tokens.push_back(tokenizeSpecial(TOKEN_RIGHT_CURLY));
				break;
			}
			default :
			{
				errorUnidentifiedSymbol();
				exit(1);
			}
		}
	}

	return tokens;
}