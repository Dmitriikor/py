//TODO почистить
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

//TODO !!! лучше в namespace как key
#define EMPTY ' '
#define BORDER '#'
#define SNAKE_BODY '='
#define FRUIT '@'

//TODO красивая ошибка: конец игры
//TODO score
//TODO цвет

//TODO туман войны

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
    private:
    std::pair<SHORT, SHORT> coord;
    //char symbol;
public:
    //FieldCell(std::pair<SHORT, SHORT> coord, char symbol) : coord(coord), symbol(symbol) {}
    FieldCell(SHORT i, SHORT j) : coord(std::pair<SHORT, SHORT>(j, i)) {} 

    SHORT& x()
    {
        return coord.first;
    }

    SHORT& y()
    {
        return coord.second;
    }

    const SHORT& x() const
    {
        return coord.first;
    }

    const SHORT& y() const
    {
        return coord.second;
    }

    bool operator==(const FieldCell &other) const
    {
        return coord == other.coord;
    }
};

class Field
{
public:
    Field()
    {
        cells.reserve(UNSIGNED_CAST(ROWS) * UNSIGNED_CAST(COLS));

        fillWithEmpty();
        fillWithBorders();

        fillVectorOfEmpty();
    }

    FieldCell extractRandomCell()
    {
            size_t pos = findNextFree();
            auto it = vectorOfEmpty.begin() + pos;
            FieldCell extractedCell = *it;

            swapAndErase(*it, vectorOfEmpty.back());

            return extractedCell;
            // FieldCell randomCell = vectorOfEmpty[pos]; //FieldCell(std::pair<SHORT, SHORT>(SHORT_CAST(5), SHORT_CAST(7)), FRUIT);
            // swapAndErase(vectorOfEmpty.back(), *(vectorOfEmpty.begin() + pos));
            // return randomCell;
    }

    FieldCell extractCell(size_t i, size_t j)
    {
        auto it = findCell(FieldCell(i, j));
        FieldCell extractedCell = *it;

        swapAndErase(*it, vectorOfEmpty.back());

        return extractedCell;
    }

    // FieldCell extractCell(SHORT x, SHORT y)
    // {
    //     auto it =  findCell(FieldCell(std::pair<SHORT, SHORT>(x, y)));
    //     FieldCell extractedCell = *it;

    //     swapAndErase(*it, vectorOfEmpty.back());

    //     return extractedCell;
    // }

    std::vector<FieldCell>::iterator findCell(const FieldCell &block)
    {
        auto it = std::find(vectorOfEmpty.begin(), vectorOfEmpty.end(), block);

        if (it == vectorOfEmpty.end())
            throw std::logic_error("findCell error");

        return it; 
    }

    void returnCell(const FieldCell &block)
    {
        vectorOfEmpty.push_back(block);
        set(vectorOfEmpty.back(), EMPTY);
    }

    void set(const FieldCell &block, char symbol)
    {
        cells[block.y() * COLS + block.x()] = symbol;
    }

    // char get(SHORT x, SHORT y) const
    // {
    //     return cells[x * COLS + y];
    // }

    void print() const
    {
        for (int i = 0; i < ROWS; ++i)
        {
            for (int j = 0; j < COLS; ++j)
            {
                std::cout << cells[i * COLS + j]<< " ";
            }
            std::cout << std::endl;
        }
    }

    private:
    std::vector<char> cells;
    std::vector<FieldCell> vectorOfEmpty;

     void swapAndErase(FieldCell &a, FieldCell &b)
    {
        std::swap(a, b);
        vectorOfEmpty.pop_back();
    }

    size_t findNextFree()
    {
        std::random_device rd;
        std::mt19937 g(rd());

        std::uniform_int_distribution<size_t> rnd(0, vectorOfEmpty.size() - 1);
        size_t pos = rnd(g);

        return pos;
    }

    void fillWithEmpty()
    {
        for (SHORT i = 0; i < ROWS; ++i)
        {
            for (SHORT j = 0; j < COLS; ++j)
            {
                cells.push_back(EMPTY);
            }
        }
    }

