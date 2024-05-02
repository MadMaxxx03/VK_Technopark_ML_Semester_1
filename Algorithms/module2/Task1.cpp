/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы
с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми.
Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество,
удаления строки из множества и проверки принадлежности данной строки множеству.
1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
g(k, i) = g(k, i-1) + i (mod m). m - степень двойки.

*/
#include <iostream>
#include <vector>
#include <string>

template<class T, class H>
class HashTable {
public:
	HashTable(int initialSize, const H& _hasher) :
		hasher(_hasher),
		table(initialSize),
		keysCount(0),
		deletedCount(0),
		tableSize(initialSize)
	{}

	~HashTable() {}

	bool Has(const T& key) {
		unsigned int hash = hasher(key) % tableSize;
		HashTableCell cell = table[hash];
		int step = 1;
		while (cell.State != Empty) {
			if (cell.State == Key) {
				if (cell.Key == key) {
					return true;
				}
			}
			hash = (hash + step) % tableSize;
			cell = table[hash];
			++step;
		}

		return false;
	}

	bool Add(const T& key) {
		if (keysCount >= 3 * tableSize / 4) {
			if (deletedCount >= tableSize / 2) {
				growTable(1);
			}
			else {
				growTable(2);
			}
		}

		unsigned int h = hasher(key);
		unsigned int hash = h % tableSize;
		HashTableCell cell = table[hash];
		int firstDeleted = -1;
		int step = 1;

		while (cell.State != Empty) {
			if (cell.State == Key) {
				if (cell.Key == key) {
					return false;
				}
			}
			else {
				if (firstDeleted == -1) {
					firstDeleted = hash % tableSize;
				}
			}

			hash = (hash + step) % tableSize;
			cell = table[hash];
			++step;
		}

		if (firstDeleted != -1) {
			table[firstDeleted].Key = key;
			table[firstDeleted].State = Key;
			table[firstDeleted].Hash = h;
			--deletedCount;
		}
		else {
			table[hash].Key = key;
			table[hash].State = Key;
			table[hash].Hash = h;
			++keysCount;
		}
		return true;
	}

	bool Delete(const T& key) {
		unsigned int hash = hasher(key) % tableSize;
		HashTableCell cell = table[hash];
		int step = 1;

		while (cell.State != Empty) {
			if (cell.State == Key) {
				if (cell.Key == key) {
					table[hash % tableSize].State = Deleted;
					++deletedCount;
					return true;
				}
			}

			hash = (hash + step) % tableSize;
			cell = table[hash];
			++step;
		}

		return false;
	}
	
private:
	H hasher;
	enum CellState { Empty, Key, Deleted };
	unsigned int keysCount;
	unsigned int deletedCount;
	unsigned int tableSize;

	struct HashTableCell {
		T Key;
		unsigned int Hash;
		CellState State;
		HashTableCell() : Hash(0), State(Empty) {}
	};
	std::vector<HashTableCell> table;

	void growTable(unsigned short int n) {
		std::vector<HashTableCell> newTable(tableSize * n);
		tableSize *= n;
		for (int i = 0; i < table.size(); ++i) {
			HashTableCell cell = table[i];
			int step = 1;
			if (cell.State == Key) {
				unsigned int listIndex = cell.Hash % tableSize;
				while (newTable[listIndex].State == Key) {
					listIndex = (listIndex + step) % tableSize;
					++step;
				}
				newTable[listIndex].Hash = cell.Hash;
				newTable[listIndex].Key = cell.Key;
				newTable[listIndex].State = Key;
			}
		}

		keysCount -= deletedCount;
		deletedCount = 0;
		table = std::move(newTable);
	}
};

struct StringHasher {
	unsigned int operator()(const std::string& key) const {
		unsigned int hash = 0;
		for (unsigned int i = 0; i < key.size(); ++i) {
			hash = hash * 137 + key[i];
		}
		return hash;
	}
};

int main()
{
	StringHasher hasher;
	HashTable<std::string, StringHasher> table(8, hasher);
	char operation = 0;
	std::string word;
	while (std::cin >> operation >> word) {
		switch (operation) {
		case '+':
			std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
			break;
		case '-':
			std::cout << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
			break;
		case '?':
			std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
			break;
		}
	}

	return 0;
}

//Время работы методов поиска, удаления и добавления: от O(1) до O(N)
//Память: ((память для string) + 4 + 4)байта * (tableSize) + 16 байт
