#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

#include "driver.h"
#include "lexer.h"
#include "parser.h"
#include "semanticAnalyzer.h"

int main(const int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "\nPlease provide the source file using the command: Project1.exe <source-file-path>";
		exit(1);
	}
	
	std::string source_code = read_file(argv[1]);

	source_code.append("\0");
	std::cout << "\nContent of source file:" << '\n' << source_code << '\n'; //debug

	//Lexical analysis
	lexer lexer(source_code);
	std::vector <token *> tokens = lexer.tokenize();

	if (tokens.back()->type != token_eof)
	{
		const auto eof_token = new token;
		eof_token->type = token_eof;
		tokens.push_back(eof_token);
	}

	/*
	 *Debug
	 */
	// std::cout << "\nList of tokens:" << '\n';
	// int counter_tokens = 0;
	// for (const token* temp : tokens)
	// {
	// 	counter_tokens ++;
	// 	std::cout << counter_tokens << ")" << "Type: " << token_type_to_string(temp->type) << " " << "Value: " << temp->value << '\n';
	// }

	//Syntax Analysis
	parser parser(tokens);
	const ast_node* root = parser.parse();
	
	/*
	 *Debug
	 */
	std::cout << "\nNumber of statements: " << root->sub_statements.size() << '\n';
	int counter_nodes = 0;
	for (const ast_node* node : root->sub_statements)
	{
		counter_nodes ++;
		std::cout << counter_nodes << ")" << "Statement Type: " << expression_type_to_string(node->expression_type) << " | " 
		<< "Node Type: " << node_type_to_string(node->type) << " | " 
		<< "Child: " << node_type_to_string(node->child->type);
		if (node->child->value != nullptr)
		{
			std::cout << " | " << "Value of child: " << *node->child->value;
		}
		std::cout << '\n';
	}
	
	//Semantic Analysis
	semantic_analyzer semantic_analyzer(root);
	semantic_analyzer.analyse();
	
	return 0;
}

std::string read_file(const char *file_name)
{
	std::ifstream source_file_stream(file_name);
	std::stringstream buffer;
	char temp;

	if (!source_file_stream)
	{
		std::cout << "\nNo such file found; please ensure provide a valid file path." << '\n';
		exit(1);
	}

	while (source_file_stream.get(temp))
	{
		buffer << temp;
	}

	return buffer.str();
}