#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <valarray>
#include <list>
#include <algorithm>
#include <vector>
#include <string>

struct ListChunk
{
    std::list<int>::iterator    begin;
    std::list<int>::iterator    end;
};

class SortList
{
private:
    std::list<int>  _list;
    std::list<ListChunk> _pend;
    std::list<ListChunk> _main_chain;
    size_t _jacob_index;

public:
    SortList();
    SortList(char* argv[], int argc);

    void printList();
    void initPend(const size_t chunk_size, const size_t chunks);
    void mergePend();
    // ListSorter(const ListSorter& sorter);
    // ListSorter& operator=(const ListSorter& other);

    void sort(size_t depth);
    void insertChunk(size_t chunks_to_send, size_t to_send_index, size_t search_area);
    std::list<ListChunk>::iterator binarySearch(int max, size_t search_area);
    std::list<ListChunk>::iterator mainChainIter(size_t index);
    inline  size_t getIndexSend(size_t curr_jacob) const;

    int lastElement(size_t index) const;
    inline size_t getSearchArea() const;
    void copyMainChain();

    // int last_elem_at(size_t index) const;
    // std::list<ListChunk>::iterator  main_chain_iter(size_t index);

    // void    print_numbers() const;
    // const std::list<int>&   get_numbers() const;

    // std::list<int>::const_iterator list_begin() const;
    // std::list<int>::const_iterator list_end() const;
    // std::list<ListChunk>::const_iterator pend_begin() const;
    // std::list<ListChunk>::const_iterator pend_end() const;
    // std::list<ListChunk>::const_iterator chain_begin() const;
    // std::list<ListChunk>::const_iterator chain_end() const;
};

template <typename T>
int chunkLastElement(T chunk_begin, size_t chunk_size) {
    std::advance(chunk_begin, chunk_size - 1);
    return (*chunk_begin);
}

template <typename T>
void chunkSwap(T index_chunk_one, T index_chunk_two, size_t chunk_size) {
    int temp;

    for (size_t i = 0; i < chunk_size; ++i) {
        temp = *index_chunk_one;
        *index_chunk_one = *index_chunk_two;
        *index_chunk_two = temp;

        ++index_chunk_one;
        ++index_chunk_two;
    }
}

template<typename T>
void push_back_leftovers(T& storage_a, T& storage_b)
{
    size_t storage_size_a;
    size_t storage_size_b;
    size_t leftovers_num;
    typename T::iterator b_end;

    storage_size_a = storage_a.size();
    storage_size_b = storage_b.size();
    b_end = storage_b.end();
    leftovers_num = storage_size_b - storage_size_a;
    std::advance(b_end, -leftovers_num);

    for (; leftovers_num > 0; --leftovers_num)
    {
        storage_a.push_back(*(b_end));
        ++b_end;
    }
}

# endif