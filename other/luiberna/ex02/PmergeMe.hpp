#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>

class PmergeMe {
    public:
        PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void sort();
        void fillContainers(char **argv);
        void printInfo(const std::string &str, double time);
        void printBefore();
        void printBeforeList();
        void printAfter();
        void printAfterList();
    private:
        std::list<int> _list;
        std::vector<int> _vec;

        void mergeVector(int begin, int mid,int end);
        void mergeSortVector(int begin, int end);
        void insertionSortVector();
        int insertionVector(int begin, int end, int numb, std::vector<int> &temp);

        void mergeList(std::list<int>::iterator begin, std::list<int>::iterator mid, std::list<int>::iterator end);
        void mergeSortList(std::list<int>::iterator begin, std::list<int>::iterator end);
        void insertSortList();
        static std::list<int>::iterator insertionList(std::list<int>::iterator begin, std::list<int>::iterator end, int value);
};

#endif