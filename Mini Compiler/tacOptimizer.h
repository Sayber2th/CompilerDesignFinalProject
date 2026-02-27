#ifndef TACOPTIMIZER_H
#define TACOPTIMIZER_H

#include <vector>
#include <string>

class tac_optimizer
{
public:
    explicit tac_optimizer(const std::vector<std::string>& tac):
        tac_optimised_(tac) {}

    void optimize();
    void print_optimised_tac() const;

    [[nodiscard]] const std::vector<std::string>& get_optimized_code() const
    {
        return tac_optimised_;
    }

private:
    std::vector<std::string> tac_optimised_;
    
    void constant_propagation();
    void constant_folding();
    void dead_code_elimination();
};

#endif