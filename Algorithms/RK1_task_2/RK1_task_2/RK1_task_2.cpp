#include <iostream>
#include<cassert>

template <class T>
class Stack {
private:
    T* data;
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
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~Stack() {
        delete[] data;
    }

    void push(const T& value) {
        if (size == capacity) {
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            T* newData = new T[newCapacity];

            for (size_t i = 0; i < size; ++i) {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
            capacity = newCapacity;
        }

        data[size++] = value;
    }

    T top() {
        assert(size != 0 && "Empty stack");

        return data[size-1];
    }

    T pop() {
        assert(size != 0 && "Empty stack");

        return data[--size];
    }

    bool isEmpty() const {
        return size == 0;
    }

    size_t stackSize() const {
        return size;
    }
};

bool isBalanced(std::string brackets) {
    Stack<char> s;

    for (char bracket : brackets) {
        if (bracket == '(') {
            s.push(bracket);
        }
        else if (bracket == ')') {
            if (s.isEmpty() || s.top() != '(') {
                return false; 
            }
            s.pop();
        }
    }

    return s.isEmpty();
}

int main()
{
    std::string brackets;
    std::cin >> brackets;

    if (isBalanced(brackets)) {
        std::cout << "YES";
    }
    else {
        std::cout << "NO";
    }
    return 0;
}

