#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <string>
#include <unordered_map>

enum node_kind : std::uint8_t;
struct ast_node;

class semantic_analyzer
{
public:
    explicit semantic_analyzer(const ast_node* program) :
        program_(program)
    {
        
    }

    static std::string get_data_type_mapped_to_node_kind(node_kind node_kind);
    void analyse();
    bool is_declared(const std::string& value) const;
    void track_declared_identifiers(const ast_node* node);
    void check_identifier_declaration_before_reference(const ast_node* node) const;
    void perform_type_check(const ast_node* assigment_node) const;
    void print_symbol_table() const;
    
private:
    const ast_node* program_;
    std::unordered_map<std::string, std::string> declared_identifiers_;
};

#endif //SEMANTIC_ANALYZER_H