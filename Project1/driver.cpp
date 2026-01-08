#include "driver.h"

int main(int argc, char* argv[])
{
	std::cout << "Compiler run!" << std::endl;
	std::cout << argv[0] << std::endl;
	std::cout << argv[1] << std::endl;

	if (argc < 2) {
		std::cerr << "Usage: mycompiler <source-file>\n";
		return 1;
	}
	std::string file_path = argv[1];

	std::string file_contents = readFile(file_path);
	std::cout << file_contents << std::endl;

	return 0;
}

std::string readFile(std::string file_name)
{
	std::ifstream file(file_name);

	if (!file.is_open()) {
		std::cerr << "Error: could not open file " << file_name << "\n";
		exit(1);
	}

	std::string str;
	std::string file_contents;
	while (std::getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}

	return file_contents;
}