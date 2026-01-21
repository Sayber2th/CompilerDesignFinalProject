#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <cstdlib>
#include <string>

//improve the formatting for this function later
void parser::raise_error_syntax(const token_type token_type, const std::string& token_value)
{
	std::cerr << "[!] SYNTAX ERROR " << type_to_string(token_type) << " " << token_value << '\n';
}

token* parser::proceed(const enum token_type type)
{
	if (current_->type != type)
	{
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

ast_node* parser::parse_identifier()
{
	proceed(token_identifier);
	proceed(token_equals);

	const auto new_node = new ast_node;
	new_node->type = node_variable;
	new_node->child = parse_integer();

	return new_node;
}

ast_node* parser::parse_keyword()
{
	if (current_->value == "int")
	{
		return parse_keyword_int();
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
		raise_error_syntax(current_->type, current_->value);
		exit(1);
	}
}

ast_node* parser::parse_integer()
{
	if (current_->type != token_int)
	{
		raise_error_syntax(current_->type, current_->value);
		exit(1);
	}

	const auto new_node = new ast_node;
	new_node->type = node_int;
	new_node->value = &current_->value;
	proceed(token_int);

	return new_node;
}

ast_node* parser::parse_keyword_int()
{
	proceed(token_keyword);

	const auto new_node = new ast_node;
	new_node->type = node_keyword_int;
	new_node->child = parse_identifier();

	return new_node;
}

ast_node* parser::parse_keyword_return()
{
	proceed(token_keyword);

	const auto new_node = new ast_node;
	new_node->type = node_return;
	new_node->child = parse_integer();

	return new_node;
}

ast_node* parser::parse_keyword_print()
{
	proceed(token_keyword);

	const auto new_node = new ast_node;
	new_node->type = node_print;
	proceed(token_left_paren);
	new_node->child = parse_integer();
	proceed(token_right_paren);

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
				root->sub_statements.push_back(parse_identifier());
				break;
			}
			default :
			{
				raise_error_syntax(current_->type, current_->value);
				exit(1);
			}
		}

		proceed(token_semicolon);
	}

	return root;
}