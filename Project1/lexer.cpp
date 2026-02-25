#include "lexer.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <algorithm>

char lexer::peek(const int offset) const
{
	if (cursor_ + offset >= source_.size()) 
	{
		return 0;
	}
	else
	{
		return source_[cursor_ + offset];
	}
}

char lexer::advance()
{
	if (cursor_ < size_)
	{
		const char temp = current_;
		cursor_ ++;
		character_number_ ++;
		current_ = (cursor_ < size_) ? source_[cursor_] : '\0';
		return temp;
	}
	else
	{
		return '\0';
	}
}

void lexer::check_and_skip_whitespace()
{
	while (current_ == ' ' || current_ == '\t' || current_ == '\r')
	{
		advance();
	}
}

void lexer::check_and_skip_newline()
{
	while (current_ == '\n')
	{
		line_number_ ++;
		character_number_ = 0;
		advance();
	}
}

void lexer::print_scanned_tokens(const std::vector<token*>& tokens)
{
	std::cout << "\nNumber of tokens: " << tokens.size() << '\n';
	std::cout << "\nList of tokens:" << '\n';
	int counter_tokens = 0;
	for (const token* temp : tokens)
	{
		counter_tokens ++;
		std::cout << counter_tokens << ")" << "Token: " << temp->value << " | " 
		<< "Type: " << token_type_to_debug_string(temp->type) << '\n';
	}
}

void lexer::raise_error_unidentified_symbol() const
{
	std::cout << "[!] TOKENIZATION ERROR : Unidentified symbol: " << current_ << " " << "at" << " ";
	std::cout << "Line: " << line_number_ << " " << "Character: " << character_number_ << '\n';
}

std::string token_type_to_string(const enum token_type type)
{
	switch (type)
	{
	case token_identifier: return "token_identifier";
	case token_int: return "token_int";
	case token_quotes_double: return "token_quotes_double";	
	case token_equals: return "token_equals";
	case token_keyword: return "token_keyword";
	case token_semicolon: return "token_semicolon";
	case token_left_paren: return "token_left_paren";
	case token_right_paren: return "token_right_paren";
	case token_left_curly: return "token_left_curly";
	case token_right_curly: return "token_right_curly";
	case token_comma: return "token_comma";
	case token_string: return "token_string";	
	case token_rel_equals: return "token_rel_equals";
	case token_rel_notequals: return "token_rel_notequals";
	case token_rel_lessthan: return "token_rel_lessthan";
	case token_rel_lessthanequals: return "token_rel_lessthanequals";
	case token_rel_greaterthan: return "token_rel_greaterthan";
	case token_rel_greaterthanequals: return "token_rel_greaterthanequals";
	case token_plus: return "token_plus";
	case token_minus: return "token_minus";
	case token_star: return "token_star";
	case token_slash: return "token_slash";
	case token_if: return "token_if";
	case token_else: return "token_else";
	case token_eof: return "token_eof";
	default: return "Unrecognised token";  // NOLINT(clang-diagnostic-covered-switch-default)
	}
}

std::string token_type_to_debug_string(enum token_type type)
{
	switch (type)
	{
	case token_identifier: return "identifier";
	case token_int: return "integer literal";
	case token_quotes_double: return "double quotes";	
	case token_equals: return "equals";
	case token_keyword: return "keyword";
	case token_semicolon: return "semicolon";
	case token_left_paren: return "left parenthesis";
	case token_right_paren: return "right parenthesis";
	case token_left_curly: return "left curly bracket";
	case token_right_curly: return "right curly bracket";
	case token_comma: return "comma";
	case token_string: return "string literal";	
	case token_rel_equals: return "relational equals to";
	case token_rel_notequals: return "relational not equals to";
	case token_rel_lessthan: return "relational less than";
	case token_rel_lessthanequals: return "relational less than equals to";
	case token_rel_greaterthan: return "relational greater than";
	case token_rel_greaterthanequals: return "relational greater than equals to";
	case token_plus: return "plus";
	case token_minus: return "minus";
	case token_star: return "asterisk";
	case token_slash: return "back slash";
	case token_if: return "if keyword";
	case token_else: return "else keyword";
	case token_eof: return "end of file token";
	default: return "Unrecognised token";  // NOLINT(clang-diagnostic-covered-switch-default)
	}
}

