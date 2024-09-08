
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

	int ShortestH() {
		return shortestH(root);
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

	int shortestH(Node* root) {
		if (root == nullptr) {
			return 0;
		}

		int l = shortestH(root->Left);
		int r = shortestH(root->Right);

		if (root->Left == nullptr) {
			return 1 + r;
		}

		if (root->Right == nullptr) {
			return 1 + l;
		}
		return std::min(l, r) + 1;
	}
};

struct IntCmp {
	bool operator()(int l, int r) const { return l < r; }
};

int main()
{
	IntCmp cmp;
	BinaryTree<int, IntCmp> tree(cmp);
	
	int elem = 0;
	while (std::cin >> elem) {
		tree.Add(elem);
	}

	std::cout << tree.ShortestH();

	return 0;
}

