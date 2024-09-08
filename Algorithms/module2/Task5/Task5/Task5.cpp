/*
Напишите две функции для создания архива из одного файла и извлечения файла из архива.
Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);
Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);
где:
typedef char byte;
interface IInputStream {
// Возвращает false, если поток закончился
virtual bool Read(byte& value) = 0;
};
interface IOutputStream {
virtual void Write(byte value) = 0;
};
В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных. 
Дерево Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8-битный символ.
В контест необходимо отправить .cpp файл содержащий функции Encode, 
Decode, а также включающий файл Huffman.h. Тестирующая программа выводит 
размер сжатого файла в процентах от исходного.
Пример минимального решения:
*/
#include "Huffman.h"

#include <list>
#include <map>
#include <sstream>
#include <queue>
#include <stack>
#include <vector>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <bitset>
#include <cassert>
#include <clocale>

using std::stack;
using std::list;
using std::map;
using std::string;
using std::vector;

typedef unsigned char byte;

struct IInputStream {
    virtual bool Read(byte& value) = 0;
};

struct IOutputStream {
    virtual void Write(byte value) = 0;
};

vector<string> TREE_BINS;
class Node {
public:
    Node() : left(nullptr), right(nullptr) {}
    Node(Node* l, Node* r) : left(l), right(r), elementsCount(l->elementsCount + r->elementsCount) {}

    size_t elementsCount = 0;
    byte byteChar = 0;
    Node* left;
    Node* right;
};

Node* loadTree(vector<string>& bins) {
    Node* root = nullptr;
    stack<Node*> stack;

    for (int i = 0; i < bins.size(); i++) {
        if (bins[i] == "1") {
            Node* node = new Node();
            char c = bins[++i][0];
            node->byteChar = (char)c;
            stack.push(node);
        }

        if (bins[i] == "0") {
            if (stack.size() == 1) {
                stack.pop();
                std::cout << "Остался один в стеке";
                break;
            };

            if (stack.size() >= 2) {
                Node* root = new Node();

                root->right = stack.top();
                stack.pop();

                root->left = stack.top();
                stack.pop();

                root = root;
                stack.push(root);
            }
        }
    }
    return root;
}

void saveTree(Node* node, vector<string>& bins) {
    if (!node) {
        return;
    }

    if (!node->left && !node->right) {
        bins.push_back("1");
        std::string str = "";
        str = node->byteChar;
        bins.push_back(str);
    }
    else {
        if (node->left) {
            saveTree(node->left, bins);
        }
        if (node->right) {
            saveTree(node->right, bins);
        }
        bins.push_back("0");
    }
}

class HuffmanEncoder {
public:
    HuffmanEncoder() {}

    ~HuffmanEncoder() = default;

    HuffmanEncoder(const HuffmanEncoder&) = delete;

    HuffmanEncoder& operator=(const HuffmanEncoder&) = delete;

    void deleteNode(Node*& node) {
        if (!node) {
            return;
        }

        std::vector<Node*> curr;
        std::vector<Node*> next;

        curr.push_back(root);
        while (!curr.empty()) {
            for (auto node : curr) {
                if (node->left != nullptr)
                    next.push_back(node->left);
                if (node->right != nullptr)
                    next.push_back(node->right);
                delete node;
            }
            curr = next;
            next.clear();
        }
    }

    vector<bool> encode(string s_) {
        s = s_;
        MakeTree();
        BuildTable(root);
        return StringToBin();
    }

    void printTree(void (*visit)(const byte& key)) const {
        PrintTree(root, visit);
    }

    void printCodeTable() {
        for (map<byte, vector<bool>>::iterator itr = code_table.begin();
            itr != code_table.end(); ++itr) {
            std::cout << itr->first << ": ";
            for (int i = 0; i < itr->second.size(); ++i) {
                std::cout << (int)itr->second[i];
            }
            std::cout << std::endl;
        }
    }

private:
    string s;
    struct NodeComparator {
        bool operator()(const Node* left, const Node* right) const {
            return left->elementsCount < right->elementsCount;
        }
    };

