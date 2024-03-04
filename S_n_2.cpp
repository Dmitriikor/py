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
#include <unordered_map>

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

class FieldCell
{
    friend class Game_2;
    friend class Snake_2;
    friend class Field;

    std::pair<SHORT, SHORT> coord;

public:
    char symbol;
    size_t index;




    FieldCell() = default;
    FieldCell(std::pair<SHORT, SHORT> coord, char symbol) : coord(coord), symbol(symbol) {}
    FieldCell(std::pair<SHORT, SHORT> coord) : coord(coord) {}
    FieldCell(std::pair<SHORT, SHORT> coord, char symbol, size_t index) : coord(coord), symbol(symbol), index(index) {}

    bool operator==(const FieldCell &other) const
    {
        return coord == other.coord && symbol == other.symbol;
    }
};

class Field
{
    friend class Game_2;
    friend class Snake_2;
    friend class FieldCell;
    size_t pos = 1;
    std::vector<FieldCell> &general_vW;
    std::unordered_map<char, std::vector<FieldCell>> map_test;



public:
    Field() = default;
    Field(std::vector<FieldCell> &general) : general_vW(general) 
    {
        map_test[EMPTY].reserve(1);
        map_test [BORDER].reserve(1); 
        map_test [SNEAKE_BODY].reserve(1); 
        map_test [FRUIT].reserve(1); 
    }

    Field &operator=(const Field &other)
    {
        if (this != &other)
        {
            general_vW = other.general_vW;
        }
        return *this;
    }


    //findNextFree->fruit
    //set(fruit) for map
    //snake move base on index FieldCell or save vector logic
    

