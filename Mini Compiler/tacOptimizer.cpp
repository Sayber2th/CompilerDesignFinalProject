#include "tacOptimizer.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

void tac_optimizer::optimize()
{
    std::vector<std::string> previous;
    do
    {
        previous = tac_optimised_;
        constant_propagation();
        constant_folding();
    } while (tac_optimised_ != previous);

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

static bool is_integer_literal(const std::string& s)
{
    if (s.empty()) return false;
    const size_t start = (s[0] == '-') ? 1 : 0;
    if (start == s.size()) return false;
    return std::all_of(s.begin() + start, s.end(),
        [](const unsigned char c){ return std::isdigit(c); });
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

        // A string literal has nothing to propagate into
        if (!rhs.empty() && rhs.front() == '"')
        {
            constants.erase(lhs);
            continue;
        }

        // Replace whole operands only, never substrings
        std::stringstream rhs_stream(rhs);
        std::string token, new_rhs;
        while (rhs_stream >> token)
        {
            const auto it = constants.find(token);
            if (!new_rhs.empty()) new_rhs += ' ';
            new_rhs += (it != constants.end()) ? it->second : token;
        }
        rhs = new_rhs;

        // Record or forget lhs based on the updated rhs
        if (is_integer_literal(rhs)) constants[lhs] = rhs;
        else constants.erase(lhs);

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
                        [](unsigned char c) { return std::isdigit(c) || c == '-'; });

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