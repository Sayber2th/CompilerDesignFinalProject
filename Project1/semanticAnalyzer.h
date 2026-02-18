#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <string>
#include <unordered_map>

enum node_type : std::uint8_t;
struct ast_node;

class semantic_analyzer
{
public:
    explicit semantic_analyzer(const ast_node* root) :
        root_(root)
    {
        
    }
    
    void analyse();
    bool is_declared(const std::string* value) const;
    void track_declared_identifiers(const ast_node* node);
    void check_identifier_declaration_before_assigment(const ast_node* identifier_node) const;
    void perform_type_check(const ast_node* identifier_node);
    void print_symbol_table() const;
    
private:
    const ast_node* root_;
    std::unordered_map<std::string, node_type> declared_identifiers_;
};

#endif //SEMANTIC_ANALYZER_H