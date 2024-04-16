/*
Дано множество целых чисел из[0..10 ^ 9] размера n.
Используя алгоритм поиска k - ой порядковой статистики, требуется найти следующие параметры множества :
10 % перцентиль
медиана
90 % перцентиль

Требования : к дополнительной памяти : O(n).
Среднее время работы : O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.

6_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”. 
Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.
*/

#include <iostream>
#include <cassert>

template <class T, class Comparator>
int Partition(T* a, int n, Comparator cmp) {
    if (n <= 1) {
        return 0;
    }
    int randomNumIdx = rand() % n;
    std::swap(a[randomNumIdx], a[n - 1]);

    T pivot = a[n - 1];
    int i = 0;
    for (int j = 0; j < n - 1; j++) {
        if (cmp(a[j], pivot)) {
            std::swap(a[i], a[j]);
            i++;
        }
    }
    std::swap(a[i], a[n - 1]);
    return i;
}

template <class T, class Comparator = std::less<T>>
T FindKStat(T* arr, int n, int k, Comparator cmp = Comparator()) {
    assert(k >= 0 && k < n);
    int l = 0;
    int r = n;
    while (true) {
        int pivotPos = Partition(arr + l, r - l, cmp);
        if (l + pivotPos == k) {
            return arr[k];
        }
        if (l + pivotPos > k) {
            r = l + pivotPos;
        }
        else {
            l = l + pivotPos + 1;
        }
    }
}

template <class T>
class Comparator {
public:
    bool operator()(const T& l, const T& r) { return l <= r; }
};

int main() {
    int n = 0;
    std::cin >> n;

    int* values = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> values[i];
    }

    std::cout << FindKStat<int, Comparator<int>>(values, n, n * 0.1) << std::endl;
    std::cout << FindKStat<int, Comparator<int>>(values, n, n * 0.5) << std::endl;
    std::cout << FindKStat<int, Comparator<int>>(values, n, n * 0.9) << std::endl;

    delete[] values;

    return 0;
}

//Среднее время работы: O(n)
//Затраты по памяти: O(n)