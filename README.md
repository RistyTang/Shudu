# Shudu
软件工程结对编程
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
