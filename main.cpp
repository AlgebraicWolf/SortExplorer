#include <iostream>

#include "sortstat.hpp"
#include "sorts.hpp"

int main() {
    int arr0[] = {5, 6, 2, 1, 4, 3, 8, 0, 9};
    SortingStat<int> arr[9];
    for(int i = 0; i < 9; i++) {
        arr[i] = arr0[i];
    }
    SortingStat<int>::resetCounters();
    QuickSort<int> b;
    b(arr0, 9);

    for(int i = 0; i < 9; i++)
        std::cout << arr0[i] << " ";

    std::cout << "\nAssignments: " << SortingStat<int>::getAssignmentCount() << "\nComparisons: " << SortingStat<int>::getComparisonCount() << "\n";
}