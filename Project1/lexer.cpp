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
	case token_identifier: return "TOKEN_IDENTIFIER";
	case token_int: return "TOKEN_INT";
	case token_equals: return "TOKEN_EQUALS";
	case token_keyword: return "TOKEN_KEYWORD";
	case token_semicolon: return "TOKEN_SEMICOLON";
	case token_left_paren: return "TOKEN_LEFT_PAREN";
	case token_right_paren: return "TOKEN_RIGHT_PAREN";
	case token_left_curly: return "TOKEN_LEFT_CURLY";
	case token_right_curly: return "TOKEN_RIGHT_CURLY";
	case token_comma: return "TOKEN_COMMA";
	case token_rel_equals: return "TOKEN_REL_EQUALS";
	case token_rel_notequals: return "TOKEN_REL_NOTEQUALS";
	case token_rel_lessthan: return "TOKEN_REL_LESSTHAN";
	case token_rel_lessthanequals: return "TOKEN_REL_LESSTHANEQUALS";
	case token_rel_greaterthan: return "TOKEN_REL_GREATERTHAN";
	case token_rel_greaterthanequals: return "TOKEN_REL_GREATERTHANEQUALS";
	case token_plus: return "TOKEN_PLUS";
	case token_minus: return "TOKEN_MINUS";
	case token_star: return "TOKEN_STAR";
	case token_slash: return "TOKEN_SLASH";
	case token_if: return "TOKEN_IF";
	case token_else: return "TOKEN_ELSE";
	case token_eof: return "TOKEN_EOF";
	default: return "UNRECOGNIZED TOKEN";  // NOLINT(clang-diagnostic-covered-switch-default)
	}
}

token* lexer::tokenize_keyword_or_identifier()
{
	std::stringstream buffer;
	buffer << advance();

	while (isalpha(current_) || current_ == '_')
	{
		buffer << advance();
	}

	const auto new_token = new token();

	new_token->value = buffer.str();
	new_token->type = (
		std::ranges::find(keywords, new_token->value) != keywords.end()
		) ? token_keyword : token_identifier;

	return new_token;
}

token* lexer::tokenize_integer()
{
	std::stringstream buffer;
	buffer << advance();

	while (isdigit(current_))
	{
		buffer << advance();
	}

	const auto new_token = new token();

	new_token->type = token_int;
	new_token->value = buffer.str();

	return new_token;
}

token* lexer::tokenize_special(const token_type type)
{
	const auto new_token = new token;
	new_token->type = type;
	const bool is_double_char_special = (
		std::ranges::find(double_character_specials, peek(-1)
		) != double_character_specials.end());
	
	if (peek(-1) != ' ' && is_double_char_special)
	{
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