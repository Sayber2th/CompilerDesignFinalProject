#include "driver.h"
#include "lexer.h"
#include "parser.h"
#include "semanticAnalyzer.h"
#include "tacGenerator.h"
#include "tacInterpreter.h"
#include "tacOptimizer.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <windows.h>

int main(const int argc, char* argv[])
{
	SetConsoleOutputCP(CP_UTF8);
	const std:: string source_code = try_read_file(argc, argv);
	std::cout << "\nContent of source file:" << '\n' << source_code << '\n'; //debug

	/*
	 *Lexical analysis
	 */
	lexer lexer(source_code);
	const std::vector <token *> tokens = lexer.tokenize();
	lexer::print_scanned_tokens(tokens); //Debug

	/*
	 *Syntax Analysis
	 */
	parser parser(tokens);
	const ast_node* program = parser.parse_program();
	parser::print_abstract_syntax_tree(program); //Debug
	
	/*
	 *Semantic Analysis
	 */
	semantic_analyzer semantic_analyzer(program);
	semantic_analyzer.analyse();
	semantic_analyzer.print_symbol_table(); //Debug
	
	/*
	 *Three Address Code Generation
	 */
	tac_generator tac(program);
	tac.generate();
	tac.print_raw_tac();
	
	/*
	 *Three Address Code Optimisation
	 */
	tac_optimizer optimizer(tac.get_code());
	optimizer.optimize();
	optimizer.print_optimised_tac();
	
	/*
	 *Three Address Code Execution
	 */
	tac_interpreter interpreter(optimizer.get_optimized_code());
	interpreter.run();
	
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

std::string try_read_file(const int argc, char* argv[])
{
	if (argc < 2) 
	{
		std::cerr << "\nPlease provide the source file using the command: Project1.exe <source-file-path>";
		exit(1);
	}
	
	if (argc > 2) 
	{
		std::cerr << "\nThis program only takes the source code text file as a parameter."
		<< "\nPlease provide the correct parameter using the command: Project1.exe <source-file-path>";
		exit(1);
	}
	
	std::string source_code = read_file(argv[1]);
	source_code.append("\0");
	
	return source_code;
}
