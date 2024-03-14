/*
Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

3_3. Реализовать очередь с помощью двух стеков.
Требования: Очередь должна быть реализована в виде класса. Стек тоже должен быть реализован в виде класса.
*/

#include <iostream>

class Stack {
private:
    int* data;
    size_t capacity;
    size_t size;

public:
    Stack() : data(nullptr), capacity(0), size(0) {}

    Stack(Stack&) = delete;

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new int[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~Stack() {
        delete[] data;
    }

    void push(const int& value) {
        if (size == capacity) {
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            int* newData = new int[newCapacity];

            for (size_t i = 0; i < size; ++i) {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
            capacity = newCapacity;
        }

        data[size++] = value;
    }

    int pop() {
        if (size == 0) {
            return -1;
        }

        return data[--size];
    }

    bool isEmpty() const {
        return size == 0;
    }

    size_t stackSize() const {
        return size;
    }
};

class Queue {
private:
    Stack stIn;
    Stack stOut;

public:
    Queue() : stIn(), stOut() {}

    Queue(Queue&) = delete;

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            stIn = other.stIn;
            stOut = other.stOut;
        }
        return *this;
    }

    ~Queue() {

    }

    void pushBack(const int& value) {
        stIn.push(value);
    }

    int popFront() {
        if (stOut.isEmpty()) {
            while (!stIn.isEmpty()) {
                stOut.push(stIn.pop());
            }
        }
        return stOut.pop();
    }
};

int main()
{
    Queue queue;
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        int op = 0, val = 0;
        std::cin >> op >> val;
        switch (op) {
        case 2: {
            
            int tmpVal = queue.popFront();
            if (tmpVal != val) {
                std::cout << "NO" << std::endl;
                return 0;
            }
            break;
            
        }
        case 3: {
            queue.pushBack(val);
            break;
        }
        }
        
    }
    std::cout << "YES" << std::endl;

    return 0;
}

//Вычислительная сложность одной операции с очередью: ~O(1)
//Для задачи в общем: O(n)
//Потребляемая память: 20 байт + 4 байта * (сумма длин 2 стеков)