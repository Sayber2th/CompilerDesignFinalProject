#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <cstdlib>
#include <string>

//improve the formatting for this function later
void Parser::raiseErrorSyntax(TokenType tokenType, std::string tokenValue)
{
	std::cerr << "[!] SYNTAX ERROR " << typeToString(tokenType) << " " << tokenValue << std::endl;
}

Token* Parser::proceed(enum TokenType TYPE)
{
	//std::cout << "called proceed" << std::endl; //debug
	if (current->TYPE != TYPE)
	{
		raiseErrorSyntax(current->TYPE, current->VALUE);
		exit(1);
	}
	else
	{
		index ++;
		current = parserTokens.at(index);
		return current;
	}
}

AST_Node* Parser::parseIdentifier()
{
	//std::cout << "called parseidentifier" << std::endl; //debug
	std::string* buffer = &current->VALUE;
	proceed(TokenType::TOKEN_IDENTIFIER);
	proceed(TokenType::TOKEN_EQUALS);

	AST_Node* newNode = new AST_Node;
	newNode->TYPE = NodeType::NODE_VARIABLE;
	newNode->CHILD = parseInteger();

	return newNode;
}

AST_Node* Parser::parseKeyword()
{
	//std::cout << "called parsekeyword" << std::endl; //debug
	if (current->VALUE == "int")
	{
		return parseKeywordInt();
	}
	else if (current->VALUE == "return")
	{
		return parseKeywordReturn();
	}
	else if (current->VALUE == "print")
	{
		return parseKeywordPrint();
	}
	else
	{
		raiseErrorSyntax(current->TYPE, current->VALUE);
		exit(1);
	}
}

AST_Node* Parser::parseInteger()
{
	//std::cout << "called parseinteger" << std::endl; //debug
	if (current->TYPE != TokenType::TOKEN_INT)
	{
		raiseErrorSyntax(current->TYPE, current->VALUE);
		exit(1);
	}

	AST_Node* newNode = new AST_Node;
	newNode->TYPE = NodeType::NODE_INT;
	newNode->VALUE = &current->VALUE;
	proceed(TokenType::TOKEN_INT);

	return newNode;
}

AST_Node* Parser::parseKeywordInt()
{
	//std::cout << "called parsekeywordint" << std::endl; //debug
	proceed(TokenType::TOKEN_KEYWORD);

	AST_Node* newNode = new AST_Node;
	newNode->TYPE = NodeType::NODE_KEYWORD_INT;
	newNode->CHILD = parseIdentifier();

	return newNode;
}

AST_Node* Parser::parseKeywordReturn()
{
	//std::cout << "called parsekeywordreturn" << std::endl; //debug
	proceed(TokenType::TOKEN_KEYWORD);

	AST_Node* newNode = new AST_Node;
	newNode->TYPE = NodeType::NODE_RETURN;
	newNode->CHILD = parseInteger();

	return newNode;
}

AST_Node* Parser::parseKeywordPrint()
{
	//std::cout << "called parsekeywordprint" << std::endl; //debug
	proceed(TokenType::TOKEN_KEYWORD);

	AST_Node* newNode = new AST_Node;
	newNode->TYPE = NodeType::NODE_PRINT;
	proceed(TokenType::TOKEN_LEFT_PAREN);
	newNode->CHILD = parseInteger();
	proceed(TokenType::TOKEN_RIGHT_PAREN);

	return newNode;
}

AST_Node* Parser::parse()
{
	//std::cout << "parse called" << std::endl; //debug
	AST_Node* ROOT = new AST_Node();
	ROOT->TYPE = NodeType::NODE_ROOT;

	while (current->TYPE != TokenType::TOKEN_EOF)
	{
		//std::cout << "inside while loop of parse" << std::endl; //debug
		switch (current->TYPE)
		{
			case TokenType::TOKEN_KEYWORD:
			{
				//std::cout << "case keyword token" << std::endl; //debug
				ROOT->SUB_STATEMENTS.push_back(parseKeyword());
				break;
			}
			case TokenType::TOKEN_IDENTIFIER :
			{
				//std::cout << "case keyword identifier" << std::endl; //debug
				ROOT->SUB_STATEMENTS.push_back(parseIdentifier());
				break;
			}
			default :
			{
				raiseErrorSyntax(current->TYPE, current->VALUE);
				exit(1);
			}
		}

		proceed(TokenType::TOKEN_SEMICOLON);
	}

	return ROOT;
}