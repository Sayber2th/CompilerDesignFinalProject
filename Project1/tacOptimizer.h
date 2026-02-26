#ifndef TACOPTIMIZER_H
#define TACOPTIMIZER_H

#include <vector>
#include <string>

class tac_optimizer
{
public:
    explicit tac_optimizer(const std::vector<std::string>& tac):
        code_(tac) {}

    void optimize();
    void print_optimised_tac() const;

    [[nodiscard]] const std::vector<std::string>& get_optimized_code() const
    {
        return code_;
    }

private:
    std::vector<std::string> code_;

    void constant_folding();
    void constant_propagation();
    void dead_code_elimination();
};

#endif