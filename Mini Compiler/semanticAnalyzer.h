#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <string>
#include <unordered_map>

enum node_kind : std::uint8_t;
struct ast_node;

struct symbol_info
{
    std::string type;
    bool initialized = false;
};

class semantic_analyzer
{
public:
    explicit semantic_analyzer(const ast_node* program) :
        program_(program) {}

    void analyse();
    [[nodiscard]] bool is_declared(const std::string& value) const;
    void track_declared_identifiers(const ast_node* node);
    void perform_type_check(const ast_node* node);
    void print_symbol_table() const;
    
private:
    const ast_node* program_;
    std::unordered_map<std::string, symbol_info> declared_identifiers_;
    
    std::string infer_expression_type(const ast_node* node) const;
};

#endif //SEMANTIC_ANALYZER_H