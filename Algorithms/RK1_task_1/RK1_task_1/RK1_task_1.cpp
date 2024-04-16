/*
Дан массив, который получен путем нескольких 
циклических сдвигов исходного отсортированного массива. 
Нужно найти индекс элемента, с которого начинался исходный массив.*/

#include <iostream>

int start_index(int* arr, int n) {
	int l = 0;
	int r = n - 1 ;
	int mid = 0;
	while (l < r) {
		mid = (l + r) / 2;
		if (arr[mid] < arr[mid + 1]) {
			r = mid;
		}
		else {
			l = mid+1;
		}
	}
	return r;
}


int main()
{
	int n = 0;
	std::cin >> n;
	int* arr = new int[n];
	for (int i = 0; i < n; i++) {
		std::cin >> arr[i];
	}
	std::cout << start_index(arr, n);
}


