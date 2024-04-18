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
#include <string>>
#include <cassert>

template<class T, class H>
class HashTable {
public:
	// Все ячейки в состоянии Empty
	HashTable(int initialSize, const H& _hasher) :
		hasher(_hasher),
		table(initialSize, nullptr),
		keysCount(0)
	{}

	~HashTable() {
		for (int i = 0; i < table.size(); ++i) {
			HashTableNode* node = table[i];
			while (node != nullptr) {
				HashTableNode* nextNode = node->Next;
				delete node;
				node = nextNode;
			}
		}
	}

	// Empty: return false
	// Key: key == Key ? return true : пробируемся дальше
	// Deleted: пробируемся дальше
	bool Has(const T& key) {
		unsigned int hash = hasher(key) % table.size();
		HashTableNode* node = table[hash];
		while (node != nullptr && node->Key != key) {
			node = node->Next;
		}
		return node != nullptr;
	}

	// Empty: firstDeleted == -1 ?
	//  (записываем ключ в текущую ячейку, помечаем ячейку Key и return true) :
	//  (записываем ключ в firstDeleted, помечаем ячейку Key и return true)
	// Key: key == Key ? return false : пробируемся дальше
	// Deleted: встречали ли Deleted раньше ? пробируемся дальше : запоминаем индекс в firstDeleted
	bool Add(const T& key) {
		if (keysCount > 3 * table.size()) {
			growTable();
		}

		unsigned int hash = hasher(key);
		unsigned int listIndex = hash % table.size();
		HashTableNode* node = table[listIndex];
		while (node != nullptr && node->Key != key) {
			node = node->Next;
		}

		if (node != nullptr) {
			return false;
		}

		table[listIndex] = new HashTableNode(key, hash, table[listIndex]);
		++keysCount;

		return true;
	}

	// Empty: return false
	// Key: key == Key ? помечаем ячейку Deleted и return true : пробируемся дальше
	// Deleted: пробируемся дальше
	bool Delete(const T& key) {
		unsigned int hash = hasher(key) % table.size();
		HashTableNode* node = table[hash];
		HashTableNode* prevNode = nullptr;
		while (node != nullptr && node->Key != key) {
			prevNode = node;
			node = node->Next;
		}

		if (node == nullptr) {
			return false;
		}

		if (prevNode == nullptr) {
			table[hash] = node->Next;
		}
		else {
			prevNode->Next = node->Next;
		}

		delete node;
		--keysCount;
		return true;
	}

private:
	H hasher;
	//	enum CellState { Empty, Key, Deleted };
	//	struct HashTableCell {
	//		T Key;
	//		unsigned int Hash;
	//		CellState State;
	//		HashTableCell() : Hash( 0 ), State( Empty ) {}
	//	};
	//	std::vector<HashTableCell> table;

	struct HashTableNode {
		T Key;
		unsigned int Hash;
		HashTableNode* Next;

		HashTableNode() : Next(nullptr), Hash(0) {}
		HashTableNode(const T& key, unsigned int hash, HashTableNode* next) :
			Key(key), Hash(hash), Next(next) {}
	};
	std::vector<HashTableNode*> table;
	unsigned int keysCount;

	void growTable() {
		std::vector<HashTableNode*> newTable(table.size() * 2, nullptr);
		for (int i = 0; i < table.size(); ++i) {
			HashTableNode* node = table[i];
			while (node != nullptr) {
				HashTableNode* nextNode = node->Next;

				unsigned int listIndex = node->Hash % newTable.size();
				node->Next = newTable[listIndex];
				newTable[listIndex] = node;

				node = nextNode;
			}
		}

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
	HashTable<std::string, StringHasher> table(2, hasher);
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
