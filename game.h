#pragma once

constexpr size_t ROW_COUNT = 4;
constexpr size_t COLUMN_COUNT = 4;

typedef int Cell;

enum class Direction {
    UP, RIGHT, DOWN, LEFT
};

enum class LineType {
    LINE, ROW
};

struct Position {
    int row;
    int column;

    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    void operator+=(const Position& other);
    friend Position operator+(Position left, const Position& right);
    void operator-=(const Position& other);
    friend Position operator-(Position left, const Position& right);
};

struct PositionPair {
    Position first;
    Position second;
};

class CellIterator {
private:
    Position iterator;
    Position end;
    Position delta;
public:
    CellIterator(Direction direction, int index);
    bool hasNext() const;
    PositionPair next();
};

bool handlePair(Cell& first, Cell& second);

class GameState {
private:
    Cell cells[ROW_COUNT][COLUMN_COUNT];
    // bool shiftLine(LineType line_type, size_t index);
public:
    GameState();
    Cell& GameState::get(const Position& position);
    bool shift(Direction direction);
    bool shiftIndex(Direction direction, int index);
    /** Spawn a 2 in a random position */
    void spawn();
};