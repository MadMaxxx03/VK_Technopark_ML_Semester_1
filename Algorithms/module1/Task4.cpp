/*
Требование для всех вариантов Задачи 4
Решение всех задач данного раздела предполагает использование кучи, реализованной в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
Куча должна быть динамической.

4.1 Слияние массивов.
Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
Требования: время работы O(N * logK). Ограничение на размер кучи O(K)..
Формат входных данных: Сначала вводится количество массивов K. Затем по очереди размер каждого массива и элементы массива. Каждый массив упорядочен по возрастанию.
Формат выходных данных: Итоговый отсортированный массив.
*/

#include <iostream>
#include<cassert>

template <class T, class Comparator = std::less<T>>
class Heap {
public:
    Heap(Comparator cmp = Comparator())
        : cmp(cmp), buffer(nullptr), bufferSize(0), size(0) {}
    
    Heap(T* arr, int arrSize, Comparator cmp = Comparator())
        : cmp(cmp), buffer(new T[arrSize]), bufferSize(arrSize), size(arrSize) {
        for (int i = 0; i < arrSize; i++) {
            buffer[i] = arr[i];
        }
        buildHeap();
    }

    Heap(const Heap&) = delete;

    Heap& operator=(const Heap& other) = delete;

    ~Heap() {
        delete[] buffer;
    }

    void Insert(const T& element) {
        if (size == bufferSize) {
            grow();
        }
        buffer[size++] = element;
        siftUp(size - 1);
    }

    T ExtractTop() {
        assert(size != 0 && "Empty buffer");
        T res = buffer[0];
        buffer[0] = buffer[size - 1];
        size--;
        siftDown(0);
        return res;
    }

    const T& ExtractTop() const {
        assert(size != 0 && "Empty buffer");
        return buffer[0];
    }

    int Size() const {
        return size;
    }

private:
    Comparator cmp;
    T* buffer;
    int bufferSize;
    int size;

    void buildHeap() {
        for (int i = size / 2 - 1; i >= 0; i--) {
            siftDown(i);
        }
    }

    void siftDown(int i) {
        while (true) {
            int findIndex = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < size && cmp(buffer[left], buffer[findIndex])) {
                findIndex = left;
            }

            if (right < size && cmp(buffer[right], buffer[findIndex])) {
                findIndex = right;
            }

            if (findIndex != i) {
                std::swap(buffer[i], buffer[findIndex]);
                i = findIndex;
            }
            else {
                break;
            }
        }
    }

    void siftUp(int i) {
        int parent = (i - 1) / 2;
        while (i > 0 && cmp(buffer[i], buffer[parent])) {
            std::swap(buffer[i], buffer[parent]);
            i = parent;
            parent = (i - 1) / 2;
        }
    }

    void grow() { 
        int newBufferSize = (size == 0) ? 1 : size * 2;
        T* newBuffer = new T[newBufferSize];
        for (int i = 0; i < size; i++) {
            newBuffer[i] = buffer[i];
        }
        delete[] buffer;
        buffer = newBuffer;
        bufferSize = newBufferSize;
    }

};

struct ArrayIterator {
    int* arr;
    int size;
    int currPos;
};

class ArrayIteratorComparator {
public:
    ArrayIteratorComparator() {}
    bool operator()(const ArrayIterator& l, const ArrayIterator& r) { return l.arr[l.currPos] < r.arr[r.currPos]; }
};

int main() {
    Heap<ArrayIterator, ArrayIteratorComparator> heap;
    int k = 0;
    std::cin >> k;
    int n = 0;
    ArrayIterator arrIt = {};
    int** dynamicArrays = new int* [k]; 

    for (int i = 0; i < k; i++) {
        std::cin >> n;
        int* arr = new int[n];
        for (int j = 0; j < n; ++j) {
            std::cin >> arr[j];
        }
        arrIt = { arr, n, 0 };
        heap.Insert(arrIt);
        dynamicArrays[i] = arr;
    }

    while (heap.Size() > 0) {
        arrIt = heap.ExtractTop();
        std::cout << arrIt.arr[arrIt.currPos] << " ";
        arrIt.currPos++;
        if (arrIt.currPos < arrIt.size) {
            heap.Insert(arrIt);
        }
    }
    // Освобождение памяти для каждого выделенного массива
    for (int i = 0; i < k; ++i) {
        delete[] dynamicArrays[i];
    }
    // Освобождение памяти для массива указателей
    delete[] dynamicArrays;
}

// Время работы: O(N * logK)
// Затраты памяти: (28 + 4 * (k + N) + 4 * N + 12 * k + 4 * n)