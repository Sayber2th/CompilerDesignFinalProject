#ifndef TACGENERATOR_H
#define TACGENERATOR_H

#include <string>
#include <vector>

struct ast_node;

class tac_generator
{
public:
    explicit tac_generator(const ast_node* program):
        program_(program) {}

    void generate();
    void print() const;
    [[nodiscard]] const std::vector<std::string>& get_code() const { return code_; }

private:
    const ast_node* program_;
    std::vector<std::string> code_;
    int temp_counter_ = 0;

    std::string new_temp();
    std::string gen_expression(const ast_node* node);
    void gen_statement(const ast_node* node);
};

#endif //TACGENERATOR_H
