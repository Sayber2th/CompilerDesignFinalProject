#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

enum token_type : std::uint8_t;
struct token;

enum node_kind : std::uint8_t
{
	node_program,
	node_declaration,
	node_assignment,
	node_print_stmt,
	node_identifier,
	node_string_literal,
	node_integer_literal,
	node_add,
	node_subtract,
	node_multiply,
	node_divide,
	node_unary_minus
};

struct ast_node
{
	node_kind kind;
	std::string value;
	std::string data_type;
	std::vector<ast_node*> children;
	ast_node* lhs = nullptr;
	ast_node* rhs = nullptr;
};

std::string node_kind_to_debug_string(node_kind kind);

class parser
{
public:
	explicit parser(const std::vector<token*>& tokens) :
		parser_tokens_(tokens),
		index_(0),
		limit_(parser_tokens_.size()),
		current_(parser_tokens_.at(index_)) {}

	
	[[noreturn]] void syntax_error_protocol() const;
	static void raise_error_syntax(token_type token_type, const std::string& token_value, int line_number, int character_number); //improve the formatting for this later

	static void print_node(const ast_node* node, const std::string& prefix, bool is_last);
	static void print_abstract_syntax_tree(const ast_node*& program);
	
	token* proceed(token_type type);
	
	/*
	*Parse statement types
	*/
	ast_node* parse_declaration();
	ast_node* parse_assignment();
	ast_node* parse_expression();
	ast_node* parse_term();
	ast_node* parse_factor();
	ast_node* parse_print_stmt();
	
	ast_node* parse_identifier();
	
	/*
	*Parse data types
	*/
	ast_node* parse_integer();
	ast_node* parse_string();
	
	ast_node* parse_program();

private:
	std::vector<token*> parser_tokens_;
	size_t index_;
	size_t limit_;
	token* current_;
};

#endif // PARSER_H