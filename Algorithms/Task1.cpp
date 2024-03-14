#include <iostream>

//Входное число лежит в диапазоне 0..2^32-1 и вводится в десятичном виде.
//1.1 Подсчитать кол - во единичных бит в входном числе, стоящих на четных позициях.Позиции битов нумеруются с 0.

int bit_count(unsigned int num) {
    unsigned char count = 0;
    while (num > 0) {
        count += num & 1;
        num = num >> 2;
    }
    return count;
}

int main()
{
    unsigned int num;
    std::cin >> num;
    std::cout << bit_count(num);
}

//Вычислительная сложность: O(n)
//Потребляемая память: 5 байт
