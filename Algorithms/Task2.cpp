/*
2_1. Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. 
Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, равного или превосходящего B[i]: A[k] >= B[i]. 
Если такого элемента нет, выведите n. n, m ≤ 10000.
Требования:  Время работы поиска k для каждого элемента B[i]: O(log(k)). 
Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k с помощью экспоненциального поиска, 
а потом уже в нем делать бинарный поиск.
Формат входных данных: в первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.
*/

#include <iostream>
#include <cmath>

std::pair<short int, short int> exp_search(int elem, unsigned short int n, int* A) {
    unsigned short int i = 0;
    while (std::pow(2, i) < n) {
        if (elem <= A[static_cast<int>(std::pow(2, i))]) {
            return std::make_pair(static_cast<int>(std::pow(2, i-1)), static_cast<int>(std::pow(2, i)));
        }
        else {
            i++;
        }
    }
    if (elem <= A[n - 1]) {
        return std::make_pair(static_cast<int>(std::pow(2, i-1)), n - 1);
    }
    return std::make_pair(n,n);
}

int bin_search(int elem, short int left, short int right, int* A) {
    while (left <= right) {
        short int mid = left + (right - left) / 2;
        if (elem <= A[mid]) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return right + 1;
}

int idx_search(int elem, unsigned short int n, int* A) {
    std::pair<short int, short int> borders = exp_search(elem, n, A);
    short int left = borders.first;
    short int right = borders.second;
    if (left == n) {
        return n;
    }
    else {
        return bin_search(elem, left, right, A);
    }
}

int main()
{   
    unsigned short int n;
    unsigned short int m;
    std::cin >> n >> m;
    int* A = new int[n];
    int* B = new int[m];
    for (short int i = 0; i < n; i++) {
        std::cin >> A[i];
    }
    for (short int i = 0; i < m; i++) {
        std::cin >> B[i];
    }
    for (short int i = 0; i < m; i++) {
        std::cout << idx_search(B[i], n, A) << " ";
    }
    delete[] A;
    delete[] B;
}

//Вычислительная сложность O(log(k))
//Потребляемая память:  (4 * (n+m) + 22) байта