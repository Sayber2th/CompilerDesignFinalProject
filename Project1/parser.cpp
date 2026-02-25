#include "parser.h"

#include <csignal>

#include "lexer.h"

#include <iostream>
#include <cstdlib>
#include <string>

std::string node_kind_to_debug_string(const node_kind kind)
{
	switch (kind)
	{
	case node_program: return "Root program node";
	case node_declaration: return "Declaration statement";
	case node_assignment: return "Assigment statement";
	case node_print_stmt: return "Print statement";
	case node_identifier: return "Identifier";
	case node_string_literal: return "String literal";
	case node_integer_literal: return "Integer literal";
	case node_add: return "Add";
	case node_subtract: return "Subtract";
	case node_multiply: return "Multiply";
	case node_divide: return "Divide";
	default: return "Unrecognized node type";  // NOLINT(clang-diagnostic-covered-switch-default)
	}
}

void parser::syntax_error_protocol() const
{
	raise_error_syntax(current_->type, current_->value, current_->line_number, current_->character_number);
	exit(1);
}

void parser::raise_error_syntax(const token_type token_type, const std::string& token_value, const int line_number, const int character_number)
{
	std::cerr << "\n[!] SYNTAX ERROR " << token_type_to_string(token_type) << '\n' 
	<< "Token: " << token_value << " | " 
	<< "At line: " << line_number << " | " << " and character: " << character_number << '\n';
}

void parser::print_node(const ast_node* node, const std::string& prefix, const bool is_last)
{
	if (!node) return;

	std::cout << prefix;
	std::cout << (is_last ? "└── " : "├── ");  // NOLINT(misc-redundant-expression, bugprone-branch-clone)
	std::cout << node_kind_to_debug_string(node->kind);

	if (!node->value.empty())
		std::cout << ": " << node->value;

	if (!node->data_type.empty())
		std::cout << " | type: " << node->data_type;

	std::cout << '\n';

	const std::string child_prefix = prefix + (is_last ? "    " : "│   ");  // NOLINT(clang-diagnostic-invalid-source-encoding)

	for (size_t i = 0; i < node->children.size(); ++i)
	{
		const bool last_child = (i == node->children.size() - 1);
		print_node(node->children[i], child_prefix, last_child);
	}
}

void parser::print_abstract_syntax_tree(const ast_node*& program)
{
	if (!program) return;

	std::cout << "\n\n";
	std::cout << node_kind_to_debug_string(program->kind) << '\n';

	for (size_t i = 0; i < program->children.size(); ++i)
	{
		const bool last_child = (i == program->children.size() - 1);
		print_node(program->children[i], "", last_child);
	}
}

token* parser::proceed(const token_type type)
{
	if (current_->type != type) syntax_error_protocol();
	
	else
	{
		index_ ++;
		current_ = parser_tokens_.at(index_);
		return current_;
	}
}

/*
 *Parse statement types
 */
ast_node* parser::parse_declaration()
{
	const auto new_node = new ast_node;
	new_node->kind = node_declaration;
	new_node->data_type = current_->value;
	
	proceed(token_keyword);
	new_node->children.push_back(parse_identifier());
	
	if (current_->type == token_equals)
	{
		proceed(token_equals);
		if (current_->type == token_quotes_double)
		{
			proceed(token_quotes_double);
			new_node->children.push_back(parse_string());
			proceed(token_quotes_double);
		}
		else
		{
			new_node->children.push_back(parse_expression());
		}
	}
	return new_node;
}

ast_node* parser::parse_assignment()
{
	const auto new_node = new ast_node;
	new_node->kind = node_assignment;
	new_node->children.push_back(parse_identifier());
	
	if (current_->type != token_equals) syntax_error_protocol();
	
	proceed(token_equals);
	
	if (current_->type == token_quotes_double)
	{
		proceed(token_quotes_double);
		new_node->children.push_back(parse_string());
		proceed(token_quotes_double);
	}
	else
	{
		new_node->children.push_back(parse_expression());
	}
	
	return new_node;
}

ast_node* parser::parse_expression()
{
	ast_node* node = parse_term();

	while (current_->type == token_plus || current_->type == token_minus)
	{
		const token_type op = current_->type;
		proceed(op);

		auto* op_node = new ast_node;
		op_node->kind = (op == token_plus) ? node_add : node_subtract;

		op_node->children.push_back(node);
		op_node->children.push_back(parse_term());

		node = op_node;
	}

	return node;
}

ast_node* parser::parse_term()
{
	ast_node* node = parse_factor();

	while (current_->type == token_star || current_->type == token_slash)
	{
		const token_type op = current_->type;
		proceed(op);

		auto* op_node = new ast_node;
		op_node->kind = (op == token_star) ? node_multiply : node_divide;

		op_node->children.push_back(node);
		op_node->children.push_back(parse_factor());

		node = op_node;
	}

	return node;
}

ast_node* parser::parse_factor()
{
	if (current_->type == token_minus)
	{
		proceed(token_minus);

		auto* node = new ast_node;
		node->kind = node_unary_minus;
		node->children.push_back(parse_factor());

		return node;
	}
	
	if (current_->type == token_int) return parse_integer();

	if (current_->type == token_identifier) return parse_identifier();

	if (current_->type == token_left_paren)
	{
		proceed(token_left_paren);
		ast_node* node = parse_expression();
		proceed(token_right_paren);
		return node;
	}

	syntax_error_protocol();
}

ast_node* parser::parse_identifier()
{
	if (current_->type != token_identifier) syntax_error_protocol();
	
	const auto new_node = new ast_node;
	new_node->value = current_->value;
	new_node->kind = node_identifier;
	proceed(token_identifier);
	
	return new_node;
}

ast_node* parser::parse_print_stmt()
{
	proceed(token_keyword);
	
	const auto new_node = new ast_node;
	new_node->kind = node_print_stmt;
	
	proceed(token_left_paren);
	
	if (current_->type == token_quotes_double)
	{
		proceed(token_quotes_double);
		new_node->children.push_back(parse_string());
		proceed(token_quotes_double);
	}
	else
	{
		new_node->children.push_back(parse_expression());
	}
	
	proceed(token_right_paren);
	
	return new_node;
}

/*
 *Parse data types
 */
ast_node* parser::parse_integer()
{
	if (current_->type != token_int) syntax_error_protocol();
	
	const auto new_node = new ast_node;
	new_node->kind = node_integer_literal;
	new_node->value = current_->value;
	proceed(token_int);

	return new_node;
}

ast_node* parser::parse_string()
{
	if (current_->type != token_string) syntax_error_protocol();
	
	const auto new_node = new ast_node;
	new_node->kind = node_string_literal;
	new_node->value = current_->value;
	proceed(token_string);

	return new_node;
}

ast_node* parser::parse_program()
{
	const auto program = new ast_node();
	program->kind = node_program;

	while (current_->type != token_eof)
	{
		switch (current_->type)  // NOLINT(clang-diagnostic-switch-enum)
		{
		case token_keyword:
			{
				if (current_->value == "int" || current_->value == "string")
				{
					program->children.push_back(parse_declaration());
				}
				else if (current_->value == "print")
				{
					program->children.push_back(parse_print_stmt());
				}
				break;
			}
		case token_identifier:
			{
				program->children.push_back(parse_assignment());
				break;
			}
		default:
			{
				syntax_error_protocol();
			}
		}
		
		proceed(token_semicolon);
	}

	return program;
}