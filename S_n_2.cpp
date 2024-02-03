#include <conio.h>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>
#include <execution>
#include <chrono>
#include <random>
#include <algorithm>

#define INT_CAST(x) static_cast<int>(x)
#define SHORT_CAST(x) static_cast<SHORT>(x)
#define UNSIGNED_CAST(x) static_cast<unsigned>(x)
#define EMPTY ' '

static const SHORT ROWS = 15; //строк
static const SHORT COLS = 15; //столбцов

class Block_2
{
    friend class Game_2;
    friend class Snake_2;
    friend class vecWorker;

    std::pair<SHORT, SHORT> coord;
    char symbol;
public:
    Block_2(std::pair<SHORT, SHORT> coord, char symbol) : coord(coord), symbol(symbol) {}
};

class vecWorker
{
    friend class Game_2;
    friend class Snake_2;
    friend class Block_2;
    std::vector<Block_2>& general_vW;
public:
    vecWorker(std::vector<Block_2>& general) : general_vW(general) {}
    
    void set(Block_2& block)
    {
        general_vW[block.coord.first * COLS + block.coord.second].symbol = block.symbol;
    }
    Block_2 get(SHORT x, SHORT y)
    {
        return general_vW[x * COLS + y];
    }
    void generateCoordinates_NON() 
    {
        for (SHORT i = 0; i < ROWS; ++i) 
        {
            for (SHORT j = 0; j < COLS; ++j) 
            {
                general_vW.push_back(Block_2(std::pair <SHORT, SHORT>(i,j), EMPTY));
            }
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(general_vW.begin(), general_vW.end(), g);
    }
    void drawField()
    {
        Block_2 border(std::pair <SHORT, SHORT>(SHORT_CAST(0), SHORT_CAST(0)), '#');

        for (SHORT i = 0; i < COLS; i++)
        {
            border.coord.first = 0;
            border.coord.second = i;
            set(border);
        }
        for (SHORT i = 0; i < ROWS; i++)
        {
            border.coord.first = i;
            border.coord.second = 0;
            set(border);
            border.coord.second = COLS-1;
            set(border);
        }
        for (SHORT i = 0; i < COLS; i++)
        {
            border.coord.first = ROWS-1;
            border.coord.second = i;
            set(border);
        }
    }
};

class Game_2
{
    friend class Snake_2;
    friend class vecWorker;
    std::vector<Block_2> general;

public:
    void print()
    {
        for (int i = 0; i < ROWS; ++i) 
        {    
            for (int j = 0; j < COLS; ++j) 
            {
                std::cout << general[i * COLS + j].symbol << " ";
            }
            std::cout << std::endl;
        }
    }
    Game_2()
    {
        general.reserve(UNSIGNED_CAST(ROWS) * UNSIGNED_CAST(COLS));
        vecWorker v(general);
        v.generateCoordinates_NON();
        v.drawField();
    }
};

class Snake_2
{


};

int main() 
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    SetConsoleOutputCP(1251);

try
{
    Game_2 game_2;

    {
        game_2.print();
        std::cout << "\n";

    }
    // {
    //     auto start = std::chrono::steady_clock::now();
    //     game_2.generateCoordinates_NON();
    //     auto end = std::chrono::steady_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Функция выполнялась NON = " << duration.count() << " миллисекунд." << std::endl;
    // }
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}


system("pause");
    return 0;
}