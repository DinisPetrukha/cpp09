#ifndef PMERGEME_HPP
#define PMERGEME_HPP

# include <iostream>
# include <sstream>
# include <algorithm>
# include <vector>
# include <queue>
# include <exception>
# include <cstdlib>
# include <cerrno>
# include <limits>
# include <ctime>

class PmergeMe {
private:
	PmergeMe();
	std::vector<int> unsortedValues;

	std::string trimInput(const std::string& input) const;
	std::vector<int> parseInput(int argc, char** argv);

	// -- | -- | -- | -- | -- | VECTOR | -- | -- | -- | -- | -- |
	// void mergeSortDeque(std::deque<int>& deque) const;

	std::vector<size_t> generateJacobsthalIndicesVector(size_t nbrOfElements) const;
	std::vector<int>::iterator binarySearchInsertPosVector(
		std::vector<int>& main,
		int key,
		size_t elementSize
	) const;

	void fillMainAndPendVector(
			const std::vector<int>& vector,
			std::vector<int>& main,
			std::vector<int>& pend,
			size_t elementSize,
			size_t elementCount
		) const;

	void fordJohnsonVector(std::vector<int>& vector, size_t elementSize);

	// -- | -- | -- | -- | -- | DEQUE | -- | -- | -- | -- | -- |

	std::deque<size_t> generateJacobsthalIndicesDeque(size_t nbrOfElements) const;

	std::deque<int>::iterator binarySearchInsertPosDeque(
		std::deque<int>& main,
		int key,
		size_t elementSize
	) const;

	void fillMainAndPendDeque(
		const std::deque<int>& deque,
		std::deque<int>& main,
		std::deque<int>& pend,
		size_t elementSize,
		size_t elementCount
	) const;

	void fordJohnsonSortDeque(std::deque<int>& deque, size_t elementSize);

	// -- | -- | -- | -- | -- | UTILS | -- | -- | -- | -- | -- |

	void logTimeTaken(
		const std::clock_t& start,
		const std::clock_t& end,
		const std::string& containerLabel,
		int containerSize
	) const;

	template <typename T>
	std::string printContainer(const T& container) const;

public:
	PmergeMe(int argc, char** argv);
	PmergeMe(const PmergeMe& src);
	~PmergeMe();
	PmergeMe& operator=(const PmergeMe& src);

	void sort();
};


template <typename T>
std::string PmergeMe::printContainer(const T& container) const {
	if (container.empty()) {
		std::cout << "bi" << std::endl;
		return "";
	}
	std::ostringstream oss;
	typename T::const_iterator it = container.begin();
	typename T::const_iterator end = container.end();
	for (; it != end; ++it) {
		std::string space = it != container.end() - 1 ? " " : "\n";
		oss << *it << space;
	}
	return oss.str();
}

# endif