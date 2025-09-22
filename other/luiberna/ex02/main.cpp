#include "PmergeMe.hpp"

bool isValidArg(char *arg, int &n) {
    std::stringstream iss(arg);
    if (!(iss >> n) || !(iss.eof()) || n < 0)
        return false;
    return true;
}

void print_vect(const std::vector<int> &v) {
    for(size_t i = 0; i < v.size(); i++)
        std::cout << v[i] << " ";
    std::cout << std::endl;
}

void print_list(const std::list<int> &l) {
    for(std::list<int>::const_iterator i = l.begin(); i != l.end(); ++i)
        std::cout << *i << " ";
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Error: Invalide number of arguments" << std::endl;
        return 1;
    }
    std::vector<int> vect;
    for (int i = 1; i < argc; i++) {
        int n;
        if (!isValidArg(argv[i], n)) {
            std::cout << "Error: argument '" << argv[i] << "' not a positive integer" << std::endl;
            return 1;
        }
        vect.push_back(n);
    }
    for(int i = 1; i < argc; i++) {
        int n;
        if (!isValidArg(argv[i], n)) {
            std::cout << "Error: argument '" << argv[i] << "' not a positive integer" << std::endl;
            return 1;
        }
    }
    PmergeMe Pmerge;
    Pmerge.fillContainers(argv);
    Pmerge.sort();
    return 0;
}