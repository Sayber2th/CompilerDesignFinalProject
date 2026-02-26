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
    std::cout << "\nOptimized TAC:\n";
    for (const auto& line : code_)
        std::cout << line << '\n';
}

void tac_optimizer::constant_propagation()
{
    std::unordered_map<std::string, std::string> constants;

    for (auto& line : code_)
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
            size_t pos = rhs.find(var);
            if (pos != std::string::npos)
                rhs.replace(pos, var.length(), value);
        }

        line = lhs + " = " + rhs;
    }
}

void tac_optimizer::constant_folding()
{
    for (auto& line : code_)
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
                else if (op == "/") result = y != 0 ? x / y : 0;

                line = lhs + " = " + std::to_string(result);
            }
        }
    }
}

void tac_optimizer::dead_code_elimination()
{
    std::unordered_set<std::string> used;
    std::vector<std::string> new_code;

    // First pass: collect used variables
    for (const auto& line : code_)
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
                if (!std::isdigit(token[0]) && token != "uminus")
                    used.insert(token);
            }
        }
    }

    // Second pass: remove unused assignments
    for (const auto& line : code_)
    {
        std::stringstream ss(line);
        std::string lhs, eq;
        ss >> lhs >> eq;

        if (eq == "=" && !used.contains(lhs) && lhs[0] == 't')
            continue;

        new_code.push_back(line);
    }

    code_ = new_code;
}