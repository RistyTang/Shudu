#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Sudoku {
public:
    static const int N = 9;
    static const int ROWS = N;
    static const int COLS = N;
    static const int BOX_SIZE = 3;
    static const int NUM_CELLS = ROWS * COLS;
    static const int MIN_GIVENS = 26;
    static const int MAX_GIVENS = 61;

    Sudoku();
    Sudoku(const string& str);
    //获取和设置数独游戏中指定单元格的值
    int getCellValue(int row, int col) const;
    void setCellValue(int row, int col, int value);
    //判断数独游戏中指定单元格是否为固定单元格。
    bool isCellFixed(int row, int col) const;
    //是否已被解决
    bool isSolved() const;
    //是否可解
    bool isSolvable() const;
    //求解数独游戏
    bool solve();
    //是否有唯一解
    bool uniqueSolution() const;
    //数独游戏的状态转换为字符串
    string toString() const;
    //生成一个新的数独游戏
    void generate(int givens, int level);
    //在指定难度水平下，数独游戏中最少和最多的固定单元格数
    static int getMinGivens(int level);
    static int getMaxGivens(int level);

private:
    struct Cell {
        int value;//值
        bool fixed;//是否给出
        Cell() : value(0), fixed(false) {}
    };
    Cell cells_[NUM_CELLS];
    bool isFixed_[NUM_CELLS];//存储是否固定
    int numFixed_;
    int numSolutions_;
    int numIterations_;
    bool eliminate();
    bool search();
    static void shuffle(int array[], int size);
    static bool isValid(const int values[], int size);
    static bool isValid(const Cell cells[], int size);
    static void getBoxIndices(int box, int indices[]);
};
int Sudoku::getMinGivens(int level)
{
    switch (level) {
    case 1: // 简单数独
        return MIN_GIVENS + 10;
    case 2: //中等数独        
        return MIN_GIVENS + 20;
    case 3: //困难数独
        return MIN_GIVENS + 30;
    default:
        return MIN_GIVENS;
    }
}

int Sudoku::getMaxGivens(int level)
{
    switch (level) {
    case 1: // 简单数独
        return MAX_GIVENS - 10;
    case 2: // 中等数独   
        return MAX_GIVENS - 20;
    case 3: // 困难数独
        return MAX_GIVENS - 30;
    default:
        return MAX_GIVENS;
    }
}

Sudoku::Sudoku() {
    for (int i = 0; i < NUM_CELLS; i++) {
        cells_[i].value = 0;
        cells_[i].fixed = false;
        isFixed_[i] = false;
    }
    numFixed_ = 0;
}

Sudoku::Sudoku(const string& str) {
    for (int i = 0; i < NUM_CELLS; i++) {
        cells_[i].fixed = false;
        isFixed_[i] = false;
    }
    numFixed_ = 0;
    for (int i = 0; i < str.size(); i++) {
        int row = i / COLS;
        int col = i % COLS;
        char ch = str[i];
        if (ch >= '1' && ch <= '9') {
            int value = ch - '0';
            setCellValue(row, col, value);
            isFixed_[row * COLS + col] = true;
        }
        else {
            setCellValue(row, col, 0);
        }
    }
}

int Sudoku::getCellValue(int row, int col) const {
    return cells_[row * COLS + col].value;
}

void Sudoku::setCellValue(int row, int col, int value) {
    cells_[row * COLS + col].value = value;
}

bool Sudoku::isCellFixed(int row, int col) const {
    return isFixed_[row * COLS + col];
}

bool Sudoku::isSolved() const {
    for (int i = 0; i < NUM_CELLS; i++) {
        if (cells_[i].value == 0) {
            return false;
        }
    }
    return isValid(cells_, NUM_CELLS);
}

bool Sudoku::solve() {
    numIterations_ = 0;
    numSolutions_ = 0;
    while (eliminate()) {}
    if (!isValid(cells_, NUM_CELLS)) {
        return false;
    }
    if (isSolved()) {
        numSolutions_ = 1;
        return true;
    }
    return search();
}

string Sudoku::toString() const {
    ostringstream oss;
    for (int i = 0; i < NUM_CELLS; i++) {
        oss << cells_[i].value;
        if (i % COLS == COLS - 1) {
            oss << "\n";
        }
        else {
            oss << " ";
        }
    }
    return oss.str();
}

void Sudoku::generate(int givens, int level) {
    vector<int> indices(NUM_CELLS);
    for (int i = 0; i < NUM_CELLS; i++) {
        indices[i] = i;
    }
    shuffle(&indices[0], indices.size());
    for (int i = 0; i < NUM_CELLS; i++) {
        int index = indices[i];
        int row = index / COLS;
        int col = index % COLS;
        int value = getCellValue(row, col);
        if (value == 0) {
            continue;
        }
        setCellValue(row, col, 0);
        if (!isSolvable() || !uniqueSolution() || numFixed_ > MAX_GIVENS) {
            setCellValue(row, col, value);
        }
        else {
            isFixed_[row * COLS + col] = true;
            numFixed_++;
            if (numFixed_ >= givens) {
                break;
            }
        }
    }
}

