#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stack>

class RPN
{
    private:
        std::stack<int> operands;
        void            executeCalc(char op);
        
    public:
        RPN();
        RPN(const RPN& );
        RPN& operator=(const RPN& );
        ~RPN();

        void evaluateExpr( const std::string& expr);
};

#endif // RPN.HPP