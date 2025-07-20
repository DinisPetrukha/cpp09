#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : _vector() {};

PmergeMe::PmergeMe(const PmergeMe& other) {
	*this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	if (this != &other)
		this->_vector = other._vector;
	return *this;
}

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(char *argv[]) : _vector(argv_to_vector(argv)) {};

std::vector<int> PmergeMe::argv_to_vector(char *argv[]) {
	std::vector<int> vec;
	for (int i = 1; argv[i]; ++i) {
		vec.push_back(std::atoi(argv[i]));
	}
	return vec;
}

void PmergeMe::print_vector() const{
	for (size_t i = 0; i < _vector.size(); ++i) {
		std::cout << _vector[i];
		if (i < _vector.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;
}
