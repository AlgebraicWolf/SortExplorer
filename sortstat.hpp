#include <utility>

template <typename T>
class SortingStat {
   private:
    static unsigned long long assignmentCount;
    static unsigned long long comparisonCount;
    T elem;

   public:
    SortingStat() : elem() {}
    
    SortingStat(T elem) : elem(elem) {}

    SortingStat(const SortingStat& other): elem(other.elem) {
        ++assignmentCount;
    }

    SortingStat(SortingStat&& other) : elem(std::move(other.elem)) {
        ++assignmentCount;
    } 

    SortingStat& operator=(const SortingStat& other) {
        elem = other.elem;
        ++assignmentCount;

        return *this;
    }

    SortingStat& operator=(SortingStat&& other) {
        elem = std::move(other.elem);
        ++assignmentCount;

        return *this;
    }

    static void resetCounters() {
        assignmentCount = 0;
        comparisonCount = 0;
    }

    static unsigned long long getAssignmentCount() {
        return assignmentCount;
    }

    static unsigned long long getComparisonCount() {
        return comparisonCount;
    }

    friend auto operator<=>(const SortingStat& lhs, const SortingStat& rhs) {
        ++comparisonCount;
        return lhs.elem <=> lhs.elem;
    }

    friend bool operator==(const SortingStat& lhs, const SortingStat& rhs) {
        ++comparisonCount;
        return lhs.elem == rhs.elem;
    }
};

template<typename T> unsigned long long SortingStat<T>::assignmentCount;
template<typename T> unsigned long long SortingStat<T>::comparisonCount;

