#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <string>

enum node_type : std::uint8_t
{
	node_root,
	node_return,
	node_print,
	node_int,
	node_identifier,
	node_variable,
	node_keyword_int
};

struct ast_node
{
	enum node_type type;
	std::string* value;
	ast_node* child;
	std::vector<ast_node*> sub_statements;
};

class parser
{
public:
	explicit parser(const std::vector<token*>& tokens) :
		parser_tokens_(tokens),
		index_(0),
		limit_(parser_tokens_.size()),
		current_(parser_tokens_.at(index_))
	{
		/*parserTokens = tokens;
		index = 0;
		limit = parserTokens.size();
		current = parserTokens.at(index);*/
	}

	static void raise_error_syntax(token_type token_type, const std::string& token_value); //improve the formatting for this later
	token* proceed(enum token_type type);
	ast_node* parse_identifier();
	ast_node* parse_keyword();
	ast_node* parse_integer();
	ast_node* parse_keyword_int();
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
