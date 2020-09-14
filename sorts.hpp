#include <utility>
#include "abstractSort.hpp"

template <typename T>
class BubbleSort : public AnnotatedSort<T> {
   public:
    const char *getName() {
        return "Bubble Sort";
    }

    sf::Color getPrimaryColor() {
        return sf::Color(168, 255, 151);
    }

    sf::Color getSecondaryColor() {
        return sf::Color(90, 133, 79);
    }

    void operator()(T *arr, size_t size) {
        using std::swap;
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < size - i; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
};

template <typename T>
class InsertionSort : public AnnotatedSort<T> {
   public:
    const char *getName() {
        return "Insertion sort";
    }

    sf::Color getPrimaryColor() {
        return sf::Color(255, 151, 168);
    }

    sf::Color getSecondaryColor() {
        return sf::Color(133, 79, 90);
    }

    void operator()(T *arr, size_t size) {
        T key;
        int j = 0;
        for (size_t i = 0; i < size; i++) {
            key = arr[i];
            j = i - 1;

            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }
};

template <typename T>
class SelectionSort : public AnnotatedSort<T> {
   public:
    const char *getName() {
        return "Selection sort";
    }

    sf::Color getPrimaryColor() {
        return sf::Color(255, 194, 151);
    }

    sf::Color getSecondaryColor() {
        return sf::Color(133, 100, 79);
    }

    void operator()(T *arr, size_t size) {
        using std::swap;

        int min_index = 0;
        for (int i = 0; i < size - 1; i++) {
            min_index = i;

            for (int j = i + 1; j < size; j++) {
                if (arr[j] < arr[min_index]) {
                    min_index = j;
                }
            }

            swap(arr[i], arr[min_index]);
        }
    }
};

template <typename T>
class QuickSort : public AnnotatedSort<T> {
   public:
    const char *getName() {
        return "QuickSort";
    }

    sf::Color getPrimaryColor() {
        return sf::Color(151, 186, 255);
    }

    sf::Color getSecondaryColor() {
        return sf::Color(79, 100, 133);
    }

    void operator()(T *arr, size_t size) {
        quickSort(arr, 0, size - 1);
    }
    

   private:
    size_t partition(T *arr, size_t begin, size_t end) {
        using std::swap;
        
        T pivot = arr[end];
        
        size_t i = begin;

        for(size_t j = begin; j < end; j++) {
            if(arr[j] < pivot) {
                swap(arr[i], arr[j]);
                i++;
            }
        }
        swap(arr[i], arr[end]);
        return i;
    }

    void quickSort(T *arr, size_t begin, size_t end) {
        if (begin < end) {
            int partitionIndex = partition(arr, begin, end);

            if(partitionIndex > 0)
                quickSort(arr, begin, partitionIndex - 1);
            
            quickSort(arr, partitionIndex + 1, end); 
        }
    }
};

// template <typename T>
// class MergeSort : public AnnotatedSort<T> {
//     public:
//     const char *getName() { 
//         return "Merge Sort";
//     }

//     void operator()(T* arr, size_t size) {

//     }

//     private:
//     void mergeSort(T *arr, size_t size) {

//     }

//     T* merge()

// };