/*
7_2. LSD для long long.
Дан массив неотрицательных целых 64-битных чисел. 
Количество чисел не больше 106. 
Отсортировать массив методом поразрядной сортировки LSD по байтам.
*/
#include <iostream>

template <class T, class TGetKey>
void  LSD(T* arr, int n, TGetKey getKey = TGetKey()) {
    int max = getKey(arr[0]);
    for (int i = 1; i < n; ++i) {
        max = std::max(max, getKey(arr[i]));
    }
    int* c = new int[max + 1];
    for (int i = 0; i < max + 1; i++) {
        c[i] = 0;
    }

    for (int i = 0; i < n; ++i) {
        ++c[getKey(arr[i])];
    }

    for (int i = 1; i < max + 1; ++i) {
        c[i] += c[i - 1];
    }

    T* tempArr = new T[n];
    for (int i = n - 1; i >= 0; --i) {
        tempArr[--c[getKey(arr[i])]] = arr[i];
    }    
    
    delete[] c;

    for (int i = 0; i < n; i++) {
        arr[i] = tempArr[i];
    }
    delete[] tempArr;
}

int main()
{
    int n = 0;
    std::cin >> n;
    unsigned long long * arr = new unsigned long long[n];
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    for (int shift = 0; shift < 64; shift += 8) {
        LSD(arr, n, [shift](unsigned long long value) { return static_cast<int>((value >> shift) & 255); });
    }


    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }

    delete[] arr;
    return 0;
}

//Вычислительная сложность: O(8(n+k))
//Затраты по памяти: n * 64бита