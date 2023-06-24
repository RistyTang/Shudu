#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <sstream>
#include <string>
#include <random>
#include <set>

using namespace std;

const int N = 9;


// 数独矩阵
int matrix[N][N];
int unique_matrix[N][N];

// 打印数独矩阵
void print_matrix() 
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// 检查某个数字是否可以在该行中出现
bool check_row(int row, int num) {
    for (int i = 0; i < N; i++) {
        if (matrix[row][i] == num) {
            return false;
        }
    }
    return true;
}

// 检查某个数字是否可以在该列中出现
bool check_col(int col, int num) {
    for (int i = 0; i < N; i++) {
        if (matrix[i][col] == num) {
            return false;
        }
    }
    return true;
}

// 检查某个数字是否可以在该宫格中出现
bool check_box(int row, int col, int num) {
    int box_start_row = row - row % 3;
    int box_start_col = col - col % 3;
    for (int i = box_start_row; i < box_start_row + 3; i++) {
        for (int j = box_start_col; j < box_start_col + 3; j++) {
            if (matrix[i][j] == num) {
                return false;
            }
        }
    }
    return true;
}

// 检查某个数字是否可以在该位置（行、列、宫格）中出现
bool check_num(int row, int col, int num) {
    return check_row(row, num) && check_col(col, num) && check_box(row, col, num);
}

// 递归求解数独问题
bool solve_sudoku(int row, int col) 
{
    if (row == N) {
        return true;
    }
    if (col == N) {
        return solve_sudoku(row + 1, 0);
    }
    if (matrix[row][col] != 0) {
        return solve_sudoku(row, col + 1);
    }
    for (int i = 1; i <= 9; i++) {
        if (check_num(row, col, i)) {
            matrix[row][col] = i;
            if (solve_sudoku(row, col + 1)) {
                return true;
            }
            matrix[row][col] = 0;
        }
    }
    return false;
}

//具有唯一解数组求解
bool only_solve_sudoku(int row, int col,int& count)
{
    if (row == N) {
        count++; // 找到一个解，计数器递增
        return true;
    }
    if (col == N) {
        return only_solve_sudoku(row + 1, 0,count);
    }
    if (matrix[row][col] != 0) {
        return only_solve_sudoku(row, col + 1,count);
    }
    for (int i = 1; i <= 9; i++) 
    {
        if (check_num(row, col, i)) {
            matrix[row][col] = i;
            if (only_solve_sudoku(row, col + 1,count)) {
                // 如果找到一个解，则继续搜索其他解
                // 不需要返回 true，因为还需要继续搜索其他解
                matrix[row][col] = 0;
            }
        }
    }
    return false;
}

// 生成数独终局
void generate_sudoku_endgames(int num)
{
    if (num < 1 || num >= 1000000)
    {
        cout << "输入局数不合法" << endl;
        return;
    }
    int endgames[1000][9][9];
    for (int cur = 0;cur < num;cur++)
    {
        srand(time(NULL)+cur);
        // 随机填充第一行
        for (int i = 0; i < N; i++)
        {
            matrix[0][i] = i + 1;
        }
        for (int i = 0; i < N; i++)
        {
            int random_index = rand() % N;
            swap(matrix[0][i], matrix[0][random_index]);
        }
        // 递归填充数独矩阵
        solve_sudoku(0, 0);
        //存储到终局中
        for (int i = 0;i < N;i++)
        {
            for (int j = 0;j < N;j++)
            {
                endgames[cur][i][j] = matrix[i][j];
            }
        }
    }
    // 将数独终局写入文件
    ofstream fout("EndGames.txt");
    if (!fout.is_open()) {
        cout << "Error: cannot open file EndGames.txt" << endl;
        return;
    }
    for (int cur = 0;cur < num;cur++)
    {
        fout << "EndGame" << cur + 1 << endl;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++) {
                fout << endgames[cur][i][j] << " ";
            }
            fout << endl;
        }
    }
    fout.close();
    cout << "Successfully wrote sudoku to file EndGames.txt" << endl;
}