    void PrintTree(Node* node, void (*visit)(const byte& key)) const {
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

                if (currentNode->byteChar)
                    std::cout << currentNode->elementsCount << "(" << currentNode->byteChar << ")"
                    << std::endl;
                else
                    std::cout << currentNode->elementsCount << std::endl;

                if (currentNode->left) {
                    queue.push(currentNode->left);
                }

                if (currentNode->right) {
                    queue.push(currentNode->right);
                }
            }
            std::cout << std::endl;
        }
    }

    vector<bool> StringToBin() {
        vector<bool> bin;
        for (int i = 0; i < s.length(); ++i) {
            byte currentByte = s[i];
            vector<bool> tableCode = code_table[currentByte];
            for (int i = 0; i < tableCode.size(); ++i) {
                bin.push_back(tableCode[i]);
            }
        }
        return bin;
    }

    map<byte, size_t> GetFreqs(string s) {
        map<byte, size_t> frequency;
        for (int i = 0; i < s.length(); ++i) {
            byte byteChar = s[i];
            frequency[byteChar]++;
        }
        return frequency;
    }

    void MakeTree() {
        if (s == "")
            return;

        list<Node*> t;
        map<byte, size_t> frequency = GetFreqs(s);

        for (map<byte, size_t>::iterator itr = frequency.begin();
            itr != frequency.end(); ++itr) {
            Node* p = new Node();
            p->byteChar = itr->first;
            p->elementsCount = itr->second;
            t.push_back(p);
        }

        while (t.size() != 1) {
            t.sort(NodeComparator());

            Node* SonL = t.front();
            t.pop_front();
            Node* SonR = t.front();
            t.pop_front();

            Node* parent = new Node(SonL, SonR);
            t.push_back(parent);
        }
        root = t.front();
    }

    void BuildTable(Node* root) {
        if (!root)
            return;

        if (root->left) {
            code.push_back(0);
            BuildTable(root->left);
        }

        if (root->right) {
            code.push_back(1);
            BuildTable(root->right);
        }

        if (!root->left && !root->right)
            code_table[root->byteChar] = code;

        code.pop_back();
    }
    vector<bool> code;

public:
    map<byte, vector<bool>> code_table;
    Node* root = nullptr;
};

map<byte, vector<bool>> CODE_TABLE;
Node* ROOT;

void binToString(byte byte_, string& string_) {
    for (int i = 7; i >= 0; i--) {
        string_ += ((byte_ >> i) & 1) ? '1' : '0';
    }
}

void streamToString(IInputStream& compressed, string& res) {
    string str;
    int strLen;
    byte byte_ = 0;

    while (compressed.Read(byte_)) {
        str = "";
        binToString(byte_, str);
        strLen = str.length();
        res += str;
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    string inputString = "";
    byte buf = 0;

    while (original.Read(buf)) {
        inputString += buf;
    }

    HuffmanEncoder hf;
    vector<bool> code = hf.encode(inputString);
    TREE_BINS.clear();
    saveTree(hf.root, TREE_BINS);
    int excessBits = code.size() % 8;
    std::bitset<8> binary(excessBits);
    std::string excBitNum = binary.to_string();
    vector<bool> finalCode;

    for (int i = 0; i < excBitNum.size(); ++i) {
        bool num_bit = (excBitNum[i] == '1') ? true : false;
        finalCode.push_back(num_bit);
    }
    int finalCodeSize = code.size();

    for (int i = 0; i < code.size(); ++i) {
        finalCode.push_back(code[i]);
    }

    int count = 0;
    buf = 0;
    bool isLastByteWritten = false;

    for (int i = 0; i < finalCode.size(); ++i) {

        buf = buf | (finalCode[i] << (7 - count));
        ++count;
        isLastByteWritten = false;
        if (count == 8) {

            isLastByteWritten = true;
            count = 0;
            compressed.Write(buf);
            buf = 0;
        }
    }

    bool isFinal = finalCode[finalCode.size() - 1];
    if (excessBits > 0) {
        compressed.Write(buf);
    }

    CODE_TABLE = hf.code_table;
    ROOT = hf.root;
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    int count = 0;
    Node* ROOT = loadTree(TREE_BINS);
    Node* node = ROOT;

    std::string result;
    streamToString(compressed, result);
    std::string binExcBits = result.substr(0, 8);
    int excBits = 8 - std::stoi(binExcBits, nullptr, 2);

    if (excBits == 8) {
        excBits = 0;
    }

    result = result.substr(8, result.size());
    result = result.substr(0, result.size() - excBits);

    for (size_t i = 0; i < result.size(); ++i) {
        bool curBit = (result[i] == '1') ? true : false;

        if (curBit)
            node = node->right;
        else
            node = node->left;

        if (!node->left && !node->right) {
            original.Write(node->byteChar);
            node = ROOT;
        }
    }
}
