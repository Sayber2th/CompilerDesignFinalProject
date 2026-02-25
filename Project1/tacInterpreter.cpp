#include "tacInterpreter.h"

#include <iostream>
#include <sstream>

void tac_interpreter::run()
{
    std::cout << "\nOutput of the program:\n";
    for (const auto& line : code_)
        execute(line);
    
    for (const auto& out : output_buffer_)
        std::cout << out << '\n';
}

void tac_interpreter::execute(const std::string& line)
{
    std::stringstream ss(line);

    std::string first;
    ss >> first;

    //print
    if (first == "print")
    {
        std::string arg;
        std::getline(ss >> std::ws, arg);

        auto [is_string, int_val, str_val] = get_value(arg);

        if (is_string)
            output_buffer_.push_back(str_val);
        else
            output_buffer_.push_back(std::to_string(int_val));

        return;
    }

    //assigment
    std::string equals;
    ss >> equals; // "="

    std::string rhs;
    std::getline(ss >> std::ws, rhs);
    
    //string literal
    if (!rhs.empty() && rhs.front() == '"' && rhs.back() == '"')
    {
        memory_[first] = get_value(rhs);
        return;
    }

    //unary minus
    if (rhs.rfind("uminus ", 0) == 0)  // NOLINT(modernize-use-starts-ends-with)
    {
        std::string operand = rhs.substr(7);
        value v = get_value(operand);
        memory_[first] = value{.is_string = false, .int_val = -v.int_val, .str_val = ""};
        return;
    }

    //binary
    std::stringstream rhs_stream(rhs);
    std::string a, op, b;

    if (rhs_stream >> a >> op >> b)
    {
        value va = get_value(a);
        value vb = get_value(b);

        int result = 0;

        if (op == "+") result = va.int_val + vb.int_val;
        else if (op == "-") result = va.int_val - vb.int_val;
        else if (op == "*") result = va.int_val * vb.int_val;
        else if (op == "/")
        {
            if (vb.int_val == 0)
            {
                std::cerr << "Runtime error: division by zero\n";
                exit(1);
            }
            result = va.int_val / vb.int_val;
        }

        memory_[first] = value{.is_string = false, .int_val = result, .str_val = ""};
        return;
    }

    memory_[first] = get_value(rhs);
}

value tac_interpreter::get_value(const std::string& token)
{
    auto it = memory_.find(token);
    if (it != memory_.end())
        return it->second;

    if (token.size() >= 2 && token.front() == '"' && token.back() == '"')
        return value{.is_string = true, .int_val = 0, .str_val = token.substr(1, token.size()-2)};

    const size_t start = (token[0] == '-') ? 1 : 0;

    for (size_t i = start; i < token.size(); ++i)
    {
        if (!isdigit(token[i]))
        {
            std::cerr << "\nRuntime error: unknown value '" << token << "'\n";
            exit(1);
        }
    }

    return value{.is_string = false, .int_val = std::stoi(token), .str_val = ""};
}