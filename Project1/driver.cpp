#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

#include "driver.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "\nPlease provide the source file using the command: Project1.exe <source-file-path>";
		exit(1);
	}
	
	std::string sourceCode = readFile(argv[1]);

	sourceCode.append("\0");
	std::cout << "\nContent of source file:" << std::endl << sourceCode << std::endl; //debug

	Lexer lexer(sourceCode);
	std::vector <Token *> tokens = lexer.tokenize();

	if (tokens.back()->TYPE != TokenType::TOKEN_EOF)
	{
		Token* EOFToken = new Token;
		EOFToken->TYPE = TokenType::TOKEN_EOF;
		tokens.push_back(EOFToken);
	}

	std::cout << "\nList of tokens:" << std::endl;
	int counter = 0;
	for (Token* temp : tokens)
	{
		counter ++;
		std::cout << counter << ")" << "Type: " << typeToString(temp->TYPE) << " " << "Value: " << temp->VALUE << std::endl;
	}

	Parser parser(tokens);

	AST_Node* ROOT = parser.parse();
	std::cout << "\nNumber of statements: " << ROOT->SUB_STATEMENTS.size() << std::endl;

	return 0;
}

std::string readFile(char *file_name)
{
	std::ifstream sourceFileStream(file_name);
	std::stringstream buffer;
	char temp;

	if (!sourceFileStream)
	{
		std::cout << "\nNo such file found; please ensure provide a valid file path." << std::endl;
		exit(1);
	}

	while (sourceFileStream.get(temp))
	{
		buffer << temp;
	}

	return buffer.str();
}