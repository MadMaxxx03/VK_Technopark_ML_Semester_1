/*
4_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту. 
Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, 
а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. 
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены 
по росту – сначала самые высокие, а в конце – самые низкие. За расстановку 
солдат отвечал прапорщик, который заметил интересную особенность – все солдаты
в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику 
правильно расставлять солдат, а именно для каждого приходящего солдата указывать, 
перед каким солдатом в строе он должен становится.
*/

#include <iostream>

template <typename T, class Compare>
class AvlTree{
    struct Node{
        Node(const T& data):data(data), left(nullptr), right(nullptr), height(1), statistic(1){}

        T data;
        Node* left;
        Node* right;
        size_t height;
        size_t statistic;
    };

public:
    AvlTree(const Compare& _cmp):root(nullptr), cmp(_cmp){}

    ~AvlTree(){destroyTree(root);}

    AvlTree(const AvlTree& other) = delete;

    AvlTree& operator=(const AvlTree& other) = delete;


    int Add(const T& data){
        int statistic = 0;
        root = addInternal(root, data, statistic);
        return statistic;
    }

    bool Has(const T& data){
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

    void Delete(T& idx){
        Node* curr = root;
        int statistic = 0;
        if (curr->left)
            statistic = getStatistic(curr->left);

        while (idx != statistic && (curr->left || curr->right)){
            if (idx < statistic){
                curr = curr->left;
                statistic = getStatistic(curr->left);
            }

            else if (idx > statistic){
                idx -= (getStatistic(curr->left) + 1);
                curr = curr->right;

                if (!curr->left)
                    statistic = 0;
                else
                    statistic = getStatistic(curr->left);
            }
        }

        root = deleteInternal(root, curr->data);
    }

private:
    void destroyTree(Node* node){
        if (node){
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* deleteInternal(Node* node, const T& data){
        if (!node)
            return nullptr;
        if (cmp(node->data, data))
            node->right = deleteInternal(node->right, data);
        else if (cmp(data, node->data))
            node->left = deleteInternal(node->left, data);
        else{
            Node* left = node->left;
            Node* right = node->right;

            delete node;

            if (!right)
                return left;

            Node* min; 
            right = findAndRemoveMin(right, min);
            min->right = right; 
            min->left = left;

            return doBalance(min);
        }

        return doBalance(node);
    }

    
    Node* findAndRemoveMin(Node* root, Node* & node) {
        if (!root) {
            node = nullptr;
            return nullptr;
        }
        if (!root->left) {
            node = root;
            return root->right;
        }
        root->left = findAndRemoveMin(root->left, node);
        return doBalance(root);
    }

    Node* addInternal(Node* node, const T& data, int& statistic){
        if (!node)
            return new Node(data);
        if (cmp(node->data, data)) {
            statistic += getStatistic(node->left) + 1;
            node->right = addInternal(node->right, data, statistic);
        }    
        else
            node->left = addInternal(node->left, data, statistic);

        return doBalance(node);
    }

    size_t getHeight(Node* node){
        return node ? node->height : 0;
    }

    void fixHeight(Node* node){
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    size_t getStatistic(Node* node) {
        return node ? node->statistic : 0;
    }

    void fixStatistic(Node* node) {
        node->statistic = getStatistic(node->left) + getStatistic(node->right) + 1;
    }

    int getBalance(Node* node){
        return getHeight(node->right) - getHeight(node->left);
    }

    Node* rotateLeft(Node* node){
        Node* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixStatistic(node);
        fixHeight(tmp);
        fixStatistic(tmp);
        return tmp;
    }

    Node* rotateRight(Node* node){
        Node* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixStatistic(node);
        fixHeight(tmp);
        fixStatistic(tmp);
        return tmp;
    }

    Node* doBalance(Node* node){
        fixHeight(node);
        fixStatistic(node);
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
    bool operator()(int l, int r) const { return l > r; }
}; 

int main(int argc, const char* argv[]) {
    IntCmp cmp;
    AvlTree <int, IntCmp> tree(cmp);

    int n = 0;
    std::cin >> n;
    short comand;
    int height;
    for (int i = 0; i < n; ++i){
        std::cin >> comand >> height;
        if (comand == 1) {
            std::cout << tree.Add(height) << std::endl;
        }
        else if (comand == 2) {
            tree.Delete(height);
        }
    }
    return 0;  
}

//Вычислительная сложность: O(log n)
//Затраты по памяти 4+20*n байт