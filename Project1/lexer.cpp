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

void lexer::raise_error_unidentified_symbol() const
{
	std::cout << "[!] LEXER ERROR : Unidentified symbol: " << current_ << " " << "at" << " ";
	std::cout << "Line: " << line_number_ << " " << "Character: " << character_number_ << '\n';
}

std::string type_to_string(const enum token_type type)
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

token* lexer::tokenize_keyword_or_identifier()
{
	const auto new_token = new token();
	std::stringstream buffer;
	buffer << advance();

	while (isalpha(current_) || current_ == '_')
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
	new_token->type = type;
	
	const bool is_double_char_special = (
		std::find(double_character_specials.begin(), double_character_specials.end(), peek(-1)  // NOLINT(modernize-use-ranges)
		) != double_character_specials.end());
	
	
	if (peek(-1) != ' ' && is_double_char_special && current_ == '=')
	{
		//('==', '!=', '<=', '>=')
		std::stringstream buffer;
		buffer << peek(-1);
		buffer << advance();

		new_token->value = buffer.str();
	}
	else
	{
		new_token->value = std::string(1, advance());
	}

	return new_token;
}

token* lexer::tokenize_string()
{
	const auto new_token = new token();
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
					advance();
					tokens.push_back(tokenize_special(token_rel_equals));
					break;
				}
				else
				{
					tokens.push_back(tokenize_special(token_equals));
					break;
				}
			}
			case '!':
			{
				if (peek(1) == '=')
				{
					advance();
					tokens.push_back(tokenize_special(token_rel_notequals));
					break;
				}
				else
				{
					advance();
					raise_error_unidentified_symbol();
					exit(1);
				}
			}
			case '<':
			{
				if (peek(1) == '=')
				{
					advance();
					tokens.push_back(tokenize_special(token_rel_lessthanequals));
					break;
				}
				else
				{
					tokens.push_back(tokenize_special(token_rel_lessthan));
					break;
				}
			}
			case '>':
			{
				if (peek(1) == '=')
				{
					advance();
					tokens.push_back(tokenize_special(token_rel_greaterthanequals));
					break;
				}
				else
				{
					tokens.push_back(tokenize_special(token_rel_greaterthan));
					break;
				}
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

	return tokens;
}