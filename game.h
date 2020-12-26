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

class GameStateIterator {
public:
    GameStateIterator(Position p);
    bool operator==(const GameStateIterator& other) const;
    bool operator!=(const GameStateIterator& other) const;
    void operator++();
    Position operator*();
private:
    Position position;
};

class GameState {
private:
    Cell cells[ROW_COUNT][COLUMN_COUNT];
    // bool shiftLine(LineType line_type, size_t index);
    
    template<typename Body>
    void forEach(Body body) {
        for (size_t row = 0; row < ROW_COUNT; row++) {
            for (size_t column = 0; column < COLUMN_COUNT; column++) {
                body(cells[row][column]);
            }
        }
    }
public:
    GameState();
    Cell& GameState::get(const Position& position);
    bool shift(Direction direction);
    bool shiftIndex(Direction direction, int index);
    /** Try to spawn a 2 in a random position */
    bool spawn();
    bool full();
    void reset();
    GameStateIterator begin() const;
    GameStateIterator end() const;
};