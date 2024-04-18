/*
Требование для всех вариантов Задачи 5
Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием в виде шаблонной функции.
Решение должно поддерживать передачу функции сравнения снаружи.
Общее время работы алгоритма O(n log n).

5_4. Закраска прямой 2.
На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и Ri). 
Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.

*/

#include <iostream>

template <class T, class Comparator>
void merge(T* arr, int left, int mid, int right, Comparator cmp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    T* L = new T[n1];
    T* R = new T[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (cmp(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

template <class T, class Comparator = std::less<T>>
void iterativeMergeSort(T* arr, int n, Comparator cmp = Comparator()) {
    for (int i = 1; i <= n - 1; i = 2 * i) {
        for (int l = 0; l < n - 1; l += 2 * i) {
            int m = (l + i - 1) < (n - 1) ? (l + i - 1) : (n - 1);
            int r = (l + 2 * i - 1) < (n - 1) ? (l + 2 * i - 1) : (n - 1);
            merge(arr, l, m, r, cmp);
        }
    }
}

struct Section {
    int x;
    int delta;
};

class SectionComparator {
public:
    SectionComparator() {}
    bool operator()(const Section& l, const Section& r) {
        if (l.x != r.x) {
            return l.x < r.x;
        }
        else {
            return l.delta < r.delta;
        }
    }
};

int main() {
    
    int n = 0;
    std::cin >> n;
    Section* sections = new Section[n*2];
    for (int i = 0; i < n; i++) {
        std::cin >> sections[i * 2].x >> sections[i * 2 + 1].x;
        sections[i * 2].delta = 1;
        sections[i * 2 + 1].delta = -1;
    }
    
    iterativeMergeSort<Section, SectionComparator>(sections, n*2);

    int ans = 0;
    int delta = 0;
    for (int i = 0; i < n * 2; i+=2) {
        delta += sections[i].delta;
        if (delta == 1) {
            ans += sections[i + 1].x - sections[i].x;
        }
        delta += sections[i + 1].delta;
    }

    std::cout << ans;
    
    delete[] sections; 
    
    return 0;
}

// Время работы: O(n log n)
// Затраты памяти: (20 + 8*2*N + 44 + 8*2*N)байт