    FieldCell& findNextFree()
    {
        if(map_test[EMPTY].empty())
        {
            return map_test[EMPTY].back();
        }

        //std::unordered_map<char, std::vector<FieldCell>> map;

        std::random_device rd;
        std::mt19937 g(rd());

        std::uniform_int_distribution<size_t> rnd(0, map_test[EMPTY].size() - 1);
        pos = rnd(g);
        //std::shuffle(map_test[EMPTY].begin(), map_test[EMPTY].end(), g);

        

        // for (size_t i = 0; i < general_vW.size(); i++)
        // {
        //     map[general_vW[i].symbol].push_back(general_vW[i]);
        // }
        //FieldCell returnedVal = *map_test[EMPTY].begin();
        //map_test[EMPTY].erase(map_test[EMPTY].begin());

        return map_test[EMPTY][pos];

        //size_t stop = 1; 
        // while (general_vW[pos].symbol != EMPTY)                                                     
        // {
        //     std::uniform_int_distribution<size_t> rnd(0, general_vW.size() - 1);
        //     pos = rnd(g);
        //     if (stop == general_vW.size()*10)
        //     {
        //         throw std::exception("no free space");
        //     }
        //     ++stop;
        // }
        // return general_vW[pos]; //pos = pos + 1 after access to pos
    }
    // void transferMap()
    // {
    //     for (size_t i = 0; i < general_vW.size(); i++)
    //     {
    //         map[general_vW[i].symbol].push_back(general_vW[i]);
    //     }  
    // }
    void set(FieldCell &block)
    {
        general_vW[block.coord.first * COLS + block.coord.second].symbol = block.symbol;
    }
    FieldCell get(SHORT x, SHORT y)
    {
        return general_vW[x * COLS + y];
    }
    void generateCoordinates_NON()
    {
        size_t index = 0;
        for (SHORT i = 0; i < ROWS; ++i)
        {
            for (SHORT j = 0; j < COLS; ++j)
            {
                general_vW.push_back(FieldCell(std::pair<SHORT, SHORT>(i, j), EMPTY, index));
                map_test[EMPTY].push_back(FieldCell(std::pair<SHORT, SHORT>(i, j), EMPTY, index));
                index++;
            }
        }
        // std::random_device rd;
        // std::mt19937 g(rd());
        // std::shuffle(general_vW.begin(), general_vW.end(), g);
        
    }
    void drawField()
    {
        FieldCell border(std::pair<SHORT, SHORT>(SHORT_CAST(0), SHORT_CAST(0)), BORDER);

        for (SHORT i = 0; i < COLS; i++)
        {
            border.coord.first = 0;
            border.coord.second = i;
            set(border);

            setMap(border, EMPTY, BORDER);
            //TODO copy code, move to function
            // for (auto it = map_test[EMPTY].begin(); it != map_test[EMPTY].end(); ++it)
            // {
            //     if (it->coord == border.coord)
            //     {
            //         it->symbol = BORDER;
            //         map_test[BORDER].push_back(*it);
            //         map_test[EMPTY].erase(it);
            //         break;
            //     }
            // }

        }
        for (SHORT i = 0; i < ROWS; i++)
        {
            border.coord.first = i;
            border.coord.second = 0;
            set(border);

            //TODO copy code, move to function
            for (auto it = map_test[EMPTY].begin(); it != map_test[EMPTY].end(); ++it)
            {
                if (it->coord == border.coord)
                {
                    it->symbol = BORDER;
                    map_test[BORDER].push_back(*it);
                    map_test[EMPTY].erase(it);
                    break;
                }
            }

            border.coord.second = COLS - 1;
            set(border);

            //TODO copy code, move to function
            for (auto it = map_test[EMPTY].begin(); it != map_test[EMPTY].end(); ++it)
            {
                if (it->coord == border.coord)
                {
                    it->symbol = BORDER;
                    map_test[BORDER].push_back(*it);
                    map_test[EMPTY].erase(it);
                    break;
                }
            }
        }
        for (SHORT i = 0; i < COLS; i++)
        {
            border.coord.first = ROWS - 1;
            border.coord.second = i;
            set(border);

            //TODO copy code, move to function
            for (auto it = map_test[EMPTY].begin(); it != map_test[EMPTY].end(); ++it)
            {
                if (it->coord == border.coord)
                {
                    it->symbol = BORDER;
                    map_test[BORDER].push_back(*it);
                    map_test[EMPTY].erase(it);
                    break;
                }
            }
        }
    }
void setMap(FieldCell &block, char mapKey, char insertKey)
{
    for (auto it = map_test[mapKey].begin(); it != map_test[mapKey].end(); ++it)
    {
        if (it->coord == block.coord)
        {
            it->symbol = insertKey;
            map_test[insertKey].push_back(*it);
            map_test[mapKey].erase(it);
            return;
        }
    }
    throw std::logic_error("setMap error");
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

    Field& v;
    std::vector<FieldCell> &general_sn;
    Direction currentDirection;
    size_t index = 0;
    std::list<FieldCell> list;


    //TODO взять координаты из Field
    bool isFruitNon = true; 
    FieldCell fruit;

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

        FieldCell old_tail;
        auto it = list.end();
        --it;
        old_tail = *it;

        if(isFruitNon)
        {
            v.setMap(*list.begin(), EMPTY, SNEAKE_BODY);
            fruit = v.findNextFree(); //FieldCell(std::pair<SHORT, SHORT>(SHORT_CAST(5), SHORT_CAST(7)), FRUIT);
            v.setMap(fruit, EMPTY, FRUIT);

            fruit.symbol = FRUIT;
            v.set(fruit);
            isFruitNon = false;
        }
        

        if (tempX == fruit.coord.second && tempY == fruit.coord.first)
        {
            // fruit_Eat();
            
            list.push_front(fruit);
            list.front().symbol = SNEAKE_BODY;
            v.set(*list.begin());

            v.setMap(fruit, FRUIT, SNEAKE_BODY);

            fruit = v.findNextFree(); //FieldCell(std::pair<SHORT, SHORT>(SHORT_CAST(5), SHORT_CAST(7)), FRUIT);
            v.setMap(fruit, EMPTY, FRUIT);
            fruit.symbol = FRUIT;
            v.set(fruit);

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
        std::list<FieldCell> list2;

        FieldCell lastElement = list.back();
        lastElement.coord.first = list.front().coord.first;
        lastElement.coord.second = list.front().coord.second;

        list.pop_back();
        list.push_front(lastElement);

        list = list;
    }

public:
    Snake_2();
    Snake_2(std::vector<FieldCell> &general, Direction currentDirection, Field &v) : general_sn(general),
                                                                                  currentDirection(currentDirection),
                                                                                  v(v)
    {
        list.emplace_front(FieldCell(std::pair<SHORT, SHORT>(SHORT_CAST(ROWS / 2), SHORT_CAST(COLS / 2)), SNEAKE_BODY));

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
    friend class Field;
    friend class FieldCell;
    Field v;
    Snake_2 s;
    std::vector<FieldCell> general;
    //Direction currentDirection;

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
    // void transferMap()
    // {
    //     v.transferMap();
    // }
    void move()
    {
        s.move();
    }
    void keyScan()
    {
        Direction temp = keyScan_2();
        if(temp != Direction::NONE)
            s.currentDirection = temp;
        
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


        // std::vector <std::vector<char>> toPrint(ROWS, std::vector<char>(COLS, ' '));

        // for(auto i = v.map_test.begin(); i != v.map_test.end(); ++i)
        // {
        //     for(auto j = i->second.begin(); j != i->second.end(); ++j)
        //     {
        //         toPrint[j->coord.first][j->coord.second] = j->symbol;
        //     }
        // }

        // //TODO ade sn from list

        // for (size_t i = 0; i < toPrint.size(); i++)
        // {
        //     for (size_t j = 0; j < toPrint[i].size(); j++)
        //     {
        //         std::cout << toPrint[i][j];
        //     }
        //     std::cout << std::endl;
        // }
    }
    Game_2() : v(general), s(general, Direction::UP, v)
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
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    try
    {
        Game_2 game_2;
        while (true)
        {
            Sleep(300);
            //system("cls");
            SetConsoleCursorPosition(console, COORD(0, 0)); //flicker fix
            game_2.keyScan();
            game_2.move();
            //game_2.transferMap();
            game_2.print();
        }

        // {
        //     auto start = std::chrono::steady_clock::now();
        //     FieldCell test(std::pair<SHORT, SHORT>(1, 1), SNEAKE_BODY);
        //     std::list<FieldCell> list {test, test, test, test, test, test, test, test, test, test};
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
        //     FieldCell test(std::pair<SHORT, SHORT>(1, 1), SNEAKE_BODY);
        //     std::deque<FieldCell> q  {test, test, test, test, test, test, test, test, test, test};
        //     for (size_t i = 0; i < 100000; i++)
        //     {
        //         //auto to_swap_back = q.back();
        //         auto to_swap_front = FieldCell(std::pair<SHORT, SHORT>(1, 1), SNEAKE_BODY); //= q.front();
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