/*
Постройте B-дерево минимального порядка t и выведите его по слоям.
В качестве ключа используются числа, лежащие в диапазоне [0..232-1]
Требования:

B-дерево должно быть реализовано в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
*/

#include <iostream>
#include <vector>
#include <queue>

template <class T, class Compare>
class BTree{
public:
    BTree(size_t _t, Compare _cmp = Compare()) : cmp(_cmp), t(_t), root(nullptr) {}

    ~BTree() {
        if (root)
            delete root;
    }

    void Insert(const T& key) {
        if (!root)
            root = new Node(true);
        if (isNodeFull(root))
        {
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(root, 0);
        }
        insertNonFull(root, key);
    }

    void PrintTree(void (*visit)(const T& key)) const {
        printTree(root, visit);
    }

private:
    struct Node
    {
        Node(bool leaf) : leaf(leaf) {}

        ~Node() {
            for (Node* child : children) {
                delete child;
            }
        }

        bool leaf;
        std::vector<T> keys;
        std::vector<Node*> children;
    };
    size_t t;
    Compare cmp;
    Node* root;

    void printTree(Node* node, void (*visit)(const T& key)) const {
        if (node == nullptr) {
            return;
        }

        std::queue<Node*> queue;
        queue.push(node);

        while (!queue.empty()) {
            int l;
            l = queue.size();

            for (int i = 0; i < l; i++) {
                Node* currentNode = queue.front();
                queue.pop();

                for (const auto& key : currentNode->keys) {
                    visit(key);
                }

                if (!currentNode->leaf) {
                    for (const auto& child : currentNode->children) {
                        queue.push(child);
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    bool isNodeFull(Node* node) { return node->keys.size() == 2 * t - 1; }

    void splitChild(Node* node, size_t index) {
        Node* y = node->children[index];
        Node* z = new Node(y->leaf);

        z->keys.resize(t - 1);

        for (size_t j = 0; j < t - 1; j++)
            z->keys[j] = y->keys[j + t];

        if (!y->leaf) {
            z->children.resize(t);
            for (size_t j = 0; j < t; j++)
                z->children[j] = y->children[j + t];
            y->children.resize(t);
        }

        node->children.resize(node->children.size() + 1);

        for (int j = node->keys.size(); j >= (int) index; j--)
            node->children[j + 1] = node->children[j];

        node->children[index + 1] = z;

        bool flag = true;
        for (int j = node->keys.size() - 1; j > (int)index - 1; j--) {
            if (flag) {
                node->keys.resize(node->keys.size() + 1);
                flag = false;
            }
            node->keys[j + 1] = node->keys[j];
        }

        if (flag)
            node->keys.resize(node->keys.size() + 1);

        node->keys[index] = y->keys[t - 1];
        y->keys.resize(t - 1);
    }

    void insertNonFull(Node* node, const T& key) {
        int pos = node->keys.size() - 1;
        if (node->leaf)
        {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && cmp(key, node->keys[pos]))
            {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        }
        else
        {
            while (pos >= 0 && cmp(key, node->keys[pos]))
            {
                pos--;
            }

            if (isNodeFull(node->children[pos + 1])) {
                splitChild(node, pos + 1);
                if (key > node->keys[pos + 1])
                    pos++;
            }
            insertNonFull(node->children[pos + 1], key);
        }
    }
};

struct IntCmp {
    bool operator()(int l, int r) const { return l < r; }
};


int main() {
    IntCmp cmp;
    size_t t;
    std::cin >> t;

    BTree<int, IntCmp> tree(t);
    int elem;

    while (std::cin >> elem) {
        tree.Insert(elem);
    }

    tree.PrintTree([](const int& key) { std::cout << key << " "; });
    return 0;
}

//Вычислитеьная сложность:  O(t ∗ logtN)
//Затраты по памяти: (кол узлов)*(2t - 1)*8байт + 12байт
