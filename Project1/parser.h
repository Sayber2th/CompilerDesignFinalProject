#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

enum token_type : std::uint8_t;
struct token;

enum node_type : std::uint8_t
{
	node_root,
	node_return,
	node_print,
	node_int,
	node_string,
	node_identifier,
	node_keyword_int,
	node_keyword_string
};

enum expression_type : std::uint8_t
{
	expression_declaration_only,
	expression_declaration_and_assigment,
	expression_assigment,
	expression_return,
	expression_print
};

struct ast_node
{
	node_type type;
	node_type node_type_check;
	expression_type expression_type;
	std::string* value = nullptr;
	ast_node* child;
	std::vector<ast_node*> sub_statements;
};

std::string node_type_to_string(enum node_type type);
std::string expression_type_to_string(enum expression_type type);

class parser
{
public:
	explicit parser(const std::vector<token*>& tokens) :
		parser_tokens_(tokens),
		index_(0),
		limit_(parser_tokens_.size()),
		current_(parser_tokens_.at(index_))
	{
		
	}

	static void raise_error_syntax(token_type token_type, const std::string& token_value); //improve the formatting for this later
	token* proceed(token_type type);
	
	ast_node* parse_identifier(bool is_statement_beginning = false);
	ast_node* parse_keyword();
	
	/*
	*Parse data types
	*/
	ast_node* parse_integer();
	ast_node* parse_string();
	
	/*
	*Parse keyword types
	*/
	ast_node* parse_keyword_int();
	ast_node* parse_keyword_string();
	ast_node* parse_keyword_return();
	ast_node* parse_keyword_print();
	
	ast_node* parse();

private:
	std::vector<token*> parser_tokens_;
	size_t index_;
	size_t limit_;
	token* current_;
};

#endif // !PARSER_H