token* lexer::tokenize_keyword_or_identifier()
{
	const auto new_token = new token();
	new_token->line_number = line_number_;
	new_token->character_number = character_number_;
	std::stringstream buffer;
	buffer << advance();

	while (isalnum(current_) || current_ == '_')
	{
		buffer << advance();
	}
	
	new_token->type = (
			std::find(keywords.begin(), keywords.end(), buffer.str()) != keywords.end()  // NOLINT(modernize-use-ranges)
		) ? token_keyword : token_identifier;
	new_token->value = buffer.str();
	return new_token;
}

token* lexer::tokenize_integer()
{
	const auto new_token = new token();
	new_token->line_number = line_number_;
	new_token->character_number = character_number_;
	std::stringstream buffer;
	buffer << advance();

	while (isdigit(current_))
	{
		buffer << advance();
	}
	
	new_token->type = token_int;
	new_token->value = buffer.str();
	return new_token;
}

token* lexer::tokenize_special(const token_type type)
{
	const auto new_token = new token();
	new_token->line_number = line_number_;
	new_token->character_number = character_number_;
	
	std::stringstream buffer;
	const char first = advance();     // consume the current character
	buffer << first;

	// handle double-character relational operators
	if ((first == '=' || first == '!' || first == '<' || first == '>') && current_ == '=')
	{
		buffer << advance();    // consume '='
	}

	new_token->type = type;
	new_token->value = buffer.str();

	return new_token;
}

token* lexer::tokenize_string()
{
	const auto new_token = new token();
	new_token->line_number = line_number_;
	new_token->character_number = character_number_;
	std::stringstream buffer;
	
	while (current_ != '"')
	{
		buffer << advance();
	}
	
	new_token->type = token_string;
	new_token->value = buffer.str();
	return new_token;
}

std::vector<token *> lexer::tokenize()
{
	std::vector<token *> tokens;
	bool is_eof = false;

	while (!is_eof && cursor_ < size_)
	{
		check_and_skip_whitespace();

		//if token is keyword or identifier
		if (isalpha(current_) || current_ == '_')
		{
			tokens.push_back(tokenize_keyword_or_identifier());
			continue;
		}
		
		//if token is integer
		if (isdigit(current_))
		{
			tokens.push_back(tokenize_integer());
			continue;
		}

		switch (current_)
		{
			case '\n':
			{
				check_and_skip_newline();
				break;
			}
			case ';' :
			{
				tokens.push_back(tokenize_special(token_semicolon));
				break;
			}
			case '=':
			{
				if (peek(1) == '=')
				{
					tokens.push_back(tokenize_special(token_rel_equals));
				}
				else
				{
					tokens.push_back(tokenize_special(token_equals));
				}
				break;
			}
			case '!':
			{
				if (peek(1) == '=')
				{
					tokens.push_back(tokenize_special(token_rel_notequals));
				}
				else
				{
					raise_error_unidentified_symbol();
					exit(1);
				}
				break;
			}
			case '<':
			{
				if (peek(1) == '=')
				{
					tokens.push_back(tokenize_special(token_rel_lessthanequals));
				}
				else
				{
					tokens.push_back(tokenize_special(token_rel_lessthan));
				}
				break;
			}
			case '>':
			{
				if (peek(1) == '=')
				{
					tokens.push_back(tokenize_special(token_rel_greaterthanequals));
				}
				else
				{
					tokens.push_back(tokenize_special(token_rel_greaterthan));
				}
				break;
			}
			case '(':
			{
				tokens.push_back(tokenize_special(token_left_paren));
				break;
			}
			case ')':
			{
				tokens.push_back(tokenize_special(token_right_paren));
				break;
			}
			case '{':
			{
				tokens.push_back(tokenize_special(token_left_curly));
				break;
			}
			case '}':
			{
				tokens.push_back(tokenize_special(token_right_curly));
				break;
			}
			case '"':
			{
				tokens.push_back(tokenize_special(token_quotes_double));
				tokens.push_back(tokenize_string());
				tokens.push_back(tokenize_special(token_quotes_double));
				break;
			}
			case '+':
			{
				tokens.push_back(tokenize_special(token_plus));
				break;
			}
			case '-':
			{
				tokens.push_back(tokenize_special(token_minus));
				break;
			}
			case '*':
			{
				tokens.push_back(tokenize_special(token_star));
				break;
			}
			case '/':
			{
				tokens.push_back(tokenize_special(token_slash));
				break;
			}
			case 0:
			{
				tokens.push_back(tokenize_special(token_eof));
				is_eof = true;
				break;
			}
			default :
			{
				raise_error_unidentified_symbol();
				exit(1);
			}
		}
	}
	
	if (tokens.back()->type != token_eof)
	{
		const auto eof_token = new token;
		eof_token->type = token_eof;
		tokens.push_back(eof_token);
	}

	return tokens;
}