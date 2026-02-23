// ReSharper disable CppClangTidyBugproneBranchClone
#include "semanticAnalyzer.h"
#include "parser.h"

#include <iostream>

std::string semantic_analyzer::get_data_type_mapped_to_node_kind(const node_kind node_kind)
{
    switch (node_kind)  // NOLINT(clang-diagnostic-switch-enum)
    {
    case node_string_literal: return "string";
    case node_integer_literal: return "int";
    default:
        std::cerr << "Internal semantic error: unsupported RHS node\n";
        exit(1);
    }
}

void semantic_analyzer::analyse()
{
    for (const ast_node* node : program_->children)
    {
        if (node->kind == node_declaration)
        {
            track_declared_identifiers(node);
            if (node->children.size() > 1)
            {
                perform_type_check(node);
            }
        }
        
        if (node->kind == node_assignment)
        {
            check_identifier_declaration_before_reference(node);
            perform_type_check(node);
        }
        
        if (node->kind == node_print_stmt)
        {
            check_identifier_declaration_before_reference(node);
        }
    }
}

bool semantic_analyzer::is_declared(const std::string& value) const
{
    const bool is_declared = declared_identifiers_.contains(value);
    return is_declared;
}

void semantic_analyzer::track_declared_identifiers(const ast_node* node)
{
    if (!is_declared(node->children[0]->value))
    {
        declared_identifiers_.insert({node->children[0]->value, node->data_type});
    }
}

void semantic_analyzer::check_identifier_declaration_before_reference(const ast_node* node) const
{
    if (node->kind == node_assignment)
    {
        if (!is_declared(node->children[0]->value))
        {
            std::cerr<< "\nVariable must be declared before usage: " << node->children[0]->value << '\n';
            exit(1);
        }
    }
    
    if (node->kind == node_print_stmt)
    {
        if (node->children[0]->kind != node_string_literal && node->children[0]->kind != node_integer_literal)
        {
            if (!is_declared(node->children[0]->value))
            {
                std::cerr<< "\nVariable must be declared before usage: " << node->children[0]->value << '\n';
                exit(1);
            }
        }
    }
}

void semantic_analyzer::perform_type_check(const ast_node* assigment_node) const
{
    const bool declaration_type_matches_stored_value = 
        declared_identifiers_.at(assigment_node->children[0]->value) == get_data_type_mapped_to_node_kind(assigment_node->children[1]->kind);
    
    if (!declaration_type_matches_stored_value)
    {
        const std::string variable_type = declared_identifiers_.at(assigment_node->children[0]->value);
        std::cerr << "\nData stored in the variable must match its declared type!" << '\n'
        << "Variable: " << assigment_node->children[0]->value << '\n'
        << "Type of variable: " << variable_type << '\n'
        << "Type of data attempted to pass into variable: " << node_kind_to_debug_string(assigment_node->children[1]->kind) << '\n';
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
        << "Symbol: " << symbol << " | Type: " << type << '\n';
    }
}