//生成数独游戏--普通版
void generate_sudoku_games(int num)
{
    //合法性判断
    if (num < 1 || num>10000)
    {
        cout << "生成数独游戏数目不合法" << endl;
        return;
    }
    string filepath;
    for (int cur = 0;cur < num;cur++)
    {
        srand(time(NULL)+cur);
        // 随机填充第一行
        for (int i = 0; i < N; i++)
        {
            matrix[0][i] = i + 1;
        }
        for (int i = 0; i < N; i++)
        {
            int random_index = rand() % N;
            swap(matrix[0][i], matrix[0][random_index]);
        }
        // 递归填充数独矩阵
        solve_sudoku(0, 0);
        // 随机挖去部分数字
        int holes = rand() % 10 + 41; // 挖去41~50个数字
        set<pair<int, int>> hole_set; // 记录已经挖过的洞的位置
        while (holes > 0) {
            int row = rand() % N;
            int col = rand() % N;
            if (hole_set.find({ row, col }) == hole_set.end()) {
                hole_set.insert({ row, col }); // 将位置加入集合
                matrix[row][col] = 0; // 挖掉这个位置
                holes--; // 洞数减 1
            }
        }
        stringstream ss;
        ss << cur + 1; // 将整数类型的变量 cur 转换为字符串
        //存储到不同文件中
        filepath = "SudokuGames" + ss.str() + ".txt";
        ofstream fout(filepath);
        if (!fout.is_open()) {
            cout << "Error: cannot open file SudokuGames.txt" << endl;
            return;
        }
        char temp;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++) {
                if (matrix[i][j] == 0)
                {
                    temp = '$';
                    fout << temp << " ";
                }
                else
                {
                    fout << matrix[i][j] << " ";
                }
            }
            fout << endl;
        }
        fout.close();
        cout << "Successfully wrote sudoku to file" <<filepath <<endl;
    }

}

//生成数独游戏--带难度值
//1简单有 30 - 40 个空;2中等有 40 - 50 个空;困难有 50 - 60 个空
void generate_sudoku_games_difficulty(int num, int difficulty)
{
    //合法性判断
    if (num < 1 || num>10000)
    {
        cout << "生成数独游戏数目不合法" << endl;
        return;
    }
    int base;
    if (difficulty == 1)
    {
        base = 30;
    }
    else if(difficulty == 2)
    {
        base = 40;
    }
    else
    {
        base = 50;
    }
    string filepath;
    for (int cur = 0;cur < num;cur++)
    {
        srand(time(NULL) + cur);
        // 随机填充第一行
        for (int i = 0; i < N; i++)
        {
            matrix[0][i] = i + 1;
        }
        for (int i = 0; i < N; i++)
        {
            int random_index = rand() % N;
            swap(matrix[0][i], matrix[0][random_index]);
        }
        // 递归填充数独矩阵
        solve_sudoku(0, 0);
        // 随机挖去部分数字
        int holes = rand() % 10 + base; // 挖去数字数目随机
        set<pair<int, int>> hole_set; // 记录已经挖过的洞的位置
        while (holes > 0) {
            int row = rand() % N;
            int col = rand() % N;
            if (hole_set.find({ row, col }) == hole_set.end()) {
                hole_set.insert({ row, col }); // 将位置加入集合
                matrix[row][col] = 0; // 挖掉这个位置
                holes--; // 洞数减 1
            }
        }
        stringstream ss;
        ss << cur + 1; // 将整数类型的变量 cur 转换为字符串
        //存储到不同文件中
        filepath = "SudokuGamesWithDifficulty" + ss.str() + ".txt";
        ofstream fout(filepath);
        if (!fout.is_open()) {
            cout << "Error: cannot open file SudokuGames.txt" << endl;
            return;
        }
        char temp;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++) {
                if (matrix[i][j] == 0)
                {
                    temp = '$';
                    fout << temp << " ";
                }
                else
                {
                    fout << matrix[i][j] << " ";
                }
            }
            fout << endl;
        }
        fout.close();
        cout << "Successfully wrote sudoku to file" << filepath << endl;
    }
}

//生成数独游戏--带空格指定
void generate_sudoku_games_blanks(int num, int min_blanks, int max_blanks)
{
    //合法性判断
    if (num < 1 || num>10000)
    {
        cout << "生成数独游戏数目不合法" << endl;
        return;
    }
    // 使用默认的随机数生成器和均匀分布函数
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min_blanks, max_blanks);//限定范围
    string filepath;
    for (int cur = 0;cur < num;cur++)
    {
        srand(time(NULL) + cur);
        // 随机填充第一行
        for (int i = 0; i < N; i++)
        {
            matrix[0][i] = i + 1;
        }
        for (int i = 0; i < N; i++)
        {
            int random_index = rand() % N;
            swap(matrix[0][i], matrix[0][random_index]);
        }
        // 递归填充数独矩阵
        solve_sudoku(0, 0);
        // 随机挖去部分数字
        int holes = dis(gen); // 挖去数字数目随机
        set<pair<int, int>> hole_set; // 记录已经挖过的洞的位置
        while (holes > 0) {
            int row = rand() % N;
            int col = rand() % N;
            if (hole_set.find({ row, col }) == hole_set.end()) {
                hole_set.insert({ row, col }); // 将位置加入集合
                matrix[row][col] = 0; // 挖掉这个位置
                holes--; // 洞数减 1
            }
        }
        stringstream ss;
        ss << cur + 1; // 将整数类型的变量 cur 转换为字符串
        //存储到不同文件中
        filepath = "SudokuGamesWithBlanks" + ss.str() + ".txt";
        ofstream fout(filepath);
        if (!fout.is_open()) {
            cout << "Error: cannot open file SudokuGames.txt" << endl;
            return;
        }
        char temp;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++) {
                if (matrix[i][j] == 0)
                {
                    temp = '$';
                    fout << temp << " ";
                }
                else
                {
                    fout << matrix[i][j] << " ";
                }
            }
            fout << endl;
        }
        fout.close();
        cout << "Successfully wrote sudoku to file" << filepath << endl;
    }
}



