// ReSharper disable CppClangTidyBugproneBranchClone
#include "semanticAnalyzer.h"
#include "parser.h"

#include <iostream>

void semantic_analyzer::analyse()
{
    for (const ast_node* node : program_->children)
    {
        if (node->kind == node_declaration)
        {
            // Declaration with initialization
            if (node->children.size() > 1)
            {
                const ast_node* rhs = node->children[1];

                validate_expression_identifiers(rhs);

                const std::string rhs_type = infer_expression_type(rhs);

                const std::string& declared_type = node->data_type;
                const std::string& var_name = node->children[0]->value;

                if (declared_type != rhs_type)
                {
                    std::cerr << "\nData stored in the variable must match its declared type!\n"
                    << "Variable: " << var_name << '\n'
                    << "Declared type: " << declared_type << '\n'
                    << "Expression type: " << rhs_type << '\n';
                    exit(1);
                }
            }

            track_declared_identifiers(node);
        }
        
        if (node->kind == node_assignment)
        {
            check_identifier_declaration_before_reference(node);
            perform_type_check(node);
        }
        
        if (node->kind == node_print_stmt)
        {
            validate_expression_identifiers(node->children[0]);
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
    const std::string& name = node->children[0]->value;

    if (is_declared(name))
    {
        std::cerr << "Variable already declared: " << name << '\n';
        exit(1);
    }

    declared_identifiers_.insert({name, node->data_type});
}

void semantic_analyzer::check_identifier_declaration_before_reference(const ast_node* node) const
{
    if (!is_declared(node->children[0]->value))
    {
        std::cerr<< "\nVariable must be declared before usage: " << node->children[0]->value << '\n';
        exit(1);
    }
}

void semantic_analyzer::perform_type_check(const ast_node* node) const
{
    const std::string& var_name = node->children[0]->value;
    const std::string declared_type = declared_identifiers_.at(var_name);

    const ast_node* rhs = node->children[1];

    validate_expression_identifiers(rhs);

    const std::string rhs_type = infer_expression_type(rhs);

    if (declared_type != rhs_type)
    {
        std::cerr << "\nData stored in the variable must match its declared type!\n"
                  << "Variable: " << var_name << '\n'
                  << "Declared type: " << declared_type << '\n'
                  << "Expression type: " << rhs_type << '\n';
        exit(1);
    }
}

void semantic_analyzer::print_symbol_table() const
{
    std::cout << "\nSymbol Table: " << '\n';
    size_t counter = declared_identifiers_.size();
    for (const auto& [symbol, type] :declared_identifiers_)
    {
        counter --;
        std::cout << declared_identifiers_.size() - counter << ")" 
        << "Symbol: " << symbol << " | Type: " << type << '\n';
    }
}

std::string semantic_analyzer::infer_expression_type(const ast_node* node) const
{
    switch (node->kind)  // NOLINT(clang-diagnostic-switch-enum)
    {
    case node_integer_literal:
        return "int";

    case node_string_literal:
        return "string";

    case node_identifier:
        {
            if (!is_declared(node->value))
            {
                std::cerr << "Variable must be declared before usage: "
                          << node->value << '\n';
                exit(1);
            }
            return declared_identifiers_.at(node->value);
        }

    case node_add:
    case node_subtract:
    case node_multiply:
    case node_divide:
        {
            const std::string left  = infer_expression_type(node->children[0]);
            const std::string right = infer_expression_type(node->children[1]);

            if (left != "int" || right != "int")
            {
                std::cerr << "Arithmetic only allowed on integers\n";
                exit(1);
            }

            return "int";
        }

    default:
        std::cerr << "Unknown expression node during semantic analysis\n";
        exit(1);
    }
}

void semantic_analyzer::validate_expression_identifiers(const ast_node* node) const
{
    if (node->kind == node_identifier)
    {
        if (!is_declared(node->value))
        {
            std::cerr << "Variable must be declared before usage: "
                      << node->value << '\n';
            exit(1);
        }
        return;
    }

    for (const ast_node* child : node->children)
    {
        validate_expression_identifiers(child);
    }
}
