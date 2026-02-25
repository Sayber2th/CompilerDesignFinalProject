#ifndef TACINTERPRETER_H
#define TACINTERPRETER_H

#include <string>
#include <unordered_map>
#include <vector>

struct value
{
    bool is_string = false;
    int int_val = 0;
    std::string str_val;
};

class tac_interpreter
{
public:
    explicit tac_interpreter(const std::vector<std::string>& code):
        code_(code) {}

    void run();

private:
    std::vector<std::string> code_;
    std::unordered_map<std::string, value> memory_;
    std::vector<std::string> output_buffer_;

    void execute(const std::string& line);
    value get_value(const std::string& token);
};

#endif //TACINTERPRETER_H