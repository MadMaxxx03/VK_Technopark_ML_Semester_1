

#include <iostream>

template <typename T, class Compare>
class AvlTree
{
    struct Node
    {
        Node(const T& data)
            : data(data), left(nullptr), right(nullptr), height(1)
        {
        }

        T data;
        Node* left;
        Node* right;
        size_t height;
    };

public:
    AvlTree(const Compare& _cmp)
        : root(nullptr), cmp(_cmp)
    {
    }

    ~AvlTree()
    {
        destroyTree(root);
    }

    void Insert(const T& data)
    {
        root = addInternal(root, data);
    }

    bool Exists(const T& data)
    {
        Node* tmp = root;
        while (tmp)
        {
            if (tmp->data == data)
                return true;
            else if (cmp(tmp->data, data))
                tmp = tmp->right;
            else
                tmp = tmp->left;
        }
        return false;
    }
    void Delete(const T& data)
    {
        root = deleteInternal(root, data);
    }

    void Next(T x) {
        next(x, root);
    }

    void Prev(T x) {
        prev(x, root);
    }

private:
    void prev(T x, Node* root) {
        Node* tmp = root;
        Node* prev = nullptr;
        while (tmp) {
            if (cmp(tmp->data, x)) {
                prev = tmp;
                tmp = tmp->right;
            }
            else {
                tmp = tmp->left;
            }
        }
        if (prev) {
            std::cout << prev->data << std::endl;
        }
        else {
            std::cout << "none" << std::endl;
        }
    }

    void next(T x, Node* root) {
        Node* tmp = root;
        Node* prev = nullptr;
        while (tmp) {
            if (cmp(x, tmp->data)) {
                prev = tmp;
                tmp = tmp->left;
            }
            else {
                tmp = tmp->right;
            }
        }
        if (prev) {
            std::cout << prev->data << std::endl;
        }
        else {
            std::cout << "none" << std::endl;
        }
    }

    void destroyTree(Node* node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* deleteInternal(Node* node, const T& data)
    {
        if (!node)
            return nullptr;
        if (cmp(node->data, data))
            node->right = deleteInternal(node->right, data);
        else if (cmp(data, node->data))
            node->left = deleteInternal(node->left, data);
        else
        {
            Node* left = node->left;
            Node* right = node->right;

            delete node;

            if (!right)
                return left;

            Node* min = findMin(right);
            min->right = removeMin(right);
            min->left = left;

            return doBalance(min);
        }

        return doBalance(node);
    }

    Node* findMin(Node* node)
    {
        while (node->left)
            node = node->left;
        return node;
    }

    Node* removeMin(Node* node)
    {
        if (!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    Node* addInternal(Node* node, const T& data)
    {
        if (!node)
            return new Node(data);
        if (node->data <= data)
            node->right = addInternal(node->right, data);
        else
            node->left = addInternal(node->left, data);

        return doBalance(node);
    }

    size_t getHeight(Node* node)
    {
        return node ? node->height : 0;
    }

    void fixHeight(Node* node)
    {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    int getBalance(Node* node)
    {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node* rotateLeft(Node* node)
    {
        Node* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    Node* rotateRight(Node* node)
    {
        Node* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    Node* doBalance(Node* node)
    {
        fixHeight(node);
        switch (getBalance(node))
        {
        case 2:
        {
            if (getBalance(node->right) < 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        case -2:
        {
            if (getBalance(node->left) > 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        default:
            return node;
        }
    }

    Node* root;
    Compare cmp;
};

struct IntCmp {
    bool operator()(int l, int r) const { return l < r; }
};

int main(int argc, const char* argv[]) {
    IntCmp cmp;
    AvlTree <int, IntCmp> tree(cmp);

    std::string command;
    int value;
    while (std::cin >> command >> value) {
        if (command == "insert") {
            tree.Insert(value);
        }
        else if (command == "exists") {
            std::cout << (tree.Exists(value) ? "true" : "false") << std::endl;
        }
        else if (command == "next") {
            tree.Next(value);
        }
        else if (command == "prev") {
            tree.Prev(value);
        }
        else if (command == "delete") {
            if (tree.Exists(value))
                tree.Delete(value);
        }
    }

    return 0;
}