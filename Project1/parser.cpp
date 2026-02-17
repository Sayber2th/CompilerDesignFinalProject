#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <cstdlib>
#include <string>

std::string node_type_to_string(const enum node_type type)
{
	switch (type)
	{
	case node_root: return "Root node";
	case node_return: return "Keyword return";
	case node_print: return "Keyword print";
	case node_int: return "Integer";
	case node_string: return "String";
	case node_identifier: return "Identifier";
	case node_keyword_int: return "Keyword int";
	case node_keyword_string: return "Keyword string";
	default: return "Unrecognized node type";  // NOLINT(clang-diagnostic-covered-switch-default)
	}
}
std::string expression_type_to_string(const enum expression_type type)
{
	switch (type)
	{
	case expression_declaration_only : return "Declaration only";
	case expression_declaration_and_assigment : return "Declaration and assignment";
	case expression_assigment : return "Assignment";
	case expression_return : return "Return statement";
	case expression_print : return "Print statement";
	default: return "Unrecognized statement type";  // NOLINT(clang-diagnostic-covered-switch-default)
	}
}

//improve the formatting for this function later
void parser::raise_error_syntax(const token_type token_type, const std::string& token_value)
{
	std::cerr << "[!] SYNTAX ERROR " << token_type_to_string(token_type) << " " << token_value << '\n';
}

token* parser::proceed(const enum token_type type)
{
	if (current_->type != type)
	{
		std::cout << "proceed error" << '\n';
		raise_error_syntax(current_->type, current_->value);
		exit(1);
	}
	else
	{
		index_ ++;
		current_ = parser_tokens_.at(index_);
		return current_;
	}
}

ast_node* parser::parse_identifier(const bool is_statement_beginning)
{
	if (current_->type != token_identifier)
	{
		std::cout << "parse identifier error" << '\n';
		raise_error_syntax(current_->type, current_->value);
		exit(1);
	}
	
	const auto new_node = new ast_node;
	new_node->value = &current_->value;
	
	proceed(token_identifier);
	
	if (current_->type == token_equals)
	{
		proceed(token_equals);
	}
	else if (current_->type == token_right_paren)
	{
		proceed(token_right_paren);
	}
	else if (current_->type == token_semicolon)
	{
		//do nothing
	}
	else
	{
		std::cout << "parse identifier error" << '\n';
		raise_error_syntax(current_->type, current_->value);
		exit(1);
	}
	
	if (is_statement_beginning)
	{
		new_node->expression_type = expression_assigment;
	}
	new_node->type = node_identifier;
	
	if (current_->type == token_int)
	{
		new_node->child = parse_integer();
	}
	if (current_->type == token_quotes_double)
	{
		proceed(token_quotes_double);
		new_node->child = parse_string();
		proceed(token_quotes_double);
	}

	return new_node;
}

ast_node* parser::parse_keyword()
{
	if (current_->value == "int")
	{
		return parse_keyword_int();
	}
	if (current_->value == "string")
	{
		return parse_keyword_string();
	}
	else if (current_->value == "return")
	{
		return parse_keyword_return();
	}
	else if (current_->value == "print")
	{
		return parse_keyword_print();
	}
	else
	{
		std::cout << "parse keyword error" << '\n';
		raise_error_syntax(current_->type, current_->value);
		exit(1);
	}
}

/*
 *Parse data types
 */
ast_node* parser::parse_integer()
{
	if (current_->type != token_int)
	{
		std::cout << "parse integer error" << '\n';
		raise_error_syntax(current_->type, current_->value);
		exit(1);
	}

	const auto new_node = new ast_node;
	new_node->type = node_int;
	new_node->value = &current_->value;
	proceed(token_int);

	return new_node;
}

ast_node* parser::parse_string()
{
	if (current_->type != token_string)
	{
		std::cout << "parse string error" << '\n';
		raise_error_syntax(current_->type, current_->value);
		exit(1);
	}
	
	const auto new_node = new ast_node;
	new_node->type = node_string;
	new_node->value = &current_->value;
	proceed(token_string);

	return new_node;
}

/*
 *Parse keyword types
 */
ast_node* parser::parse_keyword_int()
{
	//syntax error handled
	proceed(token_keyword);

	const auto new_node = new ast_node;
	new_node->expression_type = parser_tokens_[index_+1]->type == token_equals ? expression_declaration_and_assigment : expression_declaration_only;
	new_node->type = node_keyword_int;
	new_node->node_type_check = node_int;
	new_node->child = parse_identifier();

	return new_node;
}

ast_node* parser::parse_keyword_string()
{
	//syntax error handled
	proceed(token_keyword);

	const auto new_node = new ast_node;
	new_node->expression_type = parser_tokens_[index_+1]->type == token_equals ? expression_declaration_and_assigment : expression_declaration_only;
	new_node->type = node_keyword_string;
	new_node->node_type_check = node_string;
	new_node->child = parse_identifier();
	
	return new_node;
}

ast_node* parser::parse_keyword_return()
{
	
	proceed(token_keyword);

	const auto new_node = new ast_node;
	new_node->expression_type = expression_return;
	new_node->type = node_return;
	if (current_->type == token_int)
	{
		new_node->child = parse_integer();
	}
	else if (current_->type == token_quotes_double)
	{
		proceed(token_quotes_double);
		new_node->child = parse_string();
		proceed(token_quotes_double);
	}
	else
	{
		std::cout << "parse keyword return error" << '\n';
		raise_error_syntax(current_->type, current_->value);
	}

	return new_node;
}

ast_node* parser::parse_keyword_print()
{
	proceed(token_keyword);
	
	const auto new_node = new ast_node;
	new_node->expression_type = expression_print;
	new_node->type = node_print;
	
	proceed(token_left_paren);
	
	if (current_->type == token_int)
	{
		new_node->child = parse_integer();
	}
	else if (current_->type == token_quotes_double)
	{
		proceed(token_quotes_double);
		new_node->child = parse_string();
		proceed(token_quotes_double);
		proceed(token_right_paren);
	}
	else if (current_->type == token_identifier)
	{
		new_node->child = parse_identifier();
	}
	else
	{
		std::cout << "parse keyword print error" << '\n';
		raise_error_syntax(current_->type, current_->value);
	}
	
	//proceed(token_right_paren);
	return new_node;
}

ast_node* parser::parse()
{
	//std::cout << "parse called" << std::endl; //debug
	const auto root = new ast_node();
	root->type = node_root;

	while (current_->type != token_eof)
	{
		switch (current_->type)  // NOLINT(clang-diagnostic-switch-enum)
		{
			case token_keyword:
			{
				root->sub_statements.push_back(parse_keyword());
				break;
			}
			case token_identifier :
			{
				root->sub_statements.push_back(parse_identifier(true));
				break;
			}
			default :
			{
				std::cout << "parse error" << '\n';	
				raise_error_syntax(current_->type, current_->value);
				exit(1);
			}
		}
		
		proceed(token_semicolon);
	}

	return root;
}