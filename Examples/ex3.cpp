// Example of sorting algorithms

#include <iostream>
#include "..\Sorting\SortingAlgorithms.h"

void print(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    int arr[5] = {1, 3, 5, 2, 4};
    int size = sizeof(arr) / sizeof(arr[0]);

    Sorting::SortingAlgorithms sort;
    sort.merge_sort(arr, 0, size);

    print(arr, size);

    return 0;
}