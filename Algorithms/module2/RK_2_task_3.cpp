#include <unordered_set>
#include <iostream>
#include <string>

struct Node {
    std::string product_name;
    int color;
    int size;

    bool operator==(const Node& other) const {
        return product_name == other.product_name &&
            color == other.color &&
            size == other.size;
    }
};

namespace std {
    template<>
    struct hash<Node> {
        size_t operator()(const Node& node) const {
            return hash<std::string>()(node.product_name) ^
                hash<int>()(node.color) ^
                hash<int>()(node.size);
        }
    };
}

int main() {
    std::unordered_set<Node> set;
    std::string operation;

    while (std::cin >> operation) {
        Node node;
        std::cin >> node.product_name >> node.color >> node.size;

        if (operation == "+") {
            set.insert(node);
            std::cout << "OK" << std::endl;
        }
        else if (operation == "-") {
            if (set.find(node) != set.end()) {
                set.erase(node);
                std::cout << "OK" << std::endl;
            }
            else {
                std::cout << "FAIL" << std::endl;
            }
        }
        else if (operation == "?") {
            std::cout << (set.find(node) != set.end() ? "OK" : "FAIL") << std::endl;
        }
    }

    return 0;
}
