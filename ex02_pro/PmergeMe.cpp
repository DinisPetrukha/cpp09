#include "PmergeMe.hpp"

// -- | -- | -- | -- | -- | ORTHODOX | -- | -- | -- | -- | -- |

PmergeMe::PmergeMe() : unsortedValues() {}

PmergeMe::PmergeMe(int argc, char **argv) {
	this->unsortedValues = parseInput(argc, argv);
}

PmergeMe::PmergeMe(const PmergeMe& src) : unsortedValues(src.unsortedValues) {
}

PmergeMe::~PmergeMe() {}

PmergeMe& PmergeMe::operator=(const PmergeMe& src) {
	if (this != &src) {
		this->unsortedValues = src.unsortedValues;
	}
	return *this;
}

// -- | -- | -- | -- | -- | PARSING | -- | -- | -- | -- | -- |

std::string PmergeMe::trimInput(const std::string& input) const {
	if (input.empty()) {
		throw std::invalid_argument("empty argument '" + input + "'.");
	}
	const char* whitespace = " \t\v\r\n";
	std::size_t start = input.find_first_not_of(whitespace);
	if (start == std::string::npos) {
		throw std::invalid_argument("invalid argument '" + input + "'.");
	}
	std::size_t end = input.find_last_not_of(whitespace);
	return input.substr(start, end - start + 1);
}

std::vector<int> PmergeMe::parseInput(int argc, char** argv) {
	std::vector<int> argVector;
	for (int i = 1; i < argc; ++ i) {
		std::string value = trimInput(argv[i]);
		char* end = NULL;
		errno = 0;
		long result = std::strtol(value.c_str(), &end, 10);
		if (end == value.c_str() || *end != '\0') {
			throw std::invalid_argument("invalid argument '" + value + "'.");
		}
		if (errno == ERANGE || result < 0 || result > std::numeric_limits<int>::max()) {
			throw std::invalid_argument("out of bounds int '" + value + "'.");
		}
		argVector.push_back(static_cast<int>(result));
	}
	return argVector;
}

// -- | -- | -- | -- | -- | MAIN FUNC | -- | -- | -- | -- | -- |

void PmergeMe::sort() {
	const std::vector<int>& unsortedVector = this->unsortedValues;
	std::vector<int> sortVector(unsortedVector);
	std::deque<int> sortDeque(unsortedVector.begin(), unsortedVector.end());
	std::clock_t start, end;

	std::cout << "\n---VECTOR---\n❌Unsorted Sequence:\n" << printContainer(unsortedVector) << "\n";

	start = clock();
	fordJohnsonVector(sortVector, 1);
	end = clock();

	std::cout << "\n✅Sorted Sequence:\n" << printContainer(sortVector) << "\n";
	logTimeTaken(start, end, "vector", sortVector.size());

	std::cout << "\n\n-----------------------------------------------------------\n" << std::endl;

	std::cout << "\n---DEQUE---\n❌Unsorted Sequence:\n" << printContainer(unsortedVector) << "\n";

	start = clock();
	fordJohnsonSortDeque(sortDeque, 1);
	end = clock();

	std::cout << "\n✅Sorted Sequence:\n" << printContainer(sortDeque) << "\n";

	logTimeTaken(start, end, "deque", sortDeque.size());

}

// -- | -- | -- | -- | -- | VECTOR | -- | -- | -- | -- | -- |

void PmergeMe::fordJohnsonVector(std::vector<int>& vector, size_t elementSize) {
	size_t totalSize = vector.size();
	size_t elementCount = totalSize / elementSize;

	if (elementCount < 2)
		return ;

	size_t pairSize = elementSize * 2;

	for (size_t i = 0; i + pairSize <= totalSize; i += pairSize) {
		std::vector<int>::iterator leftGroupIt = vector.begin() + i;
		std::vector<int>::iterator rightGroupIt = leftGroupIt + elementSize;
		size_t indexToCompare = elementSize - 1;

		if (*(leftGroupIt + indexToCompare) > *(rightGroupIt + indexToCompare)) {
			std::swap_ranges(leftGroupIt, rightGroupIt, rightGroupIt);
		}
	}

	fordJohnsonVector(vector, pairSize);

	std::vector<int> main;
	std::vector<int> pend;

	fillMainAndPendVector(vector, main, pend, elementSize, elementCount);

	// Generate reverse JacobsthalIndices
	size_t elementsInPend = pend.size() / elementSize;
	std::vector<size_t> jacobOrder = generateJacobsthalIndicesVector(elementsInPend);

	// Creating a vector size elements of "pend" of false booleans
	std::vector<bool> inserted(elementsInPend, false); // inserted = [false, fasle, false,...]

	// Insert jacobsthal indices from pend into main
	for (size_t i = 0; i < jacobOrder.size(); ++i) {
		size_t index = jacobOrder[i];
		if (index >= elementsInPend || inserted[index]) // jacobsthal index out of range OR aleady inserted
			continue ;

		// Jacob Index * element size | this way we get the start of the range corresponding jacob index
		size_t start = index * elementSize;
		size_t end = start + elementSize;

		std::vector<int> group(pend.begin() + start, pend.begin() + end);
		// getting the main value of pend
		int key = group[elementSize - 1];

		std::vector<int>::iterator insertPos = binarySearchInsertPosVector(main, key, elementSize);
		main.insert(insertPos, group.begin(), group.end());
		inserted[index] = true;

		// Insert main into vector.
		vector.assign(main.begin(), main.end());
	}

	// Inser remaining pend in reverse order
	int elemInPend = static_cast<int>(elementsInPend);
	for (int i = elemInPend - 1; i >= 0; --i) {
		if (inserted[i])
			continue;

		size_t start = i * elementSize;
		size_t end = start + elementSize;
		std::vector<int> group(pend.begin() + start, pend.begin() + end);
		int key = group[elementSize - 1];

		std::vector<int>::iterator insertPos = binarySearchInsertPosVector(main, key, elementSize);
		main.insert(insertPos, group.begin(), group.end());
	}
	vector.assign(main.begin(), main.end());
}

