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
#include <list>
#include <deque>

#define INT_CAST(x) static_cast<int>(x)
#define SHORT_CAST(x) static_cast<SHORT>(x)
#define UNSIGNED_CAST(x) static_cast<unsigned>(x)

//!!! unordered map
#define EMPTY ' '
#define BORDER '#'
#define SNEAKE_BODY '='
#define FRUIT '@'

namespace key
{
    const int SPECIAL_KEY = 224;
    const int ARROW_UP = 72;
    const int ARROW_DOWN = 80;
    const int ARROW_LEFT = 75;
    const int ARROW_RIGHT = 77;
    const int W_KEY = 'w';
    const int A_KEY = 'a';
    const int D_KEY = 'd';
    const int S_KEY = 's';
    const int ESC = 27;

}

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

static const SHORT ROWS = 15; // строк
static const SHORT COLS = 15; // столбцов

class Block_2
{
    friend class Game_2;
    friend class Snake_2;
    friend class vecWorker;

    std::pair<SHORT, SHORT> coord;

public:
    char symbol;
    Block_2() = default;
    Block_2(std::pair<SHORT, SHORT> coord, char symbol) : coord(coord), symbol(symbol) {}
    bool operator==(const Block_2 &other) const
    {
        return coord == other.coord && symbol == other.symbol;
    }
};

class vecWorker
{
    friend class Game_2;
    friend class Snake_2;
    friend class Block_2;

    std::vector<Block_2> &general_vW;

public:
    vecWorker() = default;
    vecWorker(std::vector<Block_2> &general) : general_vW(general) {}
    vecWorker &operator=(const vecWorker &other)
    {
        if (this != &other)
        {
            general_vW = other.general_vW;
        }
        return *this;
    }

    void set(Block_2 &block)
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
                general_vW.push_back(Block_2(std::pair<SHORT, SHORT>(i, j), EMPTY));
            }
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(general_vW.begin(), general_vW.end(), g);
    }
    void drawField()
    {
        Block_2 border(std::pair<SHORT, SHORT>(SHORT_CAST(0), SHORT_CAST(0)), BORDER);

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
            border.coord.second = COLS - 1;
            set(border);
        }
        for (SHORT i = 0; i < COLS; i++)
        {
            border.coord.first = ROWS - 1;
            border.coord.second = i;
            set(border);
        }
    }
    void print()
    {
        for (int i = 0; i < ROWS; ++i)
        {
            for (int j = 0; j < COLS; ++j)
            {
                std::cout << general_vW[i * COLS + j].symbol << " ";
            }
            std::cout << std::endl;
        }
    }
};

class Snake_2
{
    friend class Game_2;

    vecWorker v;
    std::vector<Block_2> &general_sn;
    Direction &currentDirection;
    size_t index = 0;
    std::list<Block_2> list;

    //TODO взять координаты из vecWorker
    Block_2 fruit = Block_2(std::pair<SHORT, SHORT>(SHORT_CAST(1), SHORT_CAST(1)), FRUIT);

    void move()
    {
        SHORT x = 0, y = 0;
        if (currentDirection == (Direction::UP))
            y = -1;
        else if (currentDirection == (Direction::DOWN))
            y = 1;
        else if (currentDirection == (Direction::LEFT))
            x = -1;
        else if (currentDirection == (Direction::RIGHT))
            x = 1;
        // tail.coord.first
        SHORT tempX = list.begin()->coord.second + x;
        SHORT tempY = list.begin()->coord.first + y;

        Block_2 old_head = list.front();
        Block_2 old_tail;

        if (list.size() == 1)
            old_tail = old_head;
        else
            old_tail = list.back();

        if (tempX == fruit.coord.second && tempY == fruit.coord.first)
        {
            // fruit_Eat();

            fruit.symbol = SNEAKE_BODY;
            list.begin()->coord.first = fruit.coord.first;
            list.begin()->coord.second = fruit.coord.second;

            v.set(*list.begin());

            return;
        }

        moveTailToBeginning();

        list.begin()->coord.second += x;
        list.begin()->coord.first += y;

//v.print();
        old_tail.symbol = EMPTY;
        v.set(old_tail);
//v.print();
        v.set(*list.begin());
//v.print();
        v.set(fruit);
//v.print();


    }

