#include "tacGenerator.h"
#include "parser.h"

#include <iostream>
#include <cstdlib>

std::string tac_generator::new_temp()
{
    return "t" + std::to_string(++temp_counter_);
}

std::string tac_generator::gen_expression(const ast_node* node)
{
    switch (node->kind)  // NOLINT(clang-diagnostic-switch-enum)
    {
    case node_integer_literal:
        return node->value;
    
    case node_string_literal:
        return "\"" + node->value + "\"";

    case node_identifier:
        return node->value;

    case node_add:
    case node_subtract:
    case node_multiply:
    case node_divide:
        {
            const std::string left  = gen_expression(node->children[0]);
            const std::string right = gen_expression(node->children[1]);

            std::string temp = new_temp();

            std::string op;
            if (node->kind == node_add) op = "+";
            else if (node->kind == node_subtract) op = "-";
            else if (node->kind == node_multiply) op = "*";
            else op = "/";

            code_.push_back(temp + " = " + left + " " + op + " " + right);

            return temp;
        }
        
    case node_unary_minus:
        {
            const std::string val = gen_expression(node->children[0]);
            std::string temp = new_temp();

            code_.push_back(temp + " = uminus " + val);

            return temp;
        }

    default:
        std::cerr << "Unsupported expression node in TAC\n";
        exit(1);
    }
}

void tac_generator::gen_statement(const ast_node* node)
{
    switch (node->kind)  // NOLINT(clang-diagnostic-switch-enum)
    {
    case node_declaration:
        {
            // initializer exists?
            if (node->children.size() > 1)
            {
                std::string rhs = gen_expression(node->children[1]);
                std::string var = node->children[0]->value;

                code_.push_back(var + " = " + rhs);
            }
            break;
        }

    case node_assignment:
        {
            const std::string rhs = gen_expression(node->children[1]);
            const std::string var = node->children[0]->value;

            code_.push_back(var + " = " + rhs);
            break;
        }

    case node_print_stmt:
        {
            const std::string val = gen_expression(node->children[0]);
            code_.push_back("print " + val);
            break;
        }

    default:
        std::cerr << "Unsupported statement in TAC\n";
        exit(1);
    }
}

void tac_generator::generate()
{
    for (const ast_node* stmt : program_->children)
    {
        gen_statement(stmt);
    }
}

void tac_generator::print() const
{
    std::cout << "\nGenerated TAC:\n";
    for (const auto& line : code_)
        std::cout << line << '\n';
}