// Pesquisa de arvore binaria ate encontrar o index ideal.
// Retorna a posicao index
std::vector<int>::iterator PmergeMe::binarySearchInsertPosVector(
	std::vector<int>& main,
	int key,
	size_t elementSize
) const {

	size_t groupCount = main.size() / elementSize;
	size_t leftGroup = 0;
	size_t rightGroup = groupCount;

	while (leftGroup < rightGroup) {
		size_t midGroup = (leftGroup + rightGroup) / 2;
		size_t midGroupOffset = midGroup * elementSize;
		size_t midKeyIndex = midGroupOffset + elementSize - 1;

		int midVal = *(main.begin() + midKeyIndex);

		if (midVal < key)
			leftGroup = midGroup + 1;
		else
			rightGroup = midGroup;
	}

	size_t insertOffset = leftGroup * elementSize;
	return main.begin() + insertOffset;
}

void PmergeMe::fillMainAndPendVector(
	const std::vector<int>& vector,
	std::vector<int>& main,
	std::vector<int>& pend,
	size_t elementSize,
	size_t elementCount
) const {

	// Inserts b1 and a1 in the main
	size_t endA1 = 2 * elementSize;
	main.insert(main.end(), vector.begin(), vector.begin() + endA1);

	// Inserts rest of a's in the main (os "vencedores" vao para main)
	for (size_t i = 2; i < elementCount; ++i) {
		if (i % 2 == 1) {
			size_t start = i * elementSize;
			size_t end = start + elementSize;
			main.insert(main.end(), vector.begin() + start, vector.begin() + end);
		}
	}

	// Insert rest of b's in the pend (os "perdedores" vao para pend)
	for (size_t i = 2; i < elementCount; ++i) {
		if (i % 2 == 0) {
			size_t start = i * elementSize;
			size_t end = start + elementSize;
			pend.insert(pend.end(), vector.begin() + start, vector.begin() + end);
		}
	}

	// Insert leftovers into main
	size_t leftoverStart = elementCount * elementSize;
	if (leftoverStart < vector.size()) {
		main.insert(main.end(), vector.begin() + leftoverStart, vector.end());
	}
}

std::vector<size_t> PmergeMe::generateJacobsthalIndicesVector(size_t nbrOfElements) const {
	std::vector<size_t> jacobIndices;
	size_t j1 = 0;
	size_t j2 = 1;
	while (j2 < nbrOfElements) {
		jacobIndices.push_back(j2);
		size_t next = j2 + 2 * j1;
		j1 = j2;
		j2 = next;
	}
	std::reverse(jacobIndices.begin(), jacobIndices.end());
	// std::cout << "[DEBUG]: Jacobindices\n" << printContainer(jacobIndices);
	return jacobIndices;
}

// -- | -- | -- | -- | -- | DEQUE | -- | -- | -- | -- | -- |

std::deque<size_t> PmergeMe::generateJacobsthalIndicesDeque(size_t nbrOfElements) const {
	std::deque<size_t> jacobIndices;
	size_t j1 = 0;
	size_t j2 = 1;
	while (j2 < nbrOfElements) {
		jacobIndices.push_back(j2);
		size_t next = j2 + 2 * j1;
		j1 = j2;
		j2 = next;
	}
	std::reverse(jacobIndices.begin(), jacobIndices.end());
	return jacobIndices;
}

