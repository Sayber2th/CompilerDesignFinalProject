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
            perform_type_check(node);
        }
        
        if (node->kind == node_print_stmt)
        {
            infer_expression_type(node->children[0]);
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
        std::cerr << "\nVariable already declared: " << name << '\n';
        exit(1);
    }

    symbol_info info;
    info.type = node->data_type;

    // initialized ONLY if declaration had initializer
    info.initialized = (node->children.size() > 1);

    declared_identifiers_.insert({name, info});
}

void semantic_analyzer::perform_type_check(const ast_node* node)
{
    const std::string& var_name = node->children[0]->value;
    
    if (!is_declared(var_name))
    {
        std::cerr << "\nVariable must be declared before assignment: "
                  << var_name << '\n';
        exit(1);
    }
    
    const std::string declared_type = declared_identifiers_.at(var_name).type;

    const ast_node* rhs = node->children[1];
    const std::string rhs_type = infer_expression_type(rhs);

    if (declared_type != rhs_type)
    {
        std::cerr << "\nData stored in the variable must match its declared type!\n"
                  << "Variable: " << var_name << '\n'
                  << "Declared type: " << declared_type << '\n'
                  << "Expression type: " << rhs_type << '\n';
        exit(1);
    }
    
    declared_identifiers_.at(var_name).initialized = true;
}

void semantic_analyzer::print_symbol_table() const
{
    std::cout << "\nSymbol Table: " << '\n';
    size_t counter = declared_identifiers_.size();
    for (const auto& [symbol, info] : declared_identifiers_)
    {
        counter--;
        std::cout << declared_identifiers_.size() - counter << ") "
                  << "Symbol: " << symbol
                  << " | Type: " << info.type
                  << " | Initialized: " << (info.initialized ? "yes" : "no")
                  << '\n';
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
                std::cerr << "\nVariable must be declared before usage: "
                          << node->value << '\n';
                exit(1);
            }
            const auto& info = declared_identifiers_.at(node->value);

            if (!info.initialized)
            {
                std::cerr << "\nVariable used before initialization: "
                          << node->value << '\n';
                exit(1);
            }

            return info.type;
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
                std::cerr << "\nArithmetic only allowed on integers\n";
                exit(1);
            }

            return "int";
        }
        
    case node_unary_minus:
        {
            const std::string inner = infer_expression_type(node->children[0]);

            if (inner != "int")
            {
                std::cerr << "Unary minus only allowed on integers\n";
                exit(1);
            }

            return "int";
        }

    default:
        std::cerr << "\nUnknown expression node during semantic analysis\n";
        exit(1);
    }
}