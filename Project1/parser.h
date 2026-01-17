#ifndef __PARSER_H
#define __PARSER_H

#include "lexer.h"
#include <vector>
#include <string>

enum NodeType
{
	NODE_ROOT,
	NODE_RETURN,
	NODE_PRINT,
	NODE_INT,
	NODE_IDENTIFIER,
	NODE_VARIABLE,
	NODE_KEYWORD_INT
};

struct AST_Node
{
	enum NodeType TYPE;
	std::string* VALUE;
	AST_Node* CHILD;
	std::vector<AST_Node*> SUB_STATEMENTS;
};

class Parser
{
public:
	Parser(std::vector<Token*> tokens) :
		parserTokens(tokens),
		index(0),
		limit(parserTokens.size()),
		current(parserTokens.at(index))
	{
		/*parserTokens = tokens;
		index = 0;
		limit = parserTokens.size();
		current = parserTokens.at(index);*/
	}

	void raiseErrorSyntax(TokenType tokenType, std::string tokenValue); //improve the formatting for this later
	Token* proceed(enum TokenType TYPE);
	AST_Node* parseIdentifier();
	AST_Node* parseKeyword();
	AST_Node* parseInteger();
	AST_Node* parseKeywordInt();
	AST_Node* parseKeywordReturn();
	AST_Node* parseKeywordPrint();
	AST_Node* parse();

private:
	std::vector<Token*> parserTokens;
	size_t index;
	size_t limit;
	Token* current;
};

#endif // !__PARSER_H
