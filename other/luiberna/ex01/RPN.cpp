#include "RPN.hpp"

RPN::RPN() {
    _operators.insert('+');
    _operators.insert('-');
    _operators.insert('*');
    _operators.insert('/');
}

RPN::RPN(const RPN &other) {
    *this = other;
}

RPN &RPN::operator=(const RPN &other) {
    if (this != &other) {
    }
    return *this;
}

RPN::~RPN() {}

int RPN::myStoi(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    if (ss.fail() || !ss.eof()) {
        std::cerr << "Error: Invalid integer format in string '" << str << "'." << std::endl;
        throw std::invalid_argument("Invalid integer format");
    }
    return value;
}

void RPN::execute(const std::string &input) {
    std::stringstream ss(input);
    std::string token;
    int stack[MAX_STACK_SIZE];
    int top = -1;

    while (ss >> token) {
        if (token.size() == 1 && _operators.count(token[0])) {
            if (top < 1) {
                std::cerr << "Error: Not enough operands for operation '" << token << "'." << std::endl;
                return;
            }
            int b = stack[top--];
            int a = stack[top--];
            int result;
            switch (token[0]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b == 0) {
                        std::cerr << "Error: Division by zero." << std::endl;
                        return;
                    }
                    result = a / b; break;
                default:
                    std::cerr << "Error: Unknown operator '" << token << "'." << std::endl;
                    return;
            }
            if (top >= MAX_STACK_SIZE - 1) {
                std::cerr << "Error: Stack overflow." << std::endl;
                return;
            }
            stack[++top] = result;
        }
        else {
            try {
                int value = myStoi(token);
                if (value < 0 || value > 9) {
                    std::cerr << "Error: Only digits 0-9 allowed." << std::endl;
                    return;
                }
                if (top >= MAX_STACK_SIZE - 1) {
                    std::cerr << "Error: Stack overflow." << std::endl;
                    return;
                }
                stack[++top] = value;
            }
            catch (const std::invalid_argument &e) {
                std::cerr << "Error: Invalid token '" << token << "'." << std::endl;
                return;
            }
        }
    }
    if (top != 0) {
        std::cerr << "Error: Invalid RPN expression. Stack has " << (top + 1) << " elements left." << std::endl;
        return;
    }
    std::cout << stack[top] << std::endl;
}
