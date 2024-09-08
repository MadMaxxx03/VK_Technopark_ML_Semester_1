/*
Написать алгоритм для решения игры в “пятнашки”.
Достаточно найти хотя бы какое-то решение.
Число перемещений костяшек не обязано быть минимальным.

Формат входных данных
Начальная расстановка.
Формат выходных данных
Если решение существует, то в первой строке выходного файла выведите
минимальное число перемещений костяшек, которое нужно сделать, чтобы
достичь выигрышной конфигурации, а во второй строке выведите соответствующую
последовательность ходов: L означает, что костяшка сдвинулась влево,
R – вправо, U – вверх, D – вниз. Если таких последовательностей несколько,
то выведите любую из них. Если же выигрышная конфигурация недостижима,
то выведите в выходной файл одно число −1.
*/

#include <array>
#include <iostream>
#include <cassert>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cmath>

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const std::array<char, FieldSize> FinishField({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 });

class GameState {
public:
    GameState(const std::array<char, FieldSize>& _field);

    bool CanMoveLeft() const;
    bool CanMoveUp() const;
    bool CanMoveRight() const;
    bool CanMoveDown() const;

    GameState MoveLeft() const;
    GameState MoveUp() const;
    GameState MoveRight() const;
    GameState MoveDown() const;

    bool IsFinish() const;

    char GetFieldValue(size_t pos) const { return field[pos]; }
    int GetZeroPos() const { return zeroPos; }

    bool operator==(const GameState& state) const { return field == state.field; }
    bool operator!=(const GameState& state) const { return !(*this == state); }

    size_t Hash() const {
        size_t hash = 0;
        size_t p = 1;
        for (int i = 0; i < FieldSize; ++i) {
            hash += field[i] * p;
            p *= 137;
        }
        return hash;
    }

private:
    std::array<char, FieldSize> field;
    int zeroPos;
};

GameState::GameState(const std::array<char, FieldSize>& _field) :
    field(_field),
    zeroPos(-1)
{
    for (int i = 0; i < field.size(); ++i) {
        if (field[i] == 0) {
            zeroPos = i;
            break;
        }
    }
    assert(zeroPos >= 0);
}

bool GameState::CanMoveLeft() const
{
    return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveUp() const
{
    return zeroPos < FieldSize - SideSize;
}

bool GameState::CanMoveRight() const
{
    return zeroPos % SideSize > 0;
}

bool GameState::CanMoveDown() const
{
    return zeroPos >= SideSize;
}

GameState GameState::MoveLeft() const
{
    assert(CanMoveLeft());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos + 1]);
    ++newState.zeroPos;
    return newState;
}

GameState GameState::MoveUp() const
{
    assert(CanMoveUp());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos + SideSize]);
    newState.zeroPos += SideSize;
    return newState;
}

GameState GameState::MoveRight() const
{
    assert(CanMoveRight());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos - 1]);
    --newState.zeroPos;
    return newState;
}

GameState GameState::MoveDown() const
{
    assert(CanMoveDown());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos - SideSize]);
    newState.zeroPos -= SideSize;
    return newState;
}

bool GameState::IsFinish() const
{
    return field == FinishField;
}

struct StateHasher {
    size_t operator()(const GameState& state) const {
        return state.Hash();
    }
};

int Heuristic(const GameState& state) {
    int distance = 0;
    for (int i = 0; i < FieldSize; ++i) {
        if (state.GetFieldValue(i) != 0) {
            int value = state.GetFieldValue(i);
            int targetX = (value - 1) % SideSize;
            int targetY = (value - 1) / SideSize;
            int x = i % SideSize;
            int y = i / SideSize;
            distance += abs(x - targetX) + abs(y - targetY);
        }
    }
    return distance;
}

struct Node {
    GameState state;
    int heuristic;
    bool operator>(const Node& other) const {
        return heuristic > other.heuristic;
    }
};

std::string ReconstructPath(const std::unordered_map<GameState, char, StateHasher>& cameFrom, const GameState& start, const GameState& goal) {
    std::string result;
    GameState current = goal;

    while (current != start) {
        char move = cameFrom.at(current);
        result += move;
        switch (move) {
        case 'L':
            current = current.MoveRight();
            break;
        case 'U':
            current = current.MoveDown();
            break;
        case 'R':
            current = current.MoveLeft();
            break;
        case 'D':
            current = current.MoveUp();
            break;
        default:
            assert(false);
            break;
        }
    }

    std::reverse(result.begin(), result.end());
    return result;
}

std::string Get15thSolution(const GameState& startState) {
    auto cmp = [](const Node& a, const Node& b) { return a > b; };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openSet(cmp);
    std::unordered_map<GameState, char, StateHasher> cameFrom;
    std::unordered_set<size_t> visited;

    openSet.push({ startState, Heuristic(startState) });
    cameFrom[startState] = 'S';

    while (!openSet.empty()) {
        Node currentNode = openSet.top();
        GameState current = currentNode.state;
        openSet.pop();

        if (current.IsFinish()) {
            return ReconstructPath(cameFrom, startState, current);
        }

        visited.insert(current.Hash());

        auto tryMove = [&](GameState(GameState::* moveFunc)() const, char moveChar) {
            GameState nextState = (current.*moveFunc)();
            if (visited.find(nextState.Hash()) == visited.end()) {
                int heuristic = Heuristic(nextState);
                openSet.push({ nextState, heuristic });
                cameFrom[nextState] = moveChar;
            }
        };

        if (current.CanMoveLeft()) tryMove(&GameState::MoveLeft, 'L');
        if (current.CanMoveUp()) tryMove(&GameState::MoveUp, 'U');
        if (current.CanMoveRight()) tryMove(&GameState::MoveRight, 'R');
        if (current.CanMoveDown()) tryMove(&GameState::MoveDown, 'D');
    }

    return "";
}

std::ostream& operator<<(std::ostream& out, const GameState& state) {
    for (size_t y = 0; y < SideSize; ++y) {
        for (size_t x = 0; x < SideSize; ++x) {
            out << static_cast<int>(state.GetFieldValue(y * SideSize + x)) << " ";
        }
        out << std::endl;
    }
    out << std::endl;
    return out;
}

int main() {
    std::array<char, FieldSize> initialField;
    for (int i = 0; i < FieldSize; ++i) {
        int value;
        std::cin >> value;
        initialField[i] = static_cast<char>(value);
    }

    GameState state(initialField);
    std::string result = Get15thSolution(state);
    std::cout << result.length() << std::endl;
    std::cout << result << std::endl;

    return 0;
}

