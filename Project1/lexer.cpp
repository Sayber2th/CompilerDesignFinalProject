#include "lexer.h"

LexicalAnalyser::LexicalAnalyser(const std::string& src)
	: source_code(src)
	, position(0)
{
	std::cout << "Lexer received source of size: "
		<< source_code.size() << std::endl;
}
