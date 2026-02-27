#include "tacOptimizer.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

void tac_optimizer::optimize()
{
    constant_propagation();
    constant_folding();
    dead_code_elimination();
}

void tac_optimizer::print_optimised_tac() const
{
    int counter = 0;
    std::cout << "\nOptimized TAC:\n";
    for (const auto& line : tac_optimised_)
    {
        counter ++;
        std::cout << counter << "] " << line << '\n';
    }
}

void tac_optimizer::constant_propagation()
{
    std::unordered_map<std::string, std::string> constants;

    for (auto& line : tac_optimised_)
    {
        std::stringstream ss(line);

        std::string lhs, eq;
        ss >> lhs >> eq;

        if (eq != "=") continue;

        std::string rhs;
        std::getline(ss >> std::ws, rhs);

        // Simple constant assignment
        if (!rhs.empty() && std::all_of(rhs.begin(), rhs.end(),
            [](const char c){ return std::isdigit(c) || c == '-'; }))
        {
            constants[lhs] = rhs;
        }
        else
        {
            constants.erase(lhs);
        }

        // Replace occurrences in RHS
        for (auto& [var, value] : constants)
        {
            size_t pos = 0;
            while ((pos = rhs.find(var, pos)) != std::string::npos)
            {
                rhs.replace(pos, var.length(), value);
                pos += value.length();
            }
        }

        line = lhs + " = " + rhs;
    }
}

void tac_optimizer::constant_folding()
{
    for (auto& line : tac_optimised_)
    {
        std::stringstream ss(line);

        std::string lhs, eq;
        ss >> lhs >> eq;

        if (eq != "=") continue;

        std::string a, op, b;
        if (ss >> a >> op >> b)
        {
            if (std::isdigit(a[0]) && std::isdigit(b[0]))
            {
                const int x = std::stoi(a);
                const int y = std::stoi(b);
                int result = 0;

                if (op == "+") result = x + y;
                else if (op == "-") result = x - y;
                else if (op == "*") result = x * y;
                else if (op == "/")
                {
                    if (y == 0)
                    {
                        std::cerr << "Runtime error: division by zero\n";
                        exit(1);
                    }
                    result = x/y;
                }

                line = lhs + " = " + std::to_string(result);
            }
        }
    }
}

void tac_optimizer::dead_code_elimination()
{
    while (true)
    {
        std::unordered_set<std::string> used;
        std::vector<std::string> new_code;

        // First pass: collect used variables
        for (const auto& line : tac_optimised_)
        {
            if (line.rfind("print", 0) == 0)
            {
                std::stringstream ss(line);
                std::string p, var;
                ss >> p >> var;
                used.insert(var);
            }
            else
            {
                std::stringstream ss(line);
                std::string lhs, eq, rhs;
                ss >> lhs >> eq;
                std::getline(ss >> std::ws, rhs);

                std::stringstream rhs_stream(rhs);
                std::string token;
                while (rhs_stream >> token)
                {
                    bool is_number = std::all_of(token.begin(), token.end(), 
                        [](char c) { return std::isdigit(c) || c == '-'; });

                    if (!is_number && token != "uminus")
                        used.insert(token);
                }
            }
        }

        bool removed_any = false;

        // Second pass
        for (const auto& line : tac_optimised_)
        {
            std::stringstream ss(line);
            std::string lhs, eq;
            ss >> lhs >> eq;

            if (eq == "=" && !used.contains(lhs))
            {
                removed_any = true;
                continue;
            }

            new_code.push_back(line);
        }

        tac_optimised_ = new_code;

        if (!removed_any)
            break;
    }
}