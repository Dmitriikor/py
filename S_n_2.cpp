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
#define BORDER '#'
#define SNEAKE_BODY '='
#define FRUIT '@'

    enum class Key
    {
        w = 'w',
        a = 'a',
        d = 'd',
        s = 's',
        ESC = 27
    };
    enum class Direction
    {
        UP = Key::w,
        DOWN = Key::s,
        LEFT = Key::a,
        RIGHT = Key::d
    };

static const SHORT ROWS = 15; //�����
static const SHORT COLS = 15; //��������

class Block_2
{
    friend class Game_2;
    friend class Snake_2;
    friend class vecWorker;

    std::pair<SHORT, SHORT> coord;
    char symbol;
public:
    Block_2() = default;
    Block_2(std::pair<SHORT, SHORT> coord, char symbol) : coord(coord), symbol(symbol) {}
    bool operator==(const Block_2& other) const 
    {
        return coord == other.coord && symbol == other.symbol;
    }

};

class vecWorker
{
    friend class Game_2;
    friend class Snake_2;
    friend class Block_2;

    std::vector<Block_2>& general_vW;

public:
    vecWorker() = default;
    vecWorker(std::vector<Block_2>& general) : general_vW(general) {}
    vecWorker& operator=(const vecWorker& other) 
    {
        if (this != &other) {
            general_vW = other.general_vW;
        }
        return *this;
    }
    
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
        Block_2 border(std::pair <SHORT, SHORT>(SHORT_CAST(0), SHORT_CAST(0)), BORDER);

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

class Snake_2
{   
    friend class Game_2;

    vecWorker v;
    std::vector<Block_2>& general_sn;
    int& Pressed_Key;
    size_t index = 0;
    Block_2 head = Block_2(std::pair<SHORT, SHORT>(SHORT_CAST(ROWS / 2), SHORT_CAST(COLS / 2)), SNEAKE_BODY);
    Block_2 tail = head;
    Block_2 fruit= Block_2(std::pair<SHORT, SHORT>(SHORT_CAST(1), SHORT_CAST(1)), FRUIT);

    void move()
    {
            SHORT x = 0, y = 0;
            if (Pressed_Key == INT_CAST(Direction::UP))
                y = -1;
            else if (Pressed_Key == INT_CAST(Direction::DOWN))
                y = 1;
            else if (Pressed_Key == INT_CAST(Direction::LEFT))
                x = -1;
            else if (Pressed_Key == INT_CAST(Direction::RIGHT))
                x = 1;
            //tail.coord.first
            SHORT tempX = head.coord.second + x;
            SHORT tempY = head.coord.first + y;

            Block_2 old_tail = tail;
            Block_2 old_head = head;

            if (tempX == fruit.coord.second && tempY == fruit.coord.first)
            {
                //fruit_Eat();

                fruit.symbol = SNEAKE_BODY;
                head.coord.first = fruit.coord.first;
                head.coord.second = fruit.coord.second;

                v.set(head);
                
                return;
            }

            moveTailToBeginning();

            head.coord.second += x;
            head.coord.first += y;

            v.set(head);
            old_tail.symbol = EMPTY;
            old_head.symbol = EMPTY;
            v.set(tail);
            v.set(old_tail);
            v.set(old_head);
            v.set(fruit);
    }

    void moveTailToBeginning()
    {
        if (head == tail)
            return;

        tail.coord.first = head.coord.first;
        tail.coord.second = head.coord.second;
        tail.symbol = head.symbol;
    }



public:
    Snake_2();
    Snake_2(std::vector<Block_2>& general, int& Pressed_Key, vecWorker& v) : general_sn(general), 
                                                                            Pressed_Key(Pressed_Key), 
                                                                            v(v) {}

    Snake_2& operator=(const Snake_2& other) 
    {
        if (this != &other) 
        {
            general_sn = other.general_sn;
            Pressed_Key = other.Pressed_Key;
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
    int Pressed_Key = INT_CAST(Key::w);



public:
    void move()
    {
        s.move();
    }
    void keyScan()
    {
        if (_kbhit())
        {
            int tempKey = _getch();
            if (tempKey == INT_CAST(Key::a) || tempKey == INT_CAST(Key::d) || tempKey == INT_CAST(Key::w) || tempKey == INT_CAST(Key::s))
                Pressed_Key = tempKey;
            else
                return;
        }
        else
            return;
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
    Game_2() : v(general), s(general, Pressed_Key, v), Pressed_Key(INT_CAST(Key::w))
    {
        general.reserve(UNSIGNED_CAST(ROWS) * UNSIGNED_CAST(COLS));

        v.generateCoordinates_NON();
        v.drawField();
    }
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
    while(true)
    {
        system("cls");
        game_2.keyScan();
        game_2.move();
        game_2.print();
        Sleep(300);
    }
    // {
    //     auto start = std::chrono::steady_clock::now();
    //     game_2.generateCoordinates_NON();
    //     auto end = std::chrono::steady_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "������� ����������� NON = " << duration.count() << " �����������." << std::endl;
    // }
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}


system("pause");
    return 0;
}