std::deque<int>::iterator PmergeMe::binarySearchInsertPosDeque(
	std::deque<int>& main,
	int key,
	size_t elementSize
) const {
	size_t groupCount = main.size() / elementSize;
	size_t leftGroup = 0;
	size_t rightGroup = groupCount;

	while (leftGroup < rightGroup) {
		size_t midGroup = (leftGroup + rightGroup) / 2;
		size_t midGroupOffset = midGroup * elementSize;
		size_t midKeyIndex = midGroupOffset + elementSize - 1;

		int midVal = *(main.begin() + midKeyIndex);

		if (midVal < key) {
			leftGroup = midGroup + 1;
		} else {
			rightGroup = midGroup;
		}
	}

	size_t insertOffset = leftGroup * elementSize;
	return main.begin() + insertOffset;
}

void PmergeMe::fillMainAndPendDeque(
	const std::deque<int>& deque,
	std::deque<int>& main,
	std::deque<int>& pend,
	size_t elementSize,
	size_t elementCount
) const {
	// Insert b1 and a1 in main
	size_t endOfA1 = 2 * elementSize;
	main.insert(main.end(), deque.begin(), deque.begin() + endOfA1);

	// Insert rest of a's in main
	for (size_t i = 2; i < elementCount; ++i) {
		// a elements are always odd
		if (i % 2 == 1) {
			size_t start = i * elementSize;
			size_t end = start + elementSize;
			main.insert(main.end(), deque.begin() + start, deque.begin() + end);
		}
	}

	// Insert b's from b2 onward in pend
	for (size_t i = 2; i < elementCount; ++i) {
		// b elements are always even
		if (i % 2 == 0) {
			size_t start = i * elementSize;
			size_t end = start + elementSize;
			pend.insert(pend.end(), deque.begin() + start, deque.begin() + end);
		}
	}

	// Insert leftovers in main
	size_t leftoverStart = elementCount * elementSize;
	if (leftoverStart < deque.size()) {
		main.insert(main.end(), deque.begin() + leftoverStart, deque.end());
	}
}

void PmergeMe::fordJohnsonSortDeque(std::deque<int>& deque, size_t elementSize) {
	size_t totalSize = deque.size();
	size_t elementCount = totalSize / elementSize;

	if (elementCount < 2){
		return ;
	}

	size_t pairSize = elementSize * 2;

	for (size_t i = 0; i + pairSize <= totalSize; i += pairSize) {
		std::deque<int>::iterator leftGroupIt = deque.begin() + i;
		std::deque<int>::iterator rightGroupIt = leftGroupIt + elementSize;
		size_t indexToCompare = elementSize - 1;
		if (*(leftGroupIt + indexToCompare) > *(rightGroupIt + indexToCompare)) {
			std::swap_ranges(leftGroupIt, rightGroupIt, rightGroupIt);
		}
	}

	fordJohnsonSortDeque(deque, pairSize);

	std::deque<int> main;
	std::deque<int> pend;

	fillMainAndPendDeque(deque, main, pend, elementSize, elementCount);

	// Generate reverse JacobsthalIndices
	size_t elementsInPend = pend.size() / elementSize;
	std::deque<size_t> insertOrder = generateJacobsthalIndicesDeque(elementsInPend);

	// Keep track of what indices were inserted
	std::deque<bool> inserted(elementsInPend, false);

	// Insert jacobsthal indices from pend into main
	for (size_t i = 0; i < insertOrder.size(); ++i) {
		size_t index = insertOrder[i];
		if (index >= elementsInPend || inserted[index]) {
			continue ;
		}

		size_t start = index * elementSize;
		size_t end = start + elementSize;

		std::deque<int> group(pend.begin() + start, pend.begin() + end);
		int key = group[elementSize - 1];

		std::deque<int>::iterator insertPos = binarySearchInsertPosDeque(main, key, elementSize);
		main.insert(insertPos, group.begin(), group.end());
		inserted[index] = true;
	}

	// Insert remaining pend in reverse order
	int elemInPend = static_cast<int>(elementsInPend);
	for (int i = elemInPend - 1; i >= 0; --i) {
		if (inserted[i]) {
			continue;
		}

		size_t start = i * elementSize;
		size_t end = start + elementSize;
		std::deque<int> group(pend.begin() + start, pend.begin() + end);
		int key = group[elementSize - 1];

		std::deque<int>::iterator insertPos = binarySearchInsertPosDeque(main, key, elementSize);
		main.insert(insertPos, group.begin(), group.end());
	}

	// Insert main into deque.
	deque.assign(main.begin(), main.end());
}

// -- | -- | -- | -- | -- | UTILS | -- | -- | -- | -- | -- |

void PmergeMe::logTimeTaken(
	const std::clock_t& start,
	const std::clock_t& end,
	const std::string& containerLabel,
	int containerSize
) const {
	const long MICROSECONDS_IN_SECOND = 1000000L;

	double time_taken_sec = static_cast<double>(end - start) / CLOCKS_PER_SEC;
	long time_taken_us = static_cast<long>(time_taken_sec * MICROSECONDS_IN_SECOND);

	std::cout << "Time to process a range of " << containerSize << " elements"
	" with std::" << containerLabel << " : " << time_taken_us << " us\n";
}