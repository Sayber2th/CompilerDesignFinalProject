#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum token_type : std::uint8_t
{
	token_identifier,
	token_int,
	token_string,
	token_equals,
	token_keyword,
	token_semicolon,
	token_left_paren,
	token_right_paren,
	token_left_curly,
	token_right_curly,
	token_comma,
	token_quotes_double,
	token_rel_equals,
	token_rel_notequals,
	token_rel_lessthan,
	token_rel_lessthanequals,
	token_rel_greaterthan,
	token_rel_greaterthanequals,
	token_plus,
	token_minus,
	token_star,
	token_slash,
	token_if,
	token_else,
	token_eof
};

struct token
{
	token_type type;
	std::string value;
	int line_number;
	int character_number;
};

std::string token_type_to_string(enum token_type type);

class lexer
{
public:
	explicit lexer(const std::string& source_code) :
		source_(source_code),
		cursor_(0),
		size_(source_code.length()),
		current_(source_code.at(cursor_)),
		line_number_(1),
		character_number_(1)
	{
		
	}

	[[nodiscard]] char peek(int offset) const;
	char advance();
	void check_and_skip_whitespace();
	void check_and_skip_newline();
	static void print_scanned_tokens(const std::vector<token*>& tokens);
	void raise_error_unidentified_symbol() const;

	std::vector<std::string> keywords = {"int", "string", "if", "else", "return", "true", "false", "print"};
	std::vector<char> double_character_specials = {'!', '=', '<', '>'};

	std::vector<token *> tokenize();
	token* tokenize_keyword_or_identifier();
	token* tokenize_integer();
	token* tokenize_special(token_type type);
	token* tokenize_string();

private:
	std::string source_;
	size_t cursor_;
	size_t size_;
	char current_;
	int line_number_;
	int character_number_;
};

#endif //LEXER_H