bool Sudoku::eliminate() {
    bool changed = false;
    for (int i = 0; i < NUM_CELLS; i++) {
        if (cells_[i].value == 0) {
            int row = i / COLS;
            int col = i % COLS;
            int values[N];
            int numValues = 0;
            for (int k = 1; k <= N; k++) {
                bool found = false;
                for (int j = 0; j < N; j++) {
                    if (cells_[row * COLS + j].value == k ||
                        cells_[j * COLS + col].value == k ||
                        cells_[(row / BOX_SIZE * BOX_SIZE + j / BOX_SIZE) * COLS + col / BOX_SIZE * BOX_SIZE + j % BOX_SIZE].value == k) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    values[numValues++] = k;
                }
            }
            if (numValues == 1) {
                setCellValue(row, col, values[0]);
                changed = true;
            }
        }
    }
    numIterations_++;
    return changed;
}

bool Sudoku::search() {
    int index = -1;
    int minNumValues = N + 1;
    for (int i = 0; i < NUM_CELLS; i++) {
        if (cells_[i].value == 0) {
            int row = i / COLS;
            int col = i % COLS;
            int values[N];
            int numValues = 0;
            for (int k = 1; k <= N; k++) {
                bool found = false;
                for (int j = 0; j < N; j++) {
                    if (cells_[row * COLS + j].value == k ||
                        cells_[j * COLS + col].value == k ||
                        cells_[(row / BOX_SIZE * BOX_SIZE + j / BOX_SIZE) * COLS + col / BOX_SIZE * BOX_SIZE + j % BOX_SIZE].value == k) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    values[numValues++] = k;
                }
            }
            if (numValues == 0) {
                return false;
            }
            else if (numValues < minNumValues) {
                index = i;
                minNumValues = numValues;
            }
        }
    }
    if (index == -1) {
        numSolutions_++;
        return true;
    }
    int row = index / COLS;
    int col = index % COLS;
    int values[N];
    int numValues = 0;
    for (int k = 1; k <= N; k++) {
        bool found = false;
        for (int j = 0; j < N; j++) {
            if (cells_[row * COLS + j].value == k ||
                cells_[j * COLS + col].value == k ||
                cells_[(row / BOX_SIZE * BOX_SIZE + j / BOX_SIZE) * COLS + col / BOX_SIZE * BOX_SIZE + j % BOX_SIZE].value == k) {
                found = true;
                break;
            }
        }
        if (!found) {
            values[numValues++] = k;
        }
    }
    shuffle(values, numValues);
    for (int i = 0; i < numValues; i++) {
        setCellValue(row, col, values[i]);
        if (search()) {
            return true;
        }
        setCellValue(row, col, 0);
    }
    return false;
}

bool Sudoku::isSolvable() const 
{
    Sudoku copy(*this);
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < NUM_CELLS; i++) {
            if (copy.cells_[i].value == 0) {
                int row = i / COLS;
                int col = i % COLS;
                int values[N];
                int numValues = 0;
                for (int k = 1; k <= N; k++) {
                    bool found = false;
                    for (int j = 0; j < N; j++) {
                        if (copy.cells_[row * COLS + j].value == k ||
                            copy.cells_[j * COLS + col].value == k ||
                            copy.cells_[(row / BOX_SIZE * BOX_SIZE + j / BOX_SIZE) * COLS + col / BOX_SIZE * BOX_SIZE + j % BOX_SIZE].value == k) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        values[numValues++] = k;
                    }
                }
                if (numValues == 0) {
                    return false;
                }
                else if (numValues == 1) {
                    copy.setCellValue(row, col, values[0]);
                    changed = true;
                }
            }
        }
    }
    return true;
}

bool Sudoku::uniqueSolution() const 
{
    Sudoku copy(*this);
    copy.search();
    return copy.numSolutions_ == 1;
}

void Sudoku::shuffle(int array[], int size) {
    for (int i = 0; i < size; i++) {
        int j = rand() % (size - 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

bool Sudoku::isValid(const int values[], int size) {
    bool used[N] = { false };
    for (int i = 0; i < size; i++) {
        int val = values[i];
        if (val == 0) continue;
        if (used[val]) return false;
        used[val] = true;
    }
    return true;
}

bool Sudoku::isValid(const Cell cells[], int size) {
    bool used[N] = { false };
    for (int i = 0; i < size; i++) {
        Cell cell = cells[i];
        int val = cell.value;
        if (val == 0 && !cell.fixed) continue;
        if (used[val]) return false;
        used[val] = true;
    }
    return true;
}

void Sudoku::getBoxIndices(int box, int indices[])
{
    int rowStart, colStart;
    switch (box) {
    case 0:
        rowStart = colStart = 0;
        break;
    case 1:
        rowStart = 0;
        colStart = 3;
        break;
    case 2:
        rowStart = 0;
        colStart = 6;
        break;
    case 3:
        rowStart = 3;
        colStart = 0;
        break;
    case 4:
        rowStart = 3;
        colStart = 3;
        break;
    case 5:
        rowStart = 3;
        colStart = 6;
        break;
    case 6:
        rowStart = 6;
        colStart = 0;
        break;
    case 7:
        rowStart = 6;
        colStart = 3;
        break;
    case 8:
        rowStart = 6;
        colStart = 6;
        break;
    }

    int k = 0;
    for (int i = rowStart; i < rowStart + BOX_SIZE; i++) {
        for (int j = colStart; j < colStart + BOX_SIZE; j++) {
            indices[k++] = i * COLS + j;
        }
    }
}