     void fillWithBorders()
    {
        for (SHORT j = 0; j < COLS; j++)
        {
            set(FieldCell(0, j), BORDER);
            set(FieldCell(ROWS - 1, j), BORDER);
        }

        for (SHORT i = 0; i < ROWS; i++)
        {
            set(FieldCell(i, 0), BORDER);
            set(FieldCell(i, COLS - 1), BORDER);
        }
    }

    void fillVectorOfEmpty()
    {
        for (SHORT i = 0; i < ROWS; ++i)
        {
            for (SHORT j = 0; j < COLS; ++j)
            {
                if (cells[i * COLS + j] == EMPTY)
                {
                    vectorOfEmpty.push_back(FieldCell(i, j));
                }
            }
        }
    }
};

class Snake_2
{
    public:
    Snake_2(Direction currentDirection, Field &field) 

    :currentDirection(currentDirection), field(field),  list(1, field.extractCell(ROWS / 2, COLS / 2)), fruit(field.extractRandomCell())
    {
        field.set(fruit, FRUIT);
        field.set(*list.begin(), SNAKE_BODY);
    }

    void move()
    {
        FieldCell newCell = *list.begin();

        if (currentDirection == (Direction::UP))
            newCell.y() += -1;
        else if (currentDirection == (Direction::DOWN))
            newCell.y() += 1;
        else if (currentDirection == (Direction::LEFT))
            newCell.x() += -1;
        else if (currentDirection == (Direction::RIGHT))
            newCell.x() += 1;

        if (newCell == fruit)
        {
            list.push_front(newCell);
            field.set(newCell, SNAKE_BODY);

            fruit = field.extractRandomCell();
            field.set(fruit, FRUIT);
        }
        else
        {
            FieldCell newHead = field.extractCell(newCell.y(), newCell.x());
            list.push_front(newHead);
            field.set(list.front(), SNAKE_BODY);

            field.returnCell(list.back()); //field.set(list.back(), EMPTY);
            list.pop_back();
        }


        // auto it = list.end();
        // --it;
        // FieldCell old_tail = *it;
        // field.set(list.back(), EMPTY);
        // list.pop_back();
        // list.push_front(newCell);
        // field.set(list.front(), SNAKE_BODY);
        //     {
        //     auto it = field.findCell(list.front());
        //     it->coord.first = old_tail.coord.first;
        //     it->coord.second = old_tail.coord.second;
        //     } 
//field.print();
        //old_tail.symbol = EMPTY;
        //field.set(old_tail);
//field.print();
        //field.set(*list.begin());
//field.print();
        //field.set(fruit);
//field.print();
    }
    void changeDirection(Direction newDirection)
    {
        if (newDirection == Direction::NONE)
            return;

        currentDirection = newDirection;
    }

    private:

    Direction currentDirection;
    Field& field;
    std::list<FieldCell> list; //std::list<COORD> list;
    FieldCell fruit;
};

class Game_2
{
public:
    Game_2() : snake(Direction::UP, field), field()
    {

    }

    void keyScan()
    {
        Direction direction = keyScan_2();
       snake.changeDirection(direction);
        
    }

    void move()
    {
        snake.move();
    }

    void print() const
    {
        field.print();
    }

    private:

    Field field;
    Snake_2 snake;

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
            game_2.print();
            SetConsoleCursorPosition(console, COORD(0, 0));
        }

        // {
        //     auto start = std::chrono::steady_clock::now();
        //     FieldCell test(std::pair<SHORT, SHORT>(1, 1), SNAKE_BODY);
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
        //     FieldCell test(std::pair<SHORT, SHORT>(1, 1), SNAKE_BODY);
        //     std::deque<FieldCell> q  {test, test, test, test, test, test, test, test, test, test};
        //     for (size_t i = 0; i < 100000; i++)
        //     {
        //         //auto to_swap_back = q.back();
        //         auto to_swap_front = FieldCell(std::pair<SHORT, SHORT>(1, 1), SNAKE_BODY); //= q.front();
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