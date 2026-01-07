#pragma once
#include <string>
#include <fstream>
#include <iostream>

enum TokenType 
{ 
	Identifier, 
	Integer, 
	Semicolon, Comma, 
	Plus, Minus, Star, ForwardSlash 
};

std::string readFile(std::string filename);