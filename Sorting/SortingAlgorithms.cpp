#include <iostream>

namespace Sorting
{

    void SortingAlgorithms::bubble_sort(int *arr, int N)
    {
        for (int i = 0; i < N - 1; i++)
        {
            for (int j = 0; j < N - i - 1; j++)
            {
                if (arr[j + 1] < arr[j])
                    std::swap(arr[j], arr[j + 1]);
            }
        }
    }

    void SortingAlgorithms::counting_sort(int *arr, int size)
    {
        int *output = new int[size];
        int count[256] = {};
        int i;

        for (i = 0; i < size; ++i)
        {
            ++count[arr[i]];
        }

        for (i = 1; i <= 256; ++i)
        {
            count[i] += count[i - 1];
        }

        for (i = 0; i < size; ++i)
        {
            output[count[arr[i]] - 1] = arr[i];
            --count[arr[i]];
        }

        for (i = 0; i < size; ++i)
        {
            arr[i] = output[i];
        }
    }

    void SortingAlgorithms::heapify(int *arr, int N, int i)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < N && arr[left] > arr[largest])
            largest = left;
        if (right < N && arr[right] > arr[largest])
            largest = right;

        if (largest != i)
        {
            std::swap(arr[i], arr[largest]);
            heapify(arr, N, largest);
        }
    }

    void SortingAlgorithms::heap_sort(int *arr, int N)
    {
        for (int i = N / 2 - 1; i >= 0; i--)
        {
            heapify(arr, N, i);
        }

        for (int i = N - 1; i > 0; i--)
        {
            std::swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }

    void SortingAlgorithms::insertion_sort(int *arr, int size)
    {
        for (int i = 1; i < size; i++)
        {
            int value = arr[i];
            int j = i - 1;

            while (j >= 0 && arr[j] > value)
            {
                arr[j + 1] = arr[j];
                j -= 1;
            }
            arr[j + 1] = value;
        }
    }

    void SortingAlgorithms::merge(int *arr, int left, int mid, int right)
    {
        int sub1 = mid - left + 1;
        int sub2 = right - mid;

        int *leftArr = new int[sub1];
        int *rightArr = new int[sub2];

        for (int i = 0; i < sub1; i++)
            leftArr[i] = arr[left + i];
        for (int i = 0; i < sub2; i++)
            rightArr[i] = arr[mid + 1 + i];

        int indexOfSub1 = 0, indexOfSub2 = 0, indexOfMerged = left;

        while (indexOfSub1 < sub1 && indexOfSub2 < sub2)
        {
            if (leftArr[indexOfSub1] <= rightArr[indexOfSub2])
            {
                arr[indexOfMerged] = leftArr[indexOfSub1];
                indexOfSub1++;
            }
            else
            {
                arr[indexOfMerged] = rightArr[indexOfSub2];
                indexOfSub2++;
            }
            indexOfMerged++;
        }

        while (indexOfSub1 < sub1)
        {
            arr[indexOfMerged] = leftArr[indexOfSub1];
            indexOfSub1++;
            indexOfMerged++;
        }

        while (indexOfSub2 < sub2)
        {
            arr[indexOfMerged] = rightArr[indexOfSub2];
            indexOfSub2++;
            indexOfMerged++;
        }

        delete[] leftArr;
        delete[] rightArr;
    }

    void SortingAlgorithms::merge_sort(int *arr, int left, int right)
    {
        if (right > left)
        {
            int mid = left + (right - left) / 2;
            merge_sort(arr, left, mid);
            merge_sort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

    int SortingAlgorithms::partition(int *arr, int low, int high)
    {
        int pivot = arr[high];

        int i = low - 1;

        for (int j = low; j <= high - 1; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }

        std::swap(arr[i + 1], arr[high]);
        return (i + 1);
    }

    void SortingAlgorithms::quick_sort(int *arr, int low, int high)
    {
        if (low < high)
        {
            int pi = partition(arr, low, high);
            quick_sort(arr, low, pi - 1);
            quick_sort(arr, pi + 1, high);
        }
    }

    void SortingAlgorithms::selection_sort(int *arr, int N)
    {
        for (int i = 0; i < N - 1; i++)
        {
            int min_pos = i;
            for (int j = i + 1; j < N; j++)
            {
                if (arr[j] < arr[min_pos])
                    min_pos = j;
            }

            if (min_pos != i)
                std::swap(arr[i], arr[min_pos]);
        }
    }

} // namespace Sorting
