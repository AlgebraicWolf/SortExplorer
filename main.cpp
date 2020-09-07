#include "sortstat.hpp"
#include <iostream>

int main() {
    SortingStat<int> a = 5;
    SortingStat<int> b = 10;

    a < b;
    a > b;
    a <= b;
    a >= b;
    a == b; 
    a != b;

    std::cout << "Assignments: " << SortingStat<int>::getAssignmentCount() << "\nComparisons: " << SortingStat<int>::getComparisonCount() << '\n';
}