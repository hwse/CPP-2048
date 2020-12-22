#include <iostream>
#include <random>
#include <cassert>  

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
    for (size_t row = 0; row < ROW_COUNT; row++) {
        for (size_t column = 0; column < COLUMN_COUNT; column++) {
            cells[row][column] = 0;
        }
    }
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

void GameState::spawn() {
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> row_dist(0, ROW_COUNT-1);
    std::uniform_int_distribution<int> col_dist(0, COLUMN_COUNT-1);
    
    while(true) {
        Position p {row_dist(rng), col_dist(rng)};
        // spawn 2 if empty and return
        Cell& c = get(p);
        if (c == 0) {
            c = 2;
            return;
        }
    }
}