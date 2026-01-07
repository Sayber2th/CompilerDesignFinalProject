#include "lexer.h"

void main()
{
	std::string file_contents = readFile("test.txt");
	std::cout << file_contents << std::endl;
}

std::string readFile(std::string filename)
{
	std::ifstream file("test.txt");
	std::string str;
	std::string file_contents;
	while (std::getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}

	return file_contents;
}