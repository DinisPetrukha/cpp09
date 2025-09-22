#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other) {
    this->_list = other._list;
    this->_vec = other._vec;
}

PmergeMe &PmergeMe::operator=(PmergeMe const &other) {
    if (this != &other) {
        this->_list = other._list;
        this->_vec = other._vec;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

//-----------------------Printers-------------------------

void PmergeMe::printBefore() {
    std::cout << "Before(V): ";
    for (std::vector<int>::iterator it = _vec.begin(); it != _vec.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::printBeforeList() {
    std::cout << "Before(L): ";
    for (std::list<int>::iterator it = _list.begin(); it != _list.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::printAfter() {
    std::cout << "After(V): ";
    for (std::vector<int>::iterator it = _vec.begin(); it != _vec.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::printAfterList() {
    std::cout << "After(L): ";
    for (std::list<int>::iterator it = _list.begin(); it != _list.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::printInfo(const std::string &str, double time) {
    std::cout << "Time to process a range of " << _vec.size() << " elements with " << str << " : ";
    std::cout << std::fixed << std::setprecision(6) << time << " us" << std::endl;
}

//------------------------Filling-------------------------

void PmergeMe::fillContainers(char **argv) {
    for (int i = 1; argv[i]; i++) {
        int n;
        std::stringstream iss(argv[i]);
        if (!(iss >> n) || !(iss.eof()) || n < 0) {
            std::cout << "Error: argument '" << argv[i] << "' not a positive integer" << std::endl;
            return;
        }
        _vec.push_back(n);
        _list.push_back(n);
    }
}

//-----------------------Sorting-------------------------

void PmergeMe::sort() {
    std::clock_t start, end;
    double vec_time, lst_time;
    (void)vec_time;
    (void)lst_time;

    //printBeforeList();
    printBefore();

    start = clock();
    mergeSortList(_list.begin(), _list.end());
    insertSortList();
    end = clock();
    lst_time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;

    start = clock();
    mergeSortVector(0, _vec.size());
    insertionSortVector();
    end = clock();
    vec_time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;

    //printAfterList();
    printAfter();
    printInfo("std::list", lst_time);
    printInfo("std::vector", vec_time);
}

//-----------------------List_Algorithms-------------------------
void PmergeMe::mergeSortList(std::list<int>::iterator begin, std::list<int>::iterator end) {
    size_t size = std::distance(begin, end);
    if (size <= 2) {
        if (size == 2) {
            std::list<int>::iterator next = begin;
            ++next;
            if (*begin > *next)
                std::iter_swap(begin, next);
        }
        return;
    }
    std::list<int>::iterator mid = begin;
    std::advance(mid, size / 2);
    mergeSortList(begin, mid);
    mergeSortList(mid, end);
    mergeList(begin, mid, end);
}

void PmergeMe::mergeList(std::list<int>::iterator begin, std::list<int>::iterator mid, std::list<int>::iterator end) {
    std::list<int> temp;
    std::list<int>::iterator left = begin;
    std::list<int>::iterator right = mid;
    while (left != mid && right != end) {
        if (*left <= *right) {
            temp.push_back(*left);
            ++left;
        }
        else {
            temp.push_back(*right);
            ++right;
        }
    }
    while (left != mid) {
        temp.push_back(*left);
        ++left;
    }
    while (right != end) {
        temp.push_back(*right);
        ++right;
    }
    std::copy(temp.begin(), temp.end(), begin);
}

std::list<int>::iterator PmergeMe::insertionList(std::list<int>::iterator begin, std::list<int>::iterator end, int value) {
    if (begin == end)
        return begin;
    std::list<int>::iterator mid = begin;
    std::advance(mid, std::distance(begin, end) / 2);
    if (value < *mid)
        return insertionList(begin, mid, value);
    else if (value > *mid)
        return insertionList(++mid, end, value);
    else
        return mid;
}

void PmergeMe::insertSortList() {
    std::list<int> sorted;
    std::list<int>::iterator it = _list.begin();
    for (size_t i = 0; i < _list.size(); ++i) {
        if (it == --_list.end() || i % 2)
            sorted.insert(insertionList(sorted.begin(), sorted.end(), *it), *it);
        else
            sorted.push_back(*it);
        ++it;
    }
    _list = sorted;
}

//-----------------------Vector_Algorithms-------------------------

void PmergeMe::mergeSortVector(int begin, int end) {
    int size = end - begin;
    if (size <= 2) {
        if (size == 2 && _vec[begin] > _vec[end - 1])
            std::swap(_vec[begin], _vec[end - 1]);
        return;
    }
    int mid = begin + size / 2;
    mergeSortVector(begin, mid);
    mergeSortVector(mid, end);
    mergeVector(begin, mid, end);
}

void PmergeMe::mergeVector(int begin, int mid, int end) {
    std::vector<int> temp;
    int left = begin;
    int right = mid;
    while (left < mid && right < end) {
        if (_vec[left] < _vec[right]) {
            temp.push_back(_vec[left]);
            left++;
        }
        else {
            temp.push_back(_vec[right]);
            right++;
        }
    }
    while (left < mid) {
        temp.push_back(_vec[left]);
        left++;
    }
    while (right < end) {
        temp.push_back(_vec[right]);
        right++;
    }
    std::copy(temp.begin(), temp.end(), _vec.begin() + begin);
}

int PmergeMe::insertionVector(int begin, int end, int numb, std::vector<int> &temp) {
    if (begin == end)
        return begin;
    int mid = begin + (end - begin) / 2;
    if (numb < temp[mid])
        return (insertionVector(begin, mid, numb, temp));
    else if (numb > temp[mid])
        return (insertionVector(mid + 1, end, numb, temp));
    else
        return mid;
}

void PmergeMe::insertionSortVector() {
    std::vector<int> sorted;
    for (size_t i = 0; i < _vec.size(); i++) {
        if (i + 1 == _vec.size() || i % 2)
            sorted.insert(sorted.begin() + insertionVector(0, sorted.size(), _vec[i], sorted), _vec[i]);
        else
            sorted.push_back(_vec[i]);
    }
    _vec = sorted;
}