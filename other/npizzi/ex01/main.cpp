#include "RPN.hpp"

bool is_string_valid(const char* str)
{
    for (size_t i = 0; str[i]; ++i)
    {
        if (strchr("0123456789+-*/ ", str[i]) == NULL)
            return (false);
    }
    return (true);
}

int main(int argc, char* argv[])
{
    if (argc != 2 || !is_string_valid(argv[1]))
    {
        std::cout << "Error\n";
        return (1);
    }
    RPN rpn;

    try 
    {
        rpn.evaluateExpr(argv[1]);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    return (0);
}