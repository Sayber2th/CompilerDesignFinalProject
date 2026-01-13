#ifndef __LEXER_H
#define __LEXER_H

#include <string>
#include <vector>

enum TokenType
{
	TOKEN_IDENTIFIER,
	TOKEN_INT,
	TOKEN_KEYWORD,
	TOKEN_SEMICOLON,
	TOKEN_COMMA,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_SLASH,
	TOKEN_IF,
	TOKEN_EOF
};

struct Token
{
	TokenType type;
	std::string lexeme;
};

class Lexer
{
public:
	Lexer(std::string sourceCode) :
		source(sourceCode),
		cursor(0),
		size(sourceCode.length()),
		current(sourceCode.at(cursor)),
		lineNumber(1),
		characterNumber(1)
	{
		
	};

	bool isAtEnd();
	char peek(int offset);
	char advance();

	//bool isWhitespace(char currentChar);
	void checkAndSkipWhitespace();
	void checkAndSkipNewline();

	std::vector<Token> tokenize();

private:
	std::string source;
	int cursor;
	size_t size;
	char current;
	int lineNumber;
	int characterNumber;
};

#endif //__LEXER_H