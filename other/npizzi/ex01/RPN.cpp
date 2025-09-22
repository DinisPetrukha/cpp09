#include "RPN.hpp"

RPN::RPN()
{
    #ifdef VERBOSE
        std::cout << "RPN constructor called" << std::endl;
    #endif
}

RPN::RPN(const RPN& rpn) : operands(rpn.operands)
{
    #ifdef VERBOSE
        std::cout << "RPN copy constructor called" << std::endl;
    #endif
}

RPN& RPN::operator=(const RPN& rpn)
{
    operands = rpn.operands;
    #ifdef VERBOSE
        std::cout << "RPN Copy assignment operator called" << std::endl;
    #endif
    return (*this);
}

RPN::~RPN()
{
    #ifdef VERBOSE
        std::cout << "RPN destructor called" << std::endl;
    #endif
}

void RPN::executeCalc(char op)
{
    int a;
    int b;

    if (operands.size() < 2)
        throw std::runtime_error("Error not enough operands");
    b = operands.top();
    operands.pop();
    a = operands.top();
    operands.pop();
    switch (op)
    {
    case '*':
        operands.push(a * b);
        break;
    case '/':
        if (b == 0)
            throw std::runtime_error("Error division by 0");
        operands.push(a / b);
        break;
    case '+':
        operands.push(a + b);
        break;
    case '-':
        operands.push(a - b);
        break;
    }
}

void RPN::evaluateExpr(const std::string& expr)
{
    std::string err("Error char not recognized: ");

    for (std::string::const_iterator it = expr.begin(); it != expr.end(); ++it)
    {
        if (*it == ' ')
            continue;
        if (std::isdigit(*it))
        {
            operands.push(std::atoi(it.base()));
        }
        else if (std::strchr("+-*/", *it))
            executeCalc(*it);
        else
        {
            err.push_back(*it);
            throw std::runtime_error(err);
        }
    }
    if (operands.size() != 1)
        throw std::runtime_error("Error, more operands than operations");
    std::cout << operands.top() << "\n";
}