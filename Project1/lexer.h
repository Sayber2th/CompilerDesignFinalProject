#ifndef __LEXER_H
#define __LEXER_H

#include <string>
#include <vector>

enum TokenType
{
	TOKEN_IDENTIFIER,
	TOKEN_INT,
	TOKEN_EQUALS,
	TOKEN_KEYWORD,
	TOKEN_SEMICOLON,
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_CURLY,
	TOKEN_RIGHT_CURLY,
	TOKEN_COMMA,
	TOKEN_REL_EQUALS,
	TOKEN_REL_NOTEQUALS,
	TOKEN_REL_LESSTHAN,
	TOKEN_REL_LESSTHANEQUALS,
	TOKEN_REL_GREATERTHAN,
	TOKEN_REL_GREATERTHANEQUALS,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_SLASH,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_EOF
};

struct Token
{
	TokenType TYPE;
	std::string VALUE;
};

std::string typeToString(enum TokenType type);

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

	char peek(int offset);
	char advance();
	void checkAndSkipWhitespace();
	void checkAndSkipNewline();
	void raiseErrorUnidentifiedSymbol() const;

	std::vector<std::string> keywords = {"int", "if", "else", "return", "true", "false"};
	std::vector<char> doubleCharacterSpecials = {'!', '=', '<', '>'};

	std::vector<Token *> tokenize();
	Token* tokenizeKeywordOrIdentifier();
	Token* tokenizeInteger();
	Token* tokenizeSpecial(TokenType type);

private:
	std::string source;
	size_t cursor;
	size_t size;
	char current;
	int lineNumber;
	int characterNumber;
};

#endif //__LEXER_H