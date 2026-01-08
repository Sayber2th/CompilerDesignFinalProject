#include "driver.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: Project1.exe <source-file-path>";
		return 1;
	}
	std::string file_path = argv[1];
	std::string file_contents = readFile(file_path);

	return 0;
}

std::string readFile(std::string file_name)
{
	std::ifstream file(file_name);

	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << file_name << "\n";
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