/*
Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, 
если root→Key ≤ K, то узел K добавляется в правое поддерево root; 
иначе в левое поддерево root. Требования: Рекурсия запрещена. 
Решение должно поддерживать передачу функции сравнения снаружи.
2_3. Выведите элементы в порядке post-order (снизу вверх).
*/
//                    20                     
//                  /     \             
//                8        45
//               /  \     /   \			
//              7    19  30    50          
//                        \    /  \             
//                         35 46  60
          
#include<iostream>
#include <stack>

template<class T, class Compare>
class BinaryTree {
public:
	BinaryTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) {}

	~BinaryTree() {
		std::stack<Node*> stack;
		stack.push(root);

		while (!stack.empty()) {
			Node* current = stack.top();
			stack.pop();

			if (current->Left != nullptr) {
				stack.push(current->Left);
			}
			if (current->Right != nullptr) {
				stack.push(current->Right);
			}

			delete current;
		}
	}

	void Add(const T& key) {
		add(root, key);
	}

	bool Has(const T& key) const {
		return has(root, key);
	}

	void PostOrder(void (*visit)(const T& key)) const {
		postOrder(root, visit);
	}

private:
	struct Node {
		T Key;
		Node* Left;
		Node* Right;

		Node(const T& key) : Key(key), Left(nullptr), Right(nullptr) {}
	};
	Node* root;
	Compare cmp;

	void add(Node*& node, const T& key) {
		if (node == nullptr) {
			node = new Node(key);
			return;
		}

		Node* current = node;
		Node* parent = nullptr;

		while (current != nullptr) {
			parent = current;
			if (cmp(key, current->Key)) {
				current = current->Left;
			}
			else {
				current = current->Right;
			}
		}

		if (cmp(key, parent->Key)) {
			parent->Left = new Node(key);
		}
		else {
			parent->Right = new Node(key);
		}
	}

	bool has(Node* node, const T& key) const {
		Node* current = root;

		while (current != nullptr) {
			if (current->Key == key) {
				return true;
			}
			else if (cmp(key, current->Key)) {
				current = current->Left;
			}
			else {
				current = current->Right;
			}
		}

		return false;
	}

	void postOrder(Node* node, void (*visit)(const T& key)) const {
		if (node == nullptr) {
			return;
		}
		std::stack<Node*> stack;
		stack.push(node);

		while (!stack.empty()) {
			Node* next = stack.top();

			if ((next->Right == node || next->Left == node) || (next->Left == nullptr && next->Right == nullptr)) {
				stack.pop();
				visit(next->Key);
				node = next;
			}
			else {
				if (next->Right != nullptr) {
					stack.push(next->Right);
				}
				if (next->Left != nullptr) {
					stack.push(next->Left);
				}
			}
		}

	}
};

struct IntCmp {
	bool operator()(int l, int r) const { return l < r; }
};

int main()
{
	IntCmp cmp;
	BinaryTree<int, IntCmp> tree(cmp);
	int n = 0;
	std::cin >> n;
	int el = 0;
	for (int i = 0; i < n; i++) {
		std::cin >> el;
		tree.Add(el);
	}

	tree.PostOrder([](const int& key) { std::cout << key << " "; });

	return 0;
}

//Затраты по вермени: все операции O(h), где h – глубина дерева.
//Затраты по памяти: (4 + 12 * n)байт
