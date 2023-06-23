#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Sudoku.h"

using namespace std;

// 打印使用说明
void printUsage() {
    cout << "Usage: Sudoku.exe [options]" << endl;
    cout << "Options:" << endl;
    cout << "  -c <num>       generate <num> sudoku puzzles" << endl;
    cout << "  -s <file>      solve sudoku puzzles in <file>" << endl;
    cout << "  -n <num>       generate <num> sudoku games" << endl;
    cout << "  -m <level>     specify difficulty level of generated games (1=easy, 2=medium, 3=hard)" << endl;
    cout << "  -r <min> <max> specify range of numbers of blanks in generated games" << endl;
    cout << "  -u             ensure unique solution for generated games" << endl;
}

// 生成数独游戏
void generateGames(int num, int level, int min_givens, int max_givens, bool unique) {
    Sudoku sudoku;
    int givens = min_givens;
    int total = 0;
    while (total < num) {
        if (givens > max_givens) {
            cout << "Cannot generate enough games with specified range of blanks." << endl;
            break;
        }
        sudoku.generate(givens, level);
        if (!unique || sudoku.uniqueSolution()) {
            cout << sudoku.toString() << endl;
            total++;
        }
        givens++;
    }
}

// 从文件中读取数独游戏并求解
void solveGames(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cout << "Error: cannot open file " << filename << "." << endl;
        return;
    }
    string line;
    while (getline(infile, line)) {
        Sudoku sudoku(line);
        cout << "Puzzle: " << endl << sudoku.toString() << endl;
        if (sudoku.solve()) {
            cout << "Solution: " << endl << sudoku.toString() << endl;
        }
        else {
            cout << "No solution found." << endl;
        }
    }
}

// 主函数
int main(int argc, char* argv[]) {
    // 解析命令行参数
    int num = 0;
    int level = 0;
    int min_givens = 0;
    int max_givens = 0;
    bool unique = false;
    string filename;
    cout << "argc=" << argc << endl;
    for (int i = 1; i < argc; i++) 
    {
        string arg = argv[i];
        cout << arg << endl;
        if (arg == "-c") {
            i++;
            cout << "i=" << i << endl;
            if (i > argc) {
                printUsage();
                return 1;
            }
            num = stoi(argv[i]);
        }
        else if (arg == "-s") {
            i++;
            if (i >= argc) {
                printUsage();
                return 1;
            }
            filename = argv[i];
        }
        else if (arg == "-n") {
            i++;
            if (i >= argc) {
                printUsage();
                return 1;
            }
            num = stoi(argv[i]);
        }
        else if (arg == "-m") {
            i++;
            if (i >= argc) {
                printUsage();
                return 1;
            }
            level = stoi(argv[i]);
        }
        else if (arg == "-r") {
            i++;
            if (i >= argc) {
                printUsage();
                return 1;
            }
            min_givens = stoi(argv[i]);
            i++;
            if (i >= argc) {
                printUsage();
                return 1;
            }
            max_givens = stoi(argv[i]);
        }
        else if (arg == "-u") {
            unique = true;
        }
        else {
            printUsage();
            return 1;
        }
        if (level == 0)
        {
            level = 1;
        }
    }
    cout << "here" << endl;
    // 根据参数执行相应的操作
    srand(time(nullptr));
    if (num > 0) {
        if (filename.empty()) 
        {
            if (level > 0) {
                int min_givens = Sudoku::getMinGivens(level);
                int max_givens = Sudoku::getMaxGivens(level);
                generateGames(num, level, min_givens, max_givens, unique);
            }
            else if (min_givens > 0 && max_givens > 0) {
                generateGames(num, 0, min_givens, max_givens, unique);
            }
            else {
                cout << "Error: missing required argument." << endl;
                printUsage();
                return 1;
            }
        }
        else {
            cout << "Error: invalid combination of options." << endl;
            printUsage();
            return 1;
        }
    }
    else if (!filename.empty()) {
        solveGames(filename);
    }
    else {
        cout << "Error: missing required argument." << endl;
        printUsage();
        return 1;
    }

    return 0;
}
