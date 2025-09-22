#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <valarray>
#include <list>
#include <algorithm>
#include <vector>


#define USECS_IN_SEC 1000000
#define MSECS_IN_SEC 1000

struct VecChunk
{
    std::vector<int>::iterator  begin;
    std::vector<int>::iterator  end;
};

class VecSorter
{
private:
    std::vector<int>    the_numbers;
    std::vector<VecChunk>  the_pend;
    std::vector<VecChunk>  the_main_chain;

    size_t              jacob_index;

    void    init_the_pend(size_t chunk_size, size_t chunks);
    void    merge_the_pend();
    void    insert_chunk(size_t chunks_to_send, size_t to_send_index, size_t search_area);
    void    copy_the_main_chain();
    size_t  get_search_area() const;
    size_t  get_index_to_send(size_t curr_jacob) const;

    std::vector<VecChunk>::iterator binary_search_pos(int max, size_t chunk_index);


public:
    VecSorter();
    VecSorter(char* str);
    VecSorter(char** argv);
    VecSorter(const VecSorter& sorter);
    VecSorter& operator=(const VecSorter& other);

    void sort(size_t depth);
    void print_numbers() const;

    const std::vector<int>&   get_numbers() const;

    std::vector<int>::const_iterator numbers_begin() const;
    std::vector<int>::const_iterator numbers_end() const;
    std::vector<VecChunk>::const_iterator pend_begin() const;
    std::vector<VecChunk>::const_iterator pend_end() const;
    std::vector<VecChunk>::const_iterator chain_begin() const;
    std::vector<VecChunk>::const_iterator chain_end() const;
};

std::ostream& operator<<(std::ostream& os, const VecSorter& v);


struct ListChunk
{
    std::list<int>::iterator    begin;
    std::list<int>::iterator    end;
};

class ListSorter
{
private:
    std::list<int>          list;
    std::list<ListChunk>    the_pend;
    std::list<ListChunk>    the_main_chain;
    size_t                  jacob_index;

    void    init_the_pend(size_t chunk_size, size_t pairs);
    void    merge_the_pend();
    void    insert_chunk(size_t chunks_to_send, size_t to_send_index, size_t search_area);
    void    copy_the_main_chain();
    size_t  get_search_area() const;
    size_t  get_index_to_send(size_t curr_jacob) const;

    std::list<ListChunk>::iterator binary_search_pos(int max, size_t chunk_index);
public:
    ListSorter();
    ListSorter(char* str);
    ListSorter(char** argv);
    ListSorter(const ListSorter& sorter);
    ListSorter& operator=(const ListSorter& other);

    void sort(size_t depth);

    int last_elem_at(size_t index) const;
    std::list<ListChunk>::iterator  main_chain_iter(size_t index);

    void    print_numbers() const;
    const std::list<int>&   get_numbers() const;

    std::list<int>::const_iterator list_begin() const;
    std::list<int>::const_iterator list_end() const;
    std::list<ListChunk>::const_iterator pend_begin() const;
    std::list<ListChunk>::const_iterator pend_end() const;
    std::list<ListChunk>::const_iterator chain_begin() const;
    std::list<ListChunk>::const_iterator chain_end() const;


};

std::ostream& operator<<(std::ostream& os, const ListSorter& l);

template<typename T>
void splitArg(T& storage, std::string arg)
{
    std::string val;
    std::size_t pos;

    do
    {
        pos = arg.find(' ');
        val = arg.substr(0, pos);
        if (!val.empty())
        {
            storage.push_back(std::atoi(val.data()));
        }
        arg.erase(0, pos + 1);
    } while (pos != std::string::npos);
}

template<typename T>
int    are_duplicate_present(const T& the_numbers)
{
    int to_compare;
    typename T::const_iterator end = the_numbers.end();

    for (typename T::const_iterator it = the_numbers.begin(); it != end;)
    {
        to_compare = *it;
        ++it;
        for (typename T::const_iterator st = it; st != end; ++st)
        {
            if (to_compare == *st)
                return (to_compare);
        }
    }
    return (-1);
}

template<typename T>
void swap_chunk(T chunk_one_it, T chunk_two_it, size_t chunk_size)
{
    int temp;

    for (size_t i = 0; i < chunk_size; ++i)
    {
        temp = *(chunk_one_it);
        *(chunk_one_it) = *(chunk_two_it);
        *(chunk_two_it) = temp;
        ++chunk_one_it;
        ++chunk_two_it;
    }
}

template<typename T>
void push_back_leftovers(T& storage_a, T& storage_b)
{
    size_t                  storage_a_size;
    size_t                  storage_b_size;
    size_t                  leftovers_num;
    typename T::iterator    b_end;

    storage_a_size = storage_a.size();
    storage_b_size = storage_b.size();
    b_end = storage_b.end();
    leftovers_num = storage_b_size - storage_a_size;
    std::advance(b_end, -leftovers_num);

    for (; leftovers_num > 0; --leftovers_num)
    {
        storage_a.push_back(*(b_end));
        ++b_end;
    }
}

template<typename T>
inline int last_element_of_chunk(T chunk_begin, size_t chunk_size)
{
    std::advance(chunk_begin, chunk_size - 1);
    return (*(chunk_begin));
}

template<typename T>
bool check_if_sorted(const T& storage)
{
    int val;

    for (typename T::const_iterator it = storage.begin();
        it != storage.end();)
    {
        val = *it;
        ++it;
        if (it == storage.end())
        {
            //std::cout << "Sorted\n";
            return (true);
        }
        else if (val > *it)
        {
            std::cout << "Not sorted\n";
            std::cout << "Value : " << val << " greater than " << *it << "\n";
            return (false);
        }
    }
    return (true);
}

#endif // PMERGEME_HPP