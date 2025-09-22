#ifndef RPN_HPP
#define RPN_HPP

#include <set>
#include <cctype>
#include <iostream>
#include <string>
#include <sstream>
#define MAX_STACK_SIZE 100000

class RPN {
    public:
        RPN();
        RPN(const RPN &other);
        RPN &operator=(const RPN &other);
        ~RPN();

        void execute(const std::string &input);
    private:
        int myStoi(const std::string &str);
        std::set<char> _operators;
        
};

#endif