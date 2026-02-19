// ReSharper disable CppClangTidyBugproneBranchClone
#include "semanticAnalyzer.h"
#include "parser.h"

#include <iostream>

void semantic_analyzer::analyse()
{
    for (const ast_node* node : root_->sub_statements)
    {
        if (node->expression_type == expression_declaration_and_assigment || node->expression_type == expression_declaration_only)
        {
            track_declared_identifiers(node);
        }
        
        if (node->expression_type == expression_assigment)
        {
            check_identifier_declaration_before_reference(node);
            perform_type_check(node);
        }
        
        if (node->expression_type == expression_return || node->expression_type == expression_print)
        {
            check_identifier_declaration_before_reference(node);
        }
    }
}

bool semantic_analyzer::is_declared(const std::string* value) const
{
    const bool is_declared = declared_identifiers_.contains(*value);
    return is_declared;
}

void semantic_analyzer::track_declared_identifiers(const ast_node* node)
{
    if (!is_declared(node->child->value))
    {
        declared_identifiers_.insert({*node->child->value, node->node_type_check});
    }
}

void semantic_analyzer::check_identifier_declaration_before_reference(const ast_node* node) const
{
    if (node->expression_type == expression_assigment)
    {
        if (!is_declared(node->value))
        {
            std::cerr<< "\nVariable must be declared before usage: " << *node->value << '\n';
            exit(1);
        }
    }
    
    if (node->expression_type == expression_return || node->expression_type == expression_print)
    {
        std::cout << "\nRecognised print or return statement." << '\n'; //debug
        if (!is_declared(node->child->value))
        {
            std::cerr<< "\nVariable must be declared before usage: " << *node->child->value << '\n';
            exit(1);
        }
    }
}

void semantic_analyzer::perform_type_check(const ast_node* identifier_node)
{
    const bool declaration_type_matches_stored_value = 
        declared_identifiers_[*identifier_node->value] == identifier_node->child->type;
    
    if (!declaration_type_matches_stored_value)
    {
        const node_type variable_type = declared_identifiers_[*identifier_node->value];
        std::cerr << "\nValue stored in the variable must match its declared type!" << '\n'
        << "Variable: " << *identifier_node->value << '\n'
        << "Type of variable: " << node_type_to_string(variable_type) << '\n'
        << "Type of value attempted to pass into variable: " << node_type_to_string(identifier_node->child->type) << '\n';
        exit(1);
    }
}

void semantic_analyzer::print_symbol_table() const
{
    std::cout << "\nList of symbols: " << '\n';
    size_t counter = declared_identifiers_.size();
    for (const auto& [symbol, type] :declared_identifiers_)
    {
        counter --;
        std::cout << declared_identifiers_.size() - counter << ")" 
        << "Symbol: " << symbol << " | Type: " << node_type_to_string(type) << '\n';
    }
}
