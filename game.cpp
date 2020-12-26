#include <iostream>
#include <random>
#include <cassert>  
#include <algorithm>

#include "game.h"

bool Position::operator==(const Position& other) const {
    return row == other.row && column == other.column;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

void Position::operator+=(const Position& other) {
    row += other.row;
    column += other.column;
}

Position operator+(Position left, const Position& right) {
    left += right;
    return left;
}

void Position::operator-=(const Position& other) {
    row -= other.row;
    column -= other.column;
}

Position operator-(Position left, const Position& right) {
    left -= right;
    return left;
}


CellIterator::CellIterator(Direction direction, int index) {
    // a  - b  - c  - d
    // cd - bc - ab
    switch (direction) {
        case Direction::UP:
            iterator = Position{1, index};
            end = Position{ROW_COUNT, index};
            delta = Position{1, 0};
            break; 
        case Direction::RIGHT:
            iterator = Position{index, ROW_COUNT - 2};
            end = Position{index, -1};
            delta = Position{0, -1};
            break;
        case Direction::DOWN:
            iterator = Position{ROW_COUNT - 2, index};
            end = Position{-1, index};
            delta = Position{-1, 0};
            break;
        case Direction::LEFT:
            iterator = Position{index, 1};
            end = Position{index, COLUMN_COUNT};
            delta = Position{0, 1};
            break;
    }
}

bool CellIterator::hasNext() const {
    return iterator != end;
}

PositionPair CellIterator::next() {
    Position first = iterator;
    iterator += delta;
    PositionPair result{first, first-delta};
    return result;
}

bool handlePair(Cell& first, Cell& second) {
    // shift from first -> second
    if (first == 0) {
        // 0-0 -> 0-0
        // 0-2 -> 0-2
        return false;
    } else if (second == 0) {
        // 2-0 -> 0-2
        second = first;
        first = 0;
        return true;
    } else if (first == second) {
        // 2-2 -> 0-4
        second *= 2;
        first = 0;
        return true;
    } else {
        // 2-4 -> 2-4
        return false;
    }
}

GameState::GameState() { 
    reset();
}

Cell& GameState::get(const Position& position) {
    assert(position.row >= 0);
    assert(position.row < ROW_COUNT);
    assert(position.column >= 0);
    assert(position.column < COLUMN_COUNT);
    return cells[position.row][position.column];
}

bool GameState::shift(Direction direction) {
    bool changed = false;
    int indexEnd = direction == Direction::UP || direction == Direction::DOWN ? ROW_COUNT : COLUMN_COUNT;
    for (int index = 0; index < indexEnd; index++) {
        changed |= shiftIndex(direction, index);
    }
    return changed;
}

bool GameState::shiftIndex(Direction direction, int index) {
    bool any_change = false;
    while (true) {
        CellIterator iterator{direction, index};
        bool iteration_change = false;
        while (iterator.hasNext()) {
            PositionPair cells = iterator.next();
            iteration_change |= handlePair(get(cells.first), get(cells.second));
        }
        
        any_change |= iteration_change;
        // if there were no changes in this iteration we may return
        if (!iteration_change) {
            return any_change;
        }
    }
}

bool GameState::spawn() {
    // find all empty 
    std::vector<Position> free_positions;
    std::copy_if(begin(), 
                 end(), 
                 std::back_inserter(free_positions), 
                 [this](auto position) { return get(position) == 0; });

    if (free_positions.empty()) {
        return false;
    }

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> index_dist(0, free_positions.size() - 1);
    int index = index_dist(rng);
    get(free_positions.at(index)) = 2;
    return true;
}

bool GameState::full() {
    return std::all_of(begin(), end(), [this](auto position) { 
        return get(position) != 0; 
    });
}

void GameState::reset() {
    for (auto position: *this) {
        get(position) = 0;
    }
}

GameStateIterator GameState::begin() const {
    return GameStateIterator({0, 0});
}
    
GameStateIterator GameState::end() const {
    // GameStateIterator first increments columns then rows
    // so the last element we want out of the iterator is { ROW_COUNT-1, COLUMN_COUNT-1 }
    // Which means the end must be the element after that, which is: 
    return GameStateIterator({ ROW_COUNT, 0 });
}

GameStateIterator::GameStateIterator(Position p)
    : position{p} {
}

bool GameStateIterator::operator==(const GameStateIterator& other) const {
    return position == other.position;
}

bool GameStateIterator::operator!=(const GameStateIterator& other) const {
    return !(*this == other);
}

void GameStateIterator::operator++() {
    position.column += 1;
    if (position.column >= COLUMN_COUNT) {
        position.column = 0;
        position.row += 1;
    }
}

Position GameStateIterator::operator*() {
    return position;
}