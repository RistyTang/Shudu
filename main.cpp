#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include "Sudoku.h"

using namespace std;

void usage() {
    cout << "Usage:" << endl;
    cout << "  Sudoku.exe -c COUNT      : Generate COUNT Sudoku games and save to file" << endl;
    cout << "  Sudoku.exe -s FILENAME   : Solve Sudoku games in FILENAME and save to file" << endl;
    cout << "  Sudoku.exe -n COUNT [OPTIONS] : Generate COUNT Sudoku games and save to file" << endl;
    cout << "Options:" << endl;
    cout << "  -m LEVEL   : Set difficulty level (1: Easy, 2: Medium, 3: Hard)" << endl;
    cout << "  -r MIN MAX : Set number of blank cells (MIN <= # of blanks <= MAX)" << endl;
    cout << "  -u         : Generate Sudoku games with unique solution" << endl;
}

int main(int argc, char* argv[]) {
    //�Ƿ�����
    if (argc <= 1) {
        usage();
        return 0;
    }

    // Parse command line arguments
    int count = 0;
    string filename;
    int difficulty = 0;
    int min_blanks = 0;
    int max_blanks = 0;
    bool unique_solution = false;
    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "-c")
        {
            i++;
            if (i > argc)
            {
                cout << "δ���������վ�����" << endl;
                usage();
                return 1;
            }
            int num = stoi(argv[i]);
            generate_sudoku_endgames(num);
        }
        else if (arg == "-s")
        {
            i++;
            if (i >= argc)
            {
                cout << "δ��������ļ�����" << endl;
                usage();
                return 1;
            }
            filename = argv[i];
            solve_sudoku(0, 0);
            read_sudoku_from_file(filename.c_str());
            write_solution_to_file("Solution.txt");
        }
        else if (arg == "-n")
        {
            i++;
            if (i >= argc)
            {
                cout << "δ��������������Ϸ����" << endl;
                usage();
                return 1;
            }
            int num = stoi(argv[i]);
            //i++;
            if (i + 1 < argc && argv[i + 1][0] == '-')
            {
                string option = argv[i + 1];
                if (option == "-m")
                {
                    if (i + 2 >= argc)
                    {
                        cout << "��ָ��������Ϸ�Ѷȣ�" << endl;
                        usage();
                        return 1;
                    }
                    difficulty = stoi(argv[i + 2]);
                    if (difficulty < 1 || difficulty > 3) {
                        cout << "�Ѷ�ȡֵ����Ϊ1��2��3" << endl;
                        usage();
                        return 1;
                    }
                    generate_sudoku_games_difficulty(num, difficulty);
                    i += 2;
                }
                else if (option == "-r") {
                    if (i + 3 >= argc) {
                        cout << "��ָ���ڿ�����" << endl;
                        usage();
                        return 1;
                    }
                    min_blanks = stoi(argv[i + 2]);
                    max_blanks = stoi(argv[i + 3]);
                    if (min_blanks < 20 || max_blanks < min_blanks || max_blanks > 55) {
                        cout << "�������䲻�Ϸ���0<min<=max<=81" << endl;
                        usage();
                        return 1;
                    }
                    generate_sudoku_games_blanks(num, min_blanks, max_blanks);
                    i += 3;
                }
                else if (option == "-u") {
                    unique_solution = true;
                    i += 1;
                    generate_sudoku_uniques(num);
                }
                else {
                    cout << "�������" << option << endl;
                    usage();
                    return 1;
                }
            }
            else
            {
                generate_sudoku_games(num);
            }
        }
        else {
            cout << "Error: Unknown command " << arg << endl;
            usage();
            return 1;
        }
    }
    return 0;
}