    void moveTailToBeginning()
    {
        auto it = list.end();
        --it;
        list.splice(list.begin(), list, it, list.end());
    }

public:
    Snake_2();
    Snake_2(std::vector<Block_2> &general, Direction &currentDirection, vecWorker &v) : general_sn(general),
                                                                                  currentDirection(currentDirection),
                                                                                  v(v)
    {
        list.emplace_front(Block_2(std::pair<SHORT, SHORT>(SHORT_CAST(ROWS / 2), SHORT_CAST(COLS / 2)), SNEAKE_BODY));
        // list.begin();
        // list.end();
    }

    Snake_2 &operator=(const Snake_2 &other)
    {
        if (this != &other)
        {
            general_sn = other.general_sn;
            currentDirection = other.currentDirection;
            v = other.v;
        }
        return *this;
    }
};

class Game_2
{
    friend class Snake_2;
    friend class vecWorker;
    friend class Block_2;
    vecWorker v;
    Snake_2 s;
    std::vector<Block_2> general;
    Direction currentDirection;

   static Direction keyScan_2()
    {
        if (_kbhit())
        {
            int tempKey = _getch();

            if (tempKey == key::SPECIAL_KEY)
            {
                tempKey = _getch();
            }

            if (tempKey == key::A_KEY || tempKey == key::ARROW_LEFT)
            {
                return Direction::LEFT;
            }
            if (tempKey == key::S_KEY || tempKey == key::ARROW_DOWN)
            {
                return Direction::DOWN;
            }
            if (tempKey == key::W_KEY || tempKey == key::ARROW_UP)
            {
                return Direction::UP;
            }
            if (tempKey == key::D_KEY || tempKey == key::ARROW_RIGHT)
            {
                return Direction::RIGHT;
            }
        }
        
        return Direction::NONE;

    }


public:
    void move()
    {
        s.move();
    }
    void keyScan()
    {
        Direction temp = keyScan_2();
        if(temp != Direction::NONE)
            currentDirection = temp;
        
    }
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
    Game_2() : v(general), s(general, currentDirection, v), currentDirection(Direction::UP)
    {
        general.reserve(UNSIGNED_CAST(ROWS) * UNSIGNED_CAST(COLS));

        v.generateCoordinates_NON();
        v.drawField();
    }
};

std::ostream &operator<<(std::ostream &ostr, const std::list<int> &list)
{
    for (auto &i : list)
        ostr << ' ' << i;

    return ostr;
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
        while (true)
        {
            system("cls");
            game_2.keyScan();
            game_2.move();
            game_2.print();
            Sleep(300);
        }

        // {
        //     auto start = std::chrono::steady_clock::now();
        //     Block_2 test(std::pair<SHORT, SHORT>(1, 1), SNEAKE_BODY);
        //     std::list<Block_2> list {test, test, test, test, test, test, test, test, test, test};
        //     for (size_t i = 0; i < 100000; i++)
        //     {
        //         auto it = list.end();
        //         --it;
        //         list.splice(list.begin(), list, it, list.end());
        //     }
        //     //Функция выполнялась NON = 4 миллисекунд.
        //     auto end = std::chrono::steady_clock::now();
        //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        //     std::cout << "Функция выполнялась NON = " << duration.count() << " миллисекунд." << std::endl;
        // }
        // {
        //     auto start = std::chrono::steady_clock::now();
        //     Block_2 test(std::pair<SHORT, SHORT>(1, 1), SNEAKE_BODY);
        //     std::deque<Block_2> q  {test, test, test, test, test, test, test, test, test, test};
        //     for (size_t i = 0; i < 100000; i++)
        //     {
        //         //auto to_swap_back = q.back();
        //         auto to_swap_front = Block_2(std::pair<SHORT, SHORT>(1, 1), SNEAKE_BODY); //= q.front();
        //         q.push_front(to_swap_front);
        //         q.pop_back();
        //         //q.push_back(to_swap_front);
        //         //q.pop_back();
        //     }
        //      //Функция выполнялась NON = 38 миллисекунд.
        //     auto end = std::chrono::steady_clock::now();
        //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        //     std::cout << "Функция выполнялась NON = " << duration.count() << " миллисекунд." << std::endl;
        // }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    system("pause");
    return 0;
}