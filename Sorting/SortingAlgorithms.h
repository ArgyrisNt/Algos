#include <iostream>

namespace Sorting
{

    class SortingAlgorithms
    {
    public:
        void counting_sort(int *arr, int size);
        void bubble_sort(int *arr, int N);
        void heap_sort(int *arr, int N);
        void insertion_sort(int *arr, int size);
        void merge_sort(int *arr, int left, int right);
        void quick_sort(int *arr, int low, int high);
        void selection_sort(int *arr, int N);
    
    private:
        void heapify(int *arr, int N, int i);
        void merge(int *arr, int left, int mid, int right);
        int partition(int *arr, int low, int high);
    };

} // namespace Sorting

#include "SortingAlgorithms.cpp"