//生成数独游戏--唯一解
void generate_sudoku_unique(int cur)
{
    string filepath;
    srand(time(NULL) + cur);
    // 随机填充第一行
    for (int i = 0; i < N; i++)
    {
        matrix[0][i] = i + 1;
    }
    for (int i = 0; i < N; i++)
    {
        int random_index = rand() % N;
        swap(matrix[0][i], matrix[0][random_index]);
    }
    // 递归填充数独矩阵
    solve_sudoku(0, 0);
    //记录当前矩阵
    memcpy(unique_matrix, matrix, sizeof(matrix));
    int dig_times = 0;//挖取次数尝试上限
    set<pair<int, int>> hole_set; // 记录已经挖过的洞的位置
    while (hole_set.size() <= 25)
    {
        dig_times++;
        //cout << "dig times=" << dig_times << endl;
        //记录当前矩阵
        memcpy(matrix, unique_matrix, sizeof(matrix));
        //随机挖空
        srand(time(NULL) + cur);
        int row = rand() % N;
        int col = rand() % N;
        if (hole_set.find({ row, col }) == hole_set.end())
        {
            hole_set.insert({ row, col }); // 将位置加入集合
            matrix[row][col] = 0; // 挖掉这个位置
            int count = 0;
            only_solve_sudoku(0, 0, count);
            if (count == 1)//有唯一解则挖掉这个位置
            {
                unique_matrix[row][col] = 0;
                cout << "dig(" << row << "," << col << ")" << endl;
                if (hole_set.size() >= 40)
                {
                    break;
                }
            }
            else if (count == 0)
            {
                break;
            }
        }
    }
    stringstream ss;
    ss << cur + 1; // 将整数类型的变量 cur 转换为字符串
    //存储到不同文件中
    filepath = "UniqueSudokuGames" + ss.str() + ".txt";
    ofstream fout(filepath);
    if (!fout.is_open())
    {
        cout << "Error: cannot open file UniqueSudokuGames.txt" << endl;
        return;
    }
    char temp;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++) {
            if (unique_matrix[i][j] == 0)
            {
                temp = '$';
                fout << temp << " ";
            }
            else
            {
                fout << unique_matrix[i][j] << " ";
            }
        }
        fout << endl;
    }
    fout.close();
    cout << "Successfully wrote sudoku to file " << filepath << endl;
}

void generate_sudoku_uniques(int num)
{
    //合法性判断
    if (num < 1 || num>10000)
    {
        cout << "生成数独游戏数目不合法" << endl;
        return;
    }
    for (int i = 0;i < num;i++)
    {
        memset(matrix, 0, sizeof(matrix));
        cout << "start to generate unique sudoku" << i + 1 << endl;
        generate_sudoku_unique(i);
    }
}



// 从文件中读取数独问题
void read_sudoku_from_file(const char* filename) 
{
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Error: cannot open file " << filename << endl;
        return;
    }
    memset(matrix, 0, sizeof(matrix));
    char temp;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fin >> temp;
            if (temp == '$')
            {
                temp = '0';
            }
            temp = temp - '0';
            matrix[i][j] = temp;
        }
    }
    fin.close();
    cout << "Successfully read sudoku from file " << filename << endl;
}

// 将数独解写入文件
void write_solution_to_file(const char* filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Error: cannot open file " << filename << endl;
        return;
    }
    if (solve_sudoku(0, 0)) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fout << matrix[i][j] << " ";
            }
            fout << endl;
        }
        cout << "Successfully wrote solution to file " << filename << endl;
    }
    else {
        cout << "Error: cannot solve the sudoku problem" << endl;
    }
    fout.